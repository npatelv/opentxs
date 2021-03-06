// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <chrono>
#include <cstdint>
#include <ctime>
#include <iosfwd>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <shared_mutex>
#include <string>
#include <tuple>
#include <utility>

#include "internal/consensus/Consensus.hpp"
#include "internal/identity/Identity.hpp"
#include "opentxs/Proto.hpp"
#include "opentxs/Types.hpp"
#include "opentxs/Version.hpp"
#include "opentxs/api/Editor.hpp"
#include "opentxs/api/Wallet.hpp"
#include "opentxs/client/NymData.hpp"
#include "opentxs/core/Account.hpp"
#include "opentxs/core/Identifier.hpp"
#include "opentxs/core/Lockable.hpp"
#include "opentxs/core/contract/ServerContract.hpp"
#include "opentxs/core/contract/UnitDefinition.hpp"
#include "opentxs/core/contract/basket/BasketContract.hpp"
#include "opentxs/core/identifier/Nym.hpp"
#include "opentxs/core/identifier/Server.hpp"
#include "opentxs/core/identifier/UnitDefinition.hpp"
#include "opentxs/network/zeromq/socket/Publish.hpp"
#include "opentxs/network/zeromq/socket/Push.hpp"
#include "opentxs/network/zeromq/socket/Request.hpp"
#include "opentxs/network/zeromq/socket/Request.tpp"
#include "opentxs/network/zeromq/socket/Sender.tpp"
#include "opentxs/protobuf/CashEnums.pb.h"
#include "opentxs/protobuf/ContactEnums.pb.h"

namespace opentxs
{
namespace api
{
namespace client
{
class Issuer;
}  // namespace client

namespace internal
{
struct Core;
}  // namespace internal
}  // namespace api

namespace blind
{
class Purse;
}  // namespace blind

namespace identity
{
namespace internal
{
struct Nym;
}  // namespace internal

class Nym;
}  // namespace identity

namespace internal
{
struct Context;
}  // namespace internal

namespace proto
{
class Context;
class Credential;
class Nym;
class PeerReply;
class PeerRequest;
class ServerContract;
class UnitDefinition;
}  // namespace proto

class ClientContext;
class Context;
class Factory;
class NymFile;
class NymParameters;
class PasswordPrompt;
class PeerObject;
class ServerContext;
class String;
}  // namespace opentxs

namespace opentxs::api::implementation
{
class Wallet : virtual public opentxs::api::Wallet, public Lockable
{
public:
    SharedAccount Account(const Identifier& accountID) const final;
    OTIdentifier AccountPartialMatch(const std::string& hint) const final;
    ExclusiveAccount CreateAccount(
        const identifier::Nym& ownerNymID,
        const identifier::Server& notaryID,
        const identifier::UnitDefinition& instrumentDefinitionID,
        const identity::Nym& signer,
        Account::AccountType type,
        TransactionNumber stash,
        const PasswordPrompt& reason) const final;
    bool DeleteAccount(const Identifier& accountID) const final;
    SharedAccount IssuerAccount(
        const identifier::UnitDefinition& unitID) const final;
    ExclusiveAccount mutable_Account(
        const Identifier& accountID,
        const PasswordPrompt& reason,
        const AccountCallback callback) const final;
    bool UpdateAccount(
        const Identifier& accountID,
        const opentxs::ServerContext& context,
        const String& serialized,
        const PasswordPrompt& reason) const final;
    bool UpdateAccount(
        const Identifier& accountID,
        const opentxs::ServerContext& context,
        const String& serialized,
        const std::string& label,
        const PasswordPrompt& reason) const final;
    bool ImportAccount(std::unique_ptr<opentxs::Account>& imported) const final;
    std::shared_ptr<const opentxs::ClientContext> ClientContext(
        const identifier::Nym& remoteNymID) const override;
    std::shared_ptr<const opentxs::ServerContext> ServerContext(
        const identifier::Nym& localNymID,
        const Identifier& remoteID) const override;
    Editor<opentxs::ClientContext> mutable_ClientContext(
        const identifier::Nym& remoteNymID,
        const PasswordPrompt& reason) const override;
    Editor<opentxs::ServerContext> mutable_ServerContext(
        const identifier::Nym& localNymID,
        const Identifier& remoteID,
        const PasswordPrompt& reason) const override;
    std::set<OTNymID> IssuerList(const identifier::Nym& nymID) const final;
    std::shared_ptr<const api::client::Issuer> Issuer(
        const identifier::Nym& nymID,
        const identifier::Nym& issuerID) const final;
    Editor<api::client::Issuer> mutable_Issuer(
        const identifier::Nym& nymID,
        const identifier::Nym& issuerID) const final;
    bool IsLocalNym(const std::string& id) const final;
    std::size_t LocalNymCount() const final;
    std::set<OTNymID> LocalNyms() const final;
    Nym_p Nym(
        const identifier::Nym& id,
        const std::chrono::milliseconds& timeout =
            std::chrono::milliseconds(0)) const final;
    Nym_p Nym(const proto::Nym& nym) const final;
    Nym_p Nym(
        const PasswordPrompt& reason,
        const std::string name,
        const NymParameters& parameters,
        const proto::ContactItemType type) const final;
    NymData mutable_Nym(const identifier::Nym& id, const PasswordPrompt& reason)
        const final;
    std::unique_ptr<const opentxs::NymFile> Nymfile(
        const identifier::Nym& id,
        const PasswordPrompt& reason) const final;
    Editor<opentxs::NymFile> mutable_Nymfile(
        const identifier::Nym& id,
        const PasswordPrompt& reason) const final;
    Nym_p NymByIDPartialMatch(const std::string& partialId) const final;
    ObjectList NymList() const final;
    bool NymNameByIndex(const std::size_t index, String& name) const final;
    std::shared_ptr<proto::PeerReply> PeerReply(
        const identifier::Nym& nym,
        const Identifier& reply,
        const StorageBox& box) const final;
    bool PeerReplyComplete(
        const identifier::Nym& nym,
        const Identifier& replyOrRequest) const final;
    bool PeerReplyCreate(
        const identifier::Nym& nym,
        const proto::PeerRequest& request,
        const proto::PeerReply& reply) const final;
    bool PeerReplyCreateRollback(
        const identifier::Nym& nym,
        const Identifier& request,
        const Identifier& reply) const final;
    ObjectList PeerReplySent(const identifier::Nym& nym) const final;
    ObjectList PeerReplyIncoming(const identifier::Nym& nym) const final;
    ObjectList PeerReplyFinished(const identifier::Nym& nym) const final;
    ObjectList PeerReplyProcessed(const identifier::Nym& nym) const final;
    bool PeerReplyReceive(const identifier::Nym& nym, const PeerObject& reply)
        const final;
    std::shared_ptr<proto::PeerRequest> PeerRequest(
        const identifier::Nym& nym,
        const Identifier& request,
        const StorageBox& box,
        std::time_t& time) const final;
    bool PeerRequestComplete(
        const identifier::Nym& nym,
        const Identifier& reply) const final;
    bool PeerRequestCreate(
        const identifier::Nym& nym,
        const proto::PeerRequest& request) const final;
    bool PeerRequestCreateRollback(
        const identifier::Nym& nym,
        const Identifier& request) const final;
    bool PeerRequestDelete(
        const identifier::Nym& nym,
        const Identifier& request,
        const StorageBox& box) const final;
    ObjectList PeerRequestSent(const identifier::Nym& nym) const final;
    ObjectList PeerRequestIncoming(const identifier::Nym& nym) const final;
    ObjectList PeerRequestFinished(const identifier::Nym& nym) const final;
    ObjectList PeerRequestProcessed(const identifier::Nym& nym) const final;
    bool PeerRequestReceive(
        const identifier::Nym& nym,
        const PeerObject& request) const final;
    bool PeerRequestUpdate(
        const identifier::Nym& nym,
        const Identifier& request,
        const StorageBox& box) const final;
#if OT_CASH
    std::unique_ptr<const blind::Purse> Purse(
        const identifier::Nym& nym,
        const identifier::Server& server,
        const identifier::UnitDefinition& unit,
        const bool checking) const final;
    Editor<blind::Purse> mutable_Purse(
        const identifier::Nym& nym,
        const identifier::Server& server,
        const identifier::UnitDefinition& unit,
        const PasswordPrompt& reason,
        const proto::CashType type) const final;
#endif
    bool RemoveServer(const identifier::Server& id) const final;
    bool RemoveUnitDefinition(const identifier::UnitDefinition& id) const final;
    OTServerContract Server(
        const identifier::Server& id,
        const std::chrono::milliseconds& timeout =
            std::chrono::milliseconds(0)) const final;
    OTServerContract Server(const proto::ServerContract& contract) const final;
    OTServerContract Server(
        const std::string& nymid,
        const std::string& name,
        const std::string& terms,
        const std::list<contract::Server::Endpoint>& endpoints,
        const PasswordPrompt& reason,
        const VersionNumber version) const final;
    ObjectList ServerList() const final;
    bool SetNymAlias(const identifier::Nym& id, const std::string& alias)
        const final;
    bool SetServerAlias(const identifier::Server& id, const std::string& alias)
        const final;
    bool SetUnitDefinitionAlias(
        const identifier::UnitDefinition& id,
        const std::string& alias) const final;
    ObjectList UnitDefinitionList() const final;
    OTUnitDefinition UnitDefinition(
        const identifier::UnitDefinition& id,
        const std::chrono::milliseconds& timeout =
            std::chrono::milliseconds(0)) const final;
    OTBasketContract BasketContract(
        const identifier::UnitDefinition& id,
        const std::chrono::milliseconds& timeout =
            std::chrono::milliseconds(0)) const noexcept(false) final;
    OTUnitDefinition UnitDefinition(
        const proto::UnitDefinition& contract) const final;
    OTUnitDefinition UnitDefinition(
        const std::string& nymid,
        const std::string& shortname,
        const std::string& name,
        const std::string& symbol,
        const std::string& terms,
        const std::string& tla,
        const std::uint32_t power,
        const std::string& fraction,
        const proto::ContactItemType unitOfAccount,
        const PasswordPrompt& reason,
        const VersionNumber version =
            contract::Unit::DefaultVersion) const final;
    OTUnitDefinition UnitDefinition(
        const std::string& nymid,
        const std::string& shortname,
        const std::string& name,
        const std::string& symbol,
        const std::string& terms,
        const proto::ContactItemType unitOfAccount,
        const PasswordPrompt& reason,
        const VersionNumber version =
            contract::Unit::DefaultVersion) const final;
    proto::ContactItemType CurrencyTypeBasedOnUnitType(
        const identifier::UnitDefinition& contractID) const final;

    bool LoadCredential(
        const std::string& id,
        std::shared_ptr<proto::Credential>& credential) const final;
    bool SaveCredential(const proto::Credential& credential) const final;

    ~Wallet() override = default;

protected:
    using AccountLock =
        std::pair<std::shared_mutex, std::unique_ptr<opentxs::Account>>;
    using ContextID = std::pair<std::string, std::string>;
    using ContextMap =
        std::map<ContextID, std::shared_ptr<opentxs::internal::Context>>;

    const api::internal::Core& api_;
    mutable ContextMap context_map_;
    mutable std::mutex context_map_lock_;

    std::shared_ptr<opentxs::Context> context(
        const identifier::Nym& localNymID,
        const identifier::Nym& remoteNymID) const;
    proto::ContactItemType extract_unit(
        const identifier::UnitDefinition& contractID) const;
    proto::ContactItemType extract_unit(const contract::Unit& contract) const;
    void save(const PasswordPrompt& reason, opentxs::internal::Context* context)
        const;
    OTNymID server_to_nym(Identifier& nymOrNotaryID) const;

    Wallet(const api::internal::Core& core);

private:
    using AccountMap = std::map<OTIdentifier, AccountLock>;
    using NymLock =
        std::pair<std::mutex, std::shared_ptr<identity::internal::Nym>>;
    using NymMap = std::map<std::string, NymLock>;
    using ServerMap = std::map<std::string, std::shared_ptr<contract::Server>>;
    using UnitMap = std::map<std::string, std::shared_ptr<contract::Unit>>;
    using IssuerID = std::pair<OTIdentifier, OTIdentifier>;
    using IssuerLock =
        std::pair<std::mutex, std::shared_ptr<api::client::Issuer>>;
    using IssuerMap = std::map<IssuerID, IssuerLock>;
    using PurseID = std::tuple<OTNymID, OTServerID, OTUnitID>;
    using UnitNameMap = std::map<std::string, proto::ContactItemType>;
    using UnitNameReverse = std::map<proto::ContactItemType, std::string>;

    friend opentxs::Factory;

    static const UnitNameMap unit_of_account_;
    static const UnitNameReverse unit_lookup_;

    mutable AccountMap account_map_;
    mutable NymMap nym_map_;
    mutable ServerMap server_map_;
    mutable UnitMap unit_map_;
    mutable IssuerMap issuer_map_;
    mutable std::mutex account_map_lock_;
    mutable std::mutex nym_map_lock_;
    mutable std::mutex server_map_lock_;
    mutable std::mutex unit_map_lock_;
    mutable std::mutex issuer_map_lock_;
    mutable std::mutex peer_map_lock_;
    mutable std::map<std::string, std::mutex> peer_lock_;
    mutable std::mutex nymfile_map_lock_;
    mutable std::map<OTIdentifier, std::mutex> nymfile_lock_;
#if OT_CASH
    mutable std::mutex purse_lock_;
    mutable std::map<PurseID, std::mutex> purse_id_lock_;
#endif
    OTZMQPublishSocket account_publisher_;
    OTZMQPublishSocket issuer_publisher_;
    OTZMQPublishSocket nym_publisher_;
    OTZMQPublishSocket nym_created_publisher_;
    OTZMQPublishSocket server_publisher_;
    OTZMQPublishSocket peer_reply_publisher_;
    OTZMQPublishSocket peer_request_publisher_;
    OTZMQRequestSocket dht_nym_requester_;
    OTZMQRequestSocket dht_server_requester_;
    OTZMQRequestSocket dht_unit_requester_;
    OTZMQPushSocket find_nym_;

    static UnitNameReverse reverse_unit_map(const UnitNameMap& map);

    std::string account_alias(
        const std::string& accountID,
        const std::string& hint) const;
    opentxs::Account* account_factory(
        const Identifier& accountID,
        const std::string& alias,
        const std::string& serialized) const;
#if OT_CASH
    std::mutex& get_purse_lock(
        const identifier::Nym& nym,
        const identifier::Server& server,
        const identifier::UnitDefinition& unit) const;
#endif
    virtual void instantiate_client_context(
        const proto::Context& serialized,
        const Nym_p& localNym,
        const Nym_p& remoteNym,
        std::shared_ptr<opentxs::internal::Context>& output) const
    {
    }
    virtual void instantiate_server_context(
        const proto::Context& serialized,
        const Nym_p& localNym,
        const Nym_p& remoteNym,
        std::shared_ptr<opentxs::internal::Context>& output) const
    {
    }
    virtual bool load_legacy_account(
        const Identifier& accountID,
        const eLock& lock,
        AccountLock& row) const
    {
        return false;
    }
    Editor<opentxs::NymFile> mutable_nymfile(
        const Nym_p& targetNym,
        const Nym_p& signerNym,
        const identifier::Nym& id,
        const PasswordPrompt& reason) const;
    virtual void nym_to_contact(
        [[maybe_unused]] const identity::Nym& nym,
        [[maybe_unused]] const std::string& name) const noexcept
    {
    }
    std::mutex& nymfile_lock(const identifier::Nym& nymID) const;
    std::mutex& peer_lock(const std::string& nymID) const;
    void publish_server(const identifier::Server& id) const;
#if OT_CASH
    std::unique_ptr<blind::Purse> purse(
        const identifier::Nym& nym,
        const identifier::Server& server,
        const identifier::UnitDefinition& unit,
        const bool checking) const;
#endif
    void save(
        const PasswordPrompt& reason,
        const std::string id,
        std::unique_ptr<opentxs::Account>& in,
        eLock& lock,
        bool success) const;
    void save(const Lock& lock, api::client::Issuer* in) const;
#if OT_CASH
    void save(const Lock& lock, const OTNymID nym, blind::Purse* in) const;
#endif
    void save(NymData* nymData, const Lock& lock) const;
    void save(
        const PasswordPrompt& reason,
        opentxs::NymFile* nym,
        const Lock& lock) const;
    bool SaveCredentialIDs(const identity::Nym& nym) const;
    virtual Nym_p signer_nym(const identifier::Nym& id) const = 0;

    /* Throws std::out_of_range for missing accounts */
    AccountLock& account(
        const Lock& lock,
        const Identifier& accountID,
        const bool create) const;
    IssuerLock& issuer(
        const identifier::Nym& nymID,
        const identifier::Nym& issuerID,
        const bool create) const;

    OTServerContract server(std::unique_ptr<contract::Server> contract) const
        noexcept(false);
    OTUnitDefinition unit_definition(
        std::shared_ptr<contract::Unit>&& contract) const;

    Wallet() = delete;
    Wallet(const Wallet&) = delete;
    Wallet(Wallet&&) = delete;
    Wallet& operator=(const Wallet&) = delete;
    Wallet& operator=(Wallet&&) = delete;
};
}  // namespace opentxs::api::implementation
