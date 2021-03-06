// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "0_stdafx.hpp"               // IWYU pragma: associated
#include "1_Internal.hpp"             // IWYU pragma: associated
#include "api/client/Blockchain.hpp"  // IWYU pragma: associated

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <list>
#include <map>
#include <set>
#include <stdexcept>
#include <thread>
#include <type_traits>

#include "Factory.hpp"
#include "internal/api/client/Client.hpp"
#include "internal/api/client/blockchain/Blockchain.hpp"
#include "opentxs/Pimpl.hpp"
#include "opentxs/Proto.hpp"
#include "opentxs/api/Core.hpp"
#if OT_BLOCKCHAIN
#include "opentxs/api/Endpoints.hpp"
#endif  // OT_BLOCKCHAIN
#include "opentxs/api/Factory.hpp"
#include "opentxs/api/Wallet.hpp"
#include "opentxs/api/client/Activity.hpp"
#include "opentxs/api/crypto/Crypto.hpp"
#include "opentxs/api/crypto/Encode.hpp"
#include "opentxs/api/crypto/Hash.hpp"
#include "opentxs/api/storage/Storage.hpp"
#include "opentxs/blockchain/Blockchain.hpp"
#include "opentxs/core/Data.hpp"
#include "opentxs/core/Identifier.hpp"
#include "opentxs/core/Log.hpp"
#include "opentxs/core/LogSource.hpp"
#include "opentxs/identity/Nym.hpp"
#if OT_BLOCKCHAIN
#include "opentxs/network/zeromq/Context.hpp"
#include "opentxs/network/zeromq/Frame.hpp"
#include "opentxs/network/zeromq/FrameSection.hpp"
#include "opentxs/network/zeromq/Message.hpp"
#include "opentxs/network/zeromq/socket/Socket.hpp"
#include "util/Cleanup.hpp"
#endif  // OT_BLOCKCHAIN
#include "opentxs/protobuf/BlockchainEnums.pb.h"
#include "opentxs/protobuf/ContactEnums.pb.h"
#include "opentxs/protobuf/Enums.pb.h"

#define LOCK_NYM()                                                             \
    Lock mapLock(lock_);                                                       \
    auto& nymMutex = nym_lock_[nymID];                                         \
    mapLock.unlock();                                                          \
    Lock nymLock(nymMutex);

#define PATH_VERSION 1
#define COMPRESSED_PUBKEY_SIZE 33

#define OT_METHOD "opentxs::api::client::implementation::Blockchain::"

namespace zmq = opentxs::network::zeromq;

namespace opentxs
{
auto Factory::BlockchainAPI(
    const api::client::internal::Manager& api,
    const api::client::Activity& activity,
    const api::client::Contacts& contacts,
    const api::Legacy& legacy,
    const std::string& dataFolder,
    const ArgList& args) noexcept -> std::unique_ptr<api::client::Blockchain>
{
    using ReturnType = api::client::implementation::Blockchain;

    return std::make_unique<ReturnType>(
        api, activity, contacts, legacy, dataFolder, args);
}
}  // namespace opentxs

namespace opentxs::api::client::implementation
{
const Blockchain::AddressMap Blockchain::address_prefix_map_{
    {Prefix::BitcoinP2PKH, "00"},
    {Prefix::BitcoinP2SH, "05"},
    {Prefix::BitcoinTestnetP2PKH, "6f"},
    {Prefix::BitcoinTestnetP2SH, "c4"},
    {Prefix::LitecoinP2PKH, "30"},
};
const Blockchain::AddressReverseMap Blockchain::address_prefix_reverse_map_{
    reverse_map(address_prefix_map_)};
const Blockchain::StyleMap Blockchain::address_style_map_{
    {{Style::P2PKH, Chain::Bitcoin}, Prefix::BitcoinP2PKH},
    {{Style::P2SH, Chain::Bitcoin}, Prefix::BitcoinP2SH},
    {{Style::P2PKH, Chain::Bitcoin_testnet3}, Prefix::BitcoinTestnetP2PKH},
    {{Style::P2SH, Chain::Bitcoin_testnet3}, Prefix::BitcoinTestnetP2SH},
    {{Style::P2PKH, Chain::BitcoinCash}, Prefix::BitcoinP2PKH},
    {{Style::P2SH, Chain::BitcoinCash}, Prefix::BitcoinP2SH},
    {{Style::P2PKH, Chain::BitcoinCash_testnet3}, Prefix::BitcoinTestnetP2PKH},
    {{Style::P2SH, Chain::BitcoinCash_testnet3}, Prefix::BitcoinTestnetP2SH},
    {{Style::P2PKH, Chain::Litecoin}, Prefix::LitecoinP2PKH},
};
const Blockchain::StyleReverseMap Blockchain::address_style_reverse_map_{
    reverse_map(address_style_map_)};

Blockchain::Blockchain(
    const api::client::internal::Manager& api,
    const api::client::Activity& activity,
    const api::client::Contacts& contacts,
    [[maybe_unused]] const api::Legacy& legacy,
    [[maybe_unused]] const std::string& dataFolder,
    [[maybe_unused]] const ArgList& args) noexcept
    : api_(api)
    , activity_(activity)
    , contacts_(contacts)
    , lock_()
    , nym_lock_()
    , balance_lists_(*this)
    , txo_db_(*this)
#if OT_BLOCKCHAIN
    , thread_pool_(api)
    , io_(api)
    , db_(api, legacy, dataFolder, args)
    , reorg_(api_.ZeroMQ().PublishSocket())
    , networks_()
    , balances_(*this, api)
#endif  // OT_BLOCKCHAIN
{
    // WARNING: do not access api_.Wallet() during construction
#if OT_BLOCKCHAIN
    auto listen = reorg_->Start(api_.Endpoints().BlockchainReorg());

    OT_ASSERT(listen);
#endif  // OT_BLOCKCHAIN
}

#if OT_BLOCKCHAIN
Blockchain::BalanceOracle::BalanceOracle(
    const api::client::internal::Blockchain& parent,
    const api::Core& api) noexcept
    : parent_(parent)
    , api_(api)
    , zmq_(api_.ZeroMQ())
    , cb_(zmq::ListenCallback::Factory([this](auto& in) { cb(in); }))
    , socket_(zmq_.RouterSocket(cb_, zmq::socket::Socket::Direction::Bind))
    , lock_()
    , subscribers_()
{
    const auto started = socket_->Start(api_.Endpoints().BlockchainBalance());

    OT_ASSERT(started);
}
#endif  // OT_BLOCKCHAIN

Blockchain::BalanceLists::BalanceLists(
    api::client::internal::Blockchain& parent) noexcept
    : parent_(parent)
    , lock_()
    , lists_()
{
}

#if OT_BLOCKCHAIN
Blockchain::ThreadPoolManager::ThreadPoolManager(const api::Core& api) noexcept
    : api_(api)
    , map_(init())
    , running_(true)
    , int_(api_.ZeroMQ().PushSocket(zmq::socket::Socket::Direction::Bind))
    , workers_()
    , cbi_(zmq::ListenCallback::Factory([this](auto& in) { callback(in); }))
    , cbe_(zmq::ListenCallback::Factory([this](auto& in) { int_->Send(in); }))
    , ext_(api_.ZeroMQ().PullSocket(cbe_, zmq::socket::Socket::Direction::Bind))
    , init_(false)
    , lock_()
{
}
#endif  // OT_BLOCKCHAIN

Blockchain::Txo::Txo(api::client::internal::Blockchain& parent)
    : parent_(parent)
    , lock_()
{
}

auto Blockchain::BalanceLists::Get(const Chain chain) noexcept
    -> client::blockchain::internal::BalanceList&
{
    Lock lock(lock_);
    auto it = lists_.find(chain);

    if (lists_.end() != it) { return *it->second; }

    auto [it2, added] = lists_.emplace(
        chain, opentxs::Factory::BlockchainBalanceList(parent_, chain));

    OT_ASSERT(added);
    OT_ASSERT(it2->second);

    return *it2->second;
}

#if OT_BLOCKCHAIN
auto Blockchain::BalanceOracle::cb(
    opentxs::network::zeromq::Message& in) noexcept -> void
{
    const auto& header = in.Header();

    if (0 == header.size()) { return; }

    const auto& connectionID = header.at(0);
    const auto& body = in.Body();

    if (0 == body.size()) { return; }

    auto output = opentxs::blockchain::Balance{};
    const auto& chainFrame = body.at(0);
    auto cleanup = Cleanup{[&]() {
        auto message = zmq_.ReplyMessage(in);
        message->AddFrame(chainFrame);
        message->AddFrame(output.first);
        message->AddFrame(output.second);
        socket_->Send(message);
    }};

    const auto chain = chainFrame.as<Chain>();

    if (0 == opentxs::blockchain::SupportedChains().count(chain)) { return; }

    try {
        output = parent_.GetChain(chain).GetBalance();
    } catch (...) {

        return;
    }

    Lock lock(lock_);
    subscribers_[chain].emplace(api_.Factory().Data(connectionID.Bytes()));
}

auto Blockchain::BalanceOracle::UpdateBalance(
    const Chain chain,
    const Balance balance) const noexcept -> void
{
    auto cb = [this, &chain, &balance](const auto& in) {
        auto out = zmq_.Message(in);
        out->AddFrame();
        out->AddFrame(chain);
        out->AddFrame(balance.first);
        out->AddFrame(balance.second);
        socket_->Send(out);
    };
    Lock lock(lock_);
    const auto& subscribers = subscribers_[chain];
    std::for_each(std::begin(subscribers), std::end(subscribers), cb);
}

auto Blockchain::ThreadPoolManager::callback(zmq::Message& in) noexcept -> void
{
    const auto header = in.Header();

    if (2 > header.size()) {
        LogOutput(OT_METHOD)(__FUNCTION__)(": Invalid message").Flush();

        OT_FAIL;
    }

    auto worker = run(header.at(0).as<Chain>());

    if (false == worker.has_value()) { return; }

    switch (header.at(1).as<Work>()) {
        case Work::Wallet: {
            opentxs::blockchain::client::internal::Wallet::ProcessTask(in);
        } break;
        default: {
            OT_FAIL;
        }
    }
}

auto Blockchain::ThreadPoolManager::Endpoint() const noexcept -> std::string
{
    return api_.Endpoints().InternalBlockchainThreadPool();
}

auto Blockchain::ThreadPoolManager::init() noexcept -> NetworkMap
{
    auto output = NetworkMap{};

    for (const auto& chain : opentxs::blockchain::SupportedChains()) {
        auto& [active, running, promise, future, mutex] = output[chain];
        active = true;
        future = promise.get_future();
    }

    return output;
}

auto Blockchain::ThreadPoolManager::Reset(const Chain chain) const noexcept
    -> void
{
    if (false == running_.load()) { return; }

    startup();
    auto& [active, running, promise, future, mutex] = map_.at(chain);
    Lock lock(mutex);

    if (false == active) {
        try {
            promise.set_value();
        } catch (...) {
        }

        promise = {};
        future = promise.get_future();
        active = true;
    }
}

auto Blockchain::ThreadPoolManager::run(const Chain chain) noexcept
    -> std::optional<Worker>
{
    auto& data = map_.at(chain);
    auto& [active, running, promise, future, mutex] = data;
    Lock lock(mutex);
    auto output = std::optional<Worker>{};

    if (active) { output.emplace(data); }

    return output;
}

auto Blockchain::ThreadPoolManager::Shutdown() noexcept -> void
{
    if (running_.exchange(false)) {
        ext_->Close();
        int_->Close();
        auto futures = std::vector<std::shared_future<void>>{};

        for (auto& [chain, data] : map_) { futures.emplace_back(Stop(chain)); }

        for (auto& future : futures) { future.get(); }
    }
}

auto Blockchain::ThreadPoolManager::startup() const noexcept -> void
{
    Lock lock(lock_);

    if (init_) { return; }

    const auto target = std::thread::hardware_concurrency();
    workers_.reserve(target);
    const auto random = Identifier::Random();
    const auto endpoint = std::string{"inproc://"} + random->str();
    auto zmq = int_->Start(endpoint);

    OT_ASSERT(zmq);

    for (unsigned int i{0}; i < target; ++i) {
        auto& worker = workers_.emplace_back(api_.ZeroMQ().PullSocket(
            cbi_, zmq::socket::Socket::Direction::Connect));
        auto zmq = worker->Start(endpoint);

        OT_ASSERT(zmq);

        LogTrace("Started blockchain worker thread ")(i).Flush();
    }

    zmq = ext_->Start(Endpoint());

    OT_ASSERT(zmq);

    init_ = true;
}

auto Blockchain::ThreadPoolManager::Stop(const Chain chain) const noexcept
    -> Future
{
    try {
        auto& [active, running, promise, future, mutex] = map_.at(chain);
        Lock lock(mutex);
        active = false;

        if (0 == running) {
            try {
                promise.set_value();
            } catch (...) {
            }
        }

        return future;
    } catch (...) {
        auto promise = std::promise<void>{};
        promise.set_value();

        return promise.get_future();
    }
}
#endif  // OT_BLOCKCHAIN

auto Blockchain::Txo::AddSpent(
    const identifier::Nym& nym,
    const blockchain::Coin txo,
    const std::string txid) const noexcept -> bool
{
    Lock lock(lock_);
    const auto& storage = parent_.API().Storage();
    auto pData = std::make_shared<proto::StorageBlockchainTxo>();
    const auto loaded = storage.Load(nym, txo, pData, true);

    OT_ASSERT(pData);

    auto& data = *pData;

    if (false == loaded) {
        data.set_version(1);
        auto& output = *data.mutable_output();
        output.set_version(1);
        output.set_txid(txo.first);
        output.set_index(txo.second);
    }

    data.set_spent(true);
    data.add_txid(txid);

    return parent_.API().Storage().Store(nym, data);
}

auto Blockchain::Txo::AddUnspent(
    const identifier::Nym& nym,
    const blockchain::Coin txo,
    const std::vector<OTData>& elements) const noexcept -> bool
{
    Lock lock(lock_);
    const auto& storage = parent_.API().Storage();
    auto pData = std::make_shared<proto::StorageBlockchainTxo>();
    const auto loaded = storage.Load(nym, txo, pData, true);

    OT_ASSERT(pData);

    auto& data = *pData;

    if (false == loaded) {
        data.set_version(1);
        data.set_spent(false);
        auto& output = *data.mutable_output();
        output.set_version(1);
        output.set_txid(txo.first);
        output.set_index(txo.second);
    }

    for (const auto& element : elements) { data.add_element(element->str()); }

    return parent_.API().Storage().Store(nym, data);
}

auto Blockchain::Txo::Claim(
    const identifier::Nym& nym,
    const blockchain::Coin txo) const noexcept -> bool
{
    Lock lock(lock_);

    return parent_.API().Storage().RemoveTxo(nym, txo);
}

auto Blockchain::Txo::Lookup(const identifier::Nym& nym, const Data& element)
    const noexcept -> std::vector<Blockchain::Txo::Status>
{
    Lock lock(lock_);
    auto output = std::vector<Status>{};
    const auto& storage = parent_.API().Storage();
    const auto transactions = storage.LookupElement(nym, element);

    for (const auto& coin : transactions) {
        auto pData = std::make_shared<proto::StorageBlockchainTxo>();
        const auto loaded = storage.Load(nym, coin, pData, false);

        OT_ASSERT(pData);

        auto& data = *pData;

        if (loaded) { output.emplace_back(Status{coin, data.spent()}); }
    }

    return output;
}

auto Blockchain::AccountList(const identifier::Nym& nymID, const Chain chain)
    const noexcept -> std::set<OTIdentifier>
{
    if (false == validate_nym(nymID)) { return {}; }

    std::set<OTIdentifier> output;
    auto list =
        api_.Storage().BlockchainAccountList(nymID.str(), Translate(chain));

    for (const auto& accountID : list) {
        output.emplace(Identifier::Factory(accountID));
    }

    return output;
}

auto Blockchain::address_prefix(const Style style, const Chain chain) const
    noexcept(false) -> OTData
{
    return api_.Factory().Data(
        address_prefix_map_.at(address_style_map_.at({style, chain})),
        StringStyle::Hex);
}

auto Blockchain::assign_transactions(
    const blockchain::internal::BalanceElement& element) const noexcept -> bool
{
    auto map = std::
        map<std::string, std::shared_ptr<const proto::BlockchainTransaction>>{};

    for (const auto& txid : element.IncomingTransactions()) {
        auto pTransaction = Transaction(txid);

        if (false == bool(pTransaction)) {
            LogOutput(OT_METHOD)(__FUNCTION__)(
                ": Failed to load transaction (")(txid)(")")
                .Flush();

            return false;
        }

        map.emplace(txid, pTransaction);
    }

    return assign_transactions(element.NymID(), element.Contact(), map);
}

auto Blockchain::assign_transactions(
    const identifier::Nym& nymID,
    const std::set<OTIdentifier> contacts,
    const TransactionMap& transactions) const noexcept -> bool
{
    auto output{true};

    for (const auto& contact : contacts) {
        output &= assign_transactions(nymID, contact, transactions);
    }

    return output;
}

auto Blockchain::assign_transactions(
    const identifier::Nym& nymID,
    const Identifier& contactID,
    const TransactionMap& transactions) const noexcept -> bool
{
    auto thread = std::make_shared<proto::StorageThread>();
    auto threadExists{false};
    const auto threadList = api_.Storage().ThreadList(nymID.str(), false);

    for (const auto& it : threadList) {
        const auto& id = it.first;

        if (id == contactID.str()) { threadExists = true; }
    }

    if (threadExists) {
        thread = activity_.Thread(nymID, contactID);

        if (false == bool(thread)) {
            LogOutput(OT_METHOD)(__FUNCTION__)(": Failed to load thread")
                .Flush();

            return false;
        }
    }

    auto success{true};

    for (const auto& [txid, pTransaction] : transactions) {
        if (false == bool(pTransaction)) {
            LogOutput(OT_METHOD)(__FUNCTION__)(": Invalid transaction.")
                .Flush();

            return false;
        }

        const auto& transaction = *pTransaction;
        auto exists{false};

        if (thread) {
            for (const auto& activity : thread->item()) {
                if (txid.compare(activity.id()) == 0) { exists = true; }
            }
        }

        if (false == exists) {
            success &= activity_.AddBlockchainTransaction(
                nymID, contactID, txid, Clock::from_time_t(transaction.time()));
        } else {
            LogTrace(OT_METHOD)(__FUNCTION__)(": Transaction ")(txid)(
                " already exists in thread")
                .Flush();
        }
    }

    return success;
}

auto Blockchain::AssignContact(
    const identifier::Nym& nymID,
    const Identifier& accountID,
    const blockchain::Subchain subchain,
    const Bip32Index index,
    const Identifier& contactID) const noexcept -> bool
{
    if (false == validate_nym(nymID)) { return false; }

    LOCK_NYM()

    const auto chain = Translate(
        api_.Storage().BlockchainAccountType(nymID.str(), accountID.str()));

    OT_ASSERT(Chain::Unknown != chain);

    try {
        auto& node = balance_lists_.Get(chain).Nym(nymID).Node(accountID);

        try {
            const auto& element = node.BalanceElement(subchain, index);
            const auto existing = element.Contact();

            if (contactID == existing) { return true; }

            if (false == node.SetContact(subchain, index, contactID)) {

                return false;
            }

            if (existing->empty()) {

                return assign_transactions(element);
            } else {

                return move_transactions(element, existing);
            }
        } catch (...) {
            LogOutput(OT_METHOD)(__FUNCTION__)(
                ": Failed to load balance element")
                .Flush();

            return false;
        }
    } catch (...) {
        LogOutput(OT_METHOD)(__FUNCTION__)(": Failed to load account").Flush();

        return false;
    }
}

auto Blockchain::AssignLabel(
    const identifier::Nym& nymID,
    const Identifier& accountID,
    const blockchain::Subchain subchain,
    const Bip32Index index,
    const std::string& label) const noexcept -> bool
{
    if (false == validate_nym(nymID)) { return false; }

    LOCK_NYM()

    const auto chain = Translate(
        api_.Storage().BlockchainAccountType(nymID.str(), accountID.str()));

    OT_ASSERT(Chain::Unknown != chain);

    try {
        auto& node = balance_lists_.Get(chain).Nym(nymID).Node(accountID);

        try {
            const auto& element = node.BalanceElement(subchain, index);

            if (label == element.Label()) { return true; }

            return node.SetLabel(subchain, index, label);
        } catch (...) {
            LogOutput(OT_METHOD)(__FUNCTION__)(
                ": Failed to load balance element")
                .Flush();

            return false;
        }
    } catch (...) {
        LogOutput(OT_METHOD)(__FUNCTION__)(": Failed to load account").Flush();

        return false;
    }
}

auto Blockchain::BalanceTree(const identifier::Nym& nymID, const Chain chain)
    const noexcept(false) -> const blockchain::internal::BalanceTree&
{
    if (false == validate_nym(nymID)) {
        throw std::runtime_error("Invalid nym");
    }

    if (Chain::Unknown == chain) { throw std::runtime_error("Invalid chain"); }

    auto& balanceList = balance_lists_.Get(chain);

    return balanceList.Nym(nymID);
}

auto Blockchain::bip44_type(const proto::ContactItemType type) const noexcept
    -> Bip44Type
{
    switch (type) {
        case proto::CITEMTYPE_BTC: {

            return Bip44Type::BITCOIN;
        }
        case proto::CITEMTYPE_LTC: {

            return Bip44Type::LITECOIN;
        }
        case proto::CITEMTYPE_DOGE: {

            return Bip44Type::DOGECOIN;
        }
        case proto::CITEMTYPE_DASH: {

            return Bip44Type::DASH;
        }
        case proto::CITEMTYPE_BCH: {

            return Bip44Type::BITCOINCASH;
        }
        case proto::CITEMTYPE_TNBCH:
        case proto::CITEMTYPE_TNBTC:
        case proto::CITEMTYPE_TNXRP:
        case proto::CITEMTYPE_TNLTC:
        case proto::CITEMTYPE_TNXEM:
        case proto::CITEMTYPE_TNDASH:
        case proto::CITEMTYPE_TNMAID:
        case proto::CITEMTYPE_TNLSK:
        case proto::CITEMTYPE_TNDOGE:
        case proto::CITEMTYPE_TNXMR:
        case proto::CITEMTYPE_TNWAVES:
        case proto::CITEMTYPE_TNNXT:
        case proto::CITEMTYPE_TNSC:
        case proto::CITEMTYPE_TNSTEEM: {
            return Bip44Type::TESTNET;
        }
        default: {
            OT_FAIL;
        }
    }
}

auto Blockchain::CalculateAddress(
    const Chain chain,
    const Style format,
    const Data& pubkey) const noexcept -> std::string
{
    auto data = api_.Factory().Data();

    switch (format) {
        case Style::P2PKH: {
            try {
                data = PubkeyHash(chain, pubkey);
            } catch (...) {
                LogOutput(OT_METHOD)(__FUNCTION__)(": Invalid public key.")
                    .Flush();

                return {};
            }
        } break;
        default: {
            LogOutput(OT_METHOD)(__FUNCTION__)(": Unsupported address style (")(
                static_cast<std::uint16_t>(format))(")")
                .Flush();

            return {};
        }
    }

    return EncodeAddress(format, chain, data);
}

auto Blockchain::DecodeAddress(const std::string& encoded) const noexcept
    -> std::tuple<OTData, Blockchain::Style, Blockchain::Chain>
{
    auto output = std::tuple<OTData, Style, Chain>{
        api_.Factory().Data(), Style::Unknown, Chain::Unknown};
    auto& [data, style, chain] = output;
    const auto bytes = api_.Factory().Data(
        api_.Crypto().Encode().IdentifierDecode(encoded), StringStyle::Raw);
    auto type = api_.Factory().Data();

    switch (bytes->size()) {
        case 21: {
            bytes->Extract(1, type, 0);
            auto prefix{Prefix::Unknown};

            try {
                prefix = address_prefix_reverse_map_.at(type->asHex());
            } catch (...) {
                LogOutput(OT_METHOD)(__FUNCTION__)(
                    ": Unable to decode version byte (")(type->asHex())
                    .Flush();

                return output;
            }

            const auto [decodeStyle, decodeChain] =
                address_style_reverse_map_.at(prefix);
            style = decodeStyle;
            chain = decodeChain;
            bytes->Extract(20, data, 1);
        } break;
        default: {
            LogOutput(OT_METHOD)(__FUNCTION__)(": Unable to decode address (")(
                bytes->asHex())
                .Flush();
        }
    }

    return output;
}

auto Blockchain::EncodeAddress(
    const Style style,
    const Chain chain,
    const Data& data) const noexcept -> std::string
{
    switch (style) {
        case Style::P2PKH: {

            return p2pkh(chain, data);
        }
        case Style::P2SH: {

            return p2sh(chain, data);
        }
        default: {
            LogOutput(OT_METHOD)(__FUNCTION__)(": Unsupported address style (")(
                static_cast<std::uint16_t>(style))(")")
                .Flush();

            return {};
        }
    }
}

#if OT_BLOCKCHAIN
auto Blockchain::GetChain(const Chain type) const noexcept(false)
    -> const opentxs::blockchain::Network&
{
    Lock lock(lock_);

    return *networks_.at(type);
}
#endif  // OT_BLOCKCHAIN

auto Blockchain::HDSubaccount(
    const identifier::Nym& nymID,
    const Identifier& accountID) const noexcept(false) -> const blockchain::HD&
{
    const auto type =
        api_.Storage().BlockchainAccountType(nymID.str(), accountID.str());

    if (proto::CITEMTYPE_ERROR == type) {
        throw std::out_of_range("Account does not exist");
    }

    auto& balanceList = balance_lists_.Get(Translate(type));
    auto& nym = balanceList.Nym(nymID);

    return nym.HDChain(accountID);
}

auto Blockchain::init_path(
    const std::string& root,
    const proto::ContactItemType chain,
    const Bip32Index account,
    const BlockchainAccountType standard,
    proto::HDPath& path) const noexcept -> void
{
    path.set_version(PATH_VERSION);
    path.set_root(root);

    switch (standard) {
        case BlockchainAccountType::BIP32: {
            path.add_child(HDIndex{account, Bip32Child::HARDENED});
        } break;
        case BlockchainAccountType::BIP44: {
            path.add_child(
                HDIndex{Bip43Purpose::HDWALLET, Bip32Child::HARDENED});
            path.add_child(HDIndex{bip44_type(chain), Bip32Child::HARDENED});
            path.add_child(account);
        } break;
        default: {
            OT_FAIL;
        }
    }
}

auto Blockchain::move_transactions(
    const blockchain::internal::BalanceElement& element,
    const Identifier& fromContact) const noexcept -> bool
{
    OT_ASSERT(false == fromContact.empty());

    bool output{true};
    const auto toContact = element.Contact();

    if (toContact->empty()) {
        for (const auto& txid : element.IncomingTransactions()) {
            output &= activity_.UnassignBlockchainTransaction(
                element.NymID(), fromContact, txid);
        }

        return output;
    }

    auto thread = std::make_shared<proto::StorageThread>();
    const auto exists =
        api_.Storage().Load(element.NymID().str(), toContact->str(), thread);

    if (false == exists) {
        const auto created = api_.Storage().CreateThread(
            element.NymID().str(), toContact->str(), {toContact->str()});

        if (false == created) {
            LogOutput(OT_METHOD)(__FUNCTION__)(": Failed to create thread (")(
                toContact)(")")
                .Flush();

            return false;
        }
    }

    for (const auto& txid : element.IncomingTransactions()) {
        output &= activity_.MoveIncomingBlockchainTransaction(
            element.NymID(), fromContact, toContact, txid);
    }

    return output;
}

auto Blockchain::NewHDSubaccount(
    const identifier::Nym& nymID,
    const BlockchainAccountType standard,
    const Chain chain,
    const PasswordPrompt& reason) const noexcept -> OTIdentifier
{
    if (false == validate_nym(nymID)) { return Identifier::Factory(); }

    if (Chain::Unknown == chain) {
        LogOutput(OT_METHOD)(__FUNCTION__)(": Invalid chain").Flush();

        return Identifier::Factory();
    }

    auto nym = api_.Wallet().Nym(nymID);

    if (false == bool(nym)) {
        LogOutput(OT_METHOD)(__FUNCTION__)(": Nym does not exist.").Flush();

        return Identifier::Factory();
    }

    proto::HDPath nymPath{};

    if (false == nym->Path(nymPath)) {
        LogOutput(OT_METHOD)(__FUNCTION__)(": No nym path.").Flush();

        return Identifier::Factory();
    }

    if (0 == nymPath.root().size()) {
        LogOutput(OT_METHOD)(__FUNCTION__)(": Missing root.").Flush();

        return Identifier::Factory();
    }

    if (2 > nymPath.child().size()) {
        LogOutput(OT_METHOD)(__FUNCTION__)(": Invalid path.").Flush();

        return Identifier::Factory();
    }

    proto::HDPath accountPath{};
    init_path(
        nymPath.root(),
        Translate(chain),
        HDIndex{nymPath.child(1), Bip32Child::HARDENED},
        standard,
        accountPath);

    try {
        auto accountID = Identifier::Factory();
        auto& tree = balance_lists_.Get(chain).Nym(nymID);
        tree.AddHDNode(accountPath, accountID);
#if OT_BLOCKCHAIN
        balances_.UpdateBalance(chain, {});
#endif  // OT_BLOCKCHAIN

        return accountID;
    } catch (...) {
        LogVerbose(OT_METHOD)(__FUNCTION__)(": Failed to create account")
            .Flush();

        return Identifier::Factory();
    }
}

auto Blockchain::p2pkh(const Chain chain, const Data& pubkeyHash) const noexcept
    -> std::string
{
    try {
        auto preimage = address_prefix(Style::P2PKH, chain);

        OT_ASSERT(1 == preimage->size());

        preimage += pubkeyHash;

        OT_ASSERT(21 == preimage->size());

        return api_.Crypto().Encode().IdentifierEncode(preimage);
    } catch (...) {
        LogOutput(OT_METHOD)(__FUNCTION__)(": Unsupported chain (")(
            static_cast<std::uint32_t>(chain))(")")
            .Flush();

        return "";
    }
}

auto Blockchain::p2sh(const Chain chain, const Data& pubkeyHash) const noexcept
    -> std::string
{
    try {
        auto preimage = address_prefix(Style::P2SH, chain);

        OT_ASSERT(1 == preimage->size());

        preimage += pubkeyHash;

        OT_ASSERT(21 == preimage->size());

        return api_.Crypto().Encode().IdentifierEncode(preimage);
    } catch (...) {
        LogOutput(OT_METHOD)(__FUNCTION__)(": Unsupported chain (")(
            static_cast<std::uint32_t>(chain))(")")
            .Flush();

        return "";
    }
}

auto Blockchain::parse_transaction(
    const identifier::Nym& nym,
    const proto::BlockchainTransaction& tx,
    const blockchain::internal::BalanceTree& tree,
    std::set<OTIdentifier>& contacts) const noexcept
    -> Blockchain::ParsedTransaction
{
    auto output = ParsedTransaction{};
    auto& [unspent, spent] = output;
    const auto& txid = tx.txid();

    for (const auto& input : tx.input()) {
        const auto& previous = input.previous();
        const auto coin = blockchain::Coin{previous.txid(), previous.index()};
        const auto lookup = tree.LookupUTXO(coin);

        if (lookup) {
            const auto& [key, value] = lookup.value();
            spent.emplace_back(blockchain::Activity{coin, key, value});
        } else {
            txo_db_.AddSpent(nym, coin, txid);
        }
    }

    for (const auto& txout : tx.output()) {
        const auto& sKey = txout.key();
        const auto coin = blockchain::Coin{txid, txout.index()};

        if (txout.has_key()) {
            const auto key = blockchain::Key{
                sKey.subaccount(),
                static_cast<blockchain::Subchain>(sKey.subchain()),
                sKey.index()};
            unspent.emplace_back(
                blockchain::Activity{coin, key, txout.value()});
        } else {
            const auto& external = txout.external();

            switch (external.type()) {
                case proto::BTOUTPUT_P2PK:
                case proto::BTOUTPUT_P2PKH:
                case proto::BTOUTPUT_P2SH:
                case proto::BTOUTPUT_P2WPKH:
                case proto::BTOUTPUT_P2WSH: {
                    OT_ASSERT(1 == external.data_size());

                    const auto& bytes = external.data(0);
                    const auto hash =
                        api_.Factory().Data(bytes, StringStyle::Raw);
                    auto contact = api_.Factory().Identifier(
                        api_.Storage().BlockchainAddressOwner(
                            tx.chain(), hash->asHex()));

                    if (false == contact->empty()) {
                        LogTrace(OT_METHOD)(__FUNCTION__)(": Contact ")(
                            contact)(" owns item (0x")(hash->asHex())(")")
                            .Flush();
                        contacts.emplace(std::move(contact));

                        break;
                    }

                    LogTrace(OT_METHOD)(__FUNCTION__)(
                        ": No contact associated with item (0x")(hash->asHex())(
                        ")")
                        .Flush();
                    [[fallthrough]];
                }
                default: {
                    auto elements = std::vector<OTData>{};

                    for (const auto& bytes : external.data()) {
                        elements.emplace_back(
                            api_.Factory().Data(bytes, StringStyle::Raw));
                    }

                    txo_db_.AddUnspent(nym, coin, elements);
                }
            }
        }
    }

    return output;
}

auto Blockchain::PubkeyHash(
    [[maybe_unused]] const Chain chain,
    const Data& pubkey) const noexcept(false) -> OTData
{
    if (pubkey.empty()) { throw std::runtime_error("Empty pubkey"); }

    if (COMPRESSED_PUBKEY_SIZE != pubkey.size()) {
        throw std::runtime_error("Incorrect pubkey size");
    }

    auto output = Data::Factory();

    if (false ==
        api_.Crypto().Hash().Digest(
            proto::HASHTYPE_BITCOIN, pubkey.Bytes(), output->WriteInto())) {
        throw std::runtime_error("Unable to calculate hash.");
    }

    return output;
}

#if OT_BLOCKCHAIN
auto Blockchain::Start(const Chain type, const std::string& seednode) const
    noexcept -> bool
{
    if (0 == opentxs::blockchain::SupportedChains().count(type)) {
        LogOutput(OT_METHOD)(__FUNCTION__)(": Unsupported chain").Flush();

        return false;
    }

    Lock lock(lock_);

    if (0 != networks_.count(type)) {
        LogOutput(OT_METHOD)(__FUNCTION__)(": Chain already running").Flush();

        return true;
    }

    switch (type) {
        case Chain::Bitcoin:
        case Chain::Bitcoin_testnet3:
        case Chain::BitcoinCash:
        case Chain::BitcoinCash_testnet3: {
            thread_pool_.Reset(type);
            auto [it, added] = networks_.emplace(
                type,
                opentxs::factory::BlockchainNetworkBitcoin(
                    api_, *this, type, seednode, ""));

            return it->second->Connect();
        }
        default: {
        }
    }

    return false;
}

auto Blockchain::Stop(const Chain type) const noexcept -> bool
{
    thread_pool_.Stop(type).get();
    Lock lock(lock_);
    auto it = networks_.find(type);

    if (networks_.end() == it) { return false; }

    OT_ASSERT(it->second);

    it->second->Shutdown();

    networks_.erase(it);

    return true;
}
#endif  // OT_BLOCKCHAIN

auto Blockchain::StoreTransaction(
    const identifier::Nym& nymID,
    const Chain chain,
    const proto::BlockchainTransaction& transaction,
    const PasswordPrompt& reason) const noexcept -> bool
{
    if (false == validate_nym(nymID)) { return false; }

    LOCK_NYM()

    auto contacts = std::set<OTIdentifier>{};
    const auto& tree = balance_lists_.Get(chain).Nym(nymID);
    const auto parsed = parse_transaction(nymID, transaction, tree, contacts);
    const auto& [unspent, spent] = parsed;
    const auto associated =
        tree.AssociateTransaction(unspent, spent, contacts, reason);

    if (false == associated) {
        LogOutput(OT_METHOD)(__FUNCTION__)(": Invalid transaction.").Flush();

        return false;
    }

    const auto success = assign_transactions(
        nymID,
        contacts,
        {{transaction.txid(),
          std::make_shared<const proto::BlockchainTransaction>(transaction)}});

    return success && api_.Storage().Store(nymID, transaction);
}

auto Blockchain::Transaction(const std::string& txid) const noexcept
    -> std::shared_ptr<proto::BlockchainTransaction>
{
    auto output = std::shared_ptr<proto::BlockchainTransaction>{};

    if (false == api_.Storage().Load(txid, output, false)) {
        LogOutput(OT_METHOD)(__FUNCTION__)(": Failed to load transaction.")
            .Flush();
    }

    return output;
}

auto Blockchain::update_transactions(
    const Lock& lock,
    const identifier::Nym& nym,
    const TransactionContactMap& transactions) const noexcept -> bool
{
    Lock nymLock(nym_lock_[nym]);
    auto output{true};

    for (const auto& it : transactions) {
        const auto& txid = it.first;
        const auto contacts = it.second;
        const auto pTransaction = Transaction(txid);

        if (false == bool(pTransaction)) {
            LogOutput(OT_METHOD)(__FUNCTION__)(": Failed to load transaction.")
                .Flush();

            return false;
        }

        output &= assign_transactions(nym, contacts, {{txid, pTransaction}});
    }

    return output;
}

auto Blockchain::UpdateTransactions(
    const std::map<OTData, OTIdentifier>& changed) const noexcept -> bool
{
    Lock lock(lock_);

    auto transactions = NymTransactionMap{};

    for (const auto& [bytes, contactID] : changed) {
        for (const auto& nym : api_.Wallet().LocalNyms()) {
            const auto coins = txo_db_.Lookup(nym, bytes);

            for (const auto& coin : coins) {
                const auto& txid = coin.first.first;
                transactions[nym][txid].emplace(contactID);
            }
        }
    }

    auto output{true};

    for (const auto& [nymID, map] : transactions) {
        output &= update_transactions(lock, nymID, map);
    }

    return output;
}

auto Blockchain::validate_nym(const identifier::Nym& nymID) const noexcept
    -> bool
{
    if (false == nymID.empty()) {
        if (0 < api_.Wallet().LocalNyms().count(nymID)) { return true; }
    }

    return false;
}

#if OT_BLOCKCHAIN
Blockchain::ThreadPoolManager::~ThreadPoolManager()
{
    if (running_.load()) { Shutdown(); }
}
#endif  // OT_BLOCKCHAIN

Blockchain::~Blockchain()
{
#if OT_BLOCKCHAIN
    LogVerbose("Shutting down ")(networks_.size())(" blockchain clients")
        .Flush();
    thread_pool_.Shutdown();

    for (auto& [chain, network] : networks_) { network->Shutdown().get(); }

    networks_.clear();
    io_.Shutdown();
#endif  // OT_BLOCKCHAIN
}
}  // namespace opentxs::api::client::implementation
