// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

// IWYU pragma: private
// IWYU pragma: friend ".*src/ui/BlockchainAccountActivity.cpp"

#pragma once

#include <atomic>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "1_Internal.hpp"
#include "internal/api/client/Client.hpp"
#include "internal/blockchain/Blockchain.hpp"
#include "internal/ui/UI.hpp"
#include "opentxs/Proto.hpp"
#include "opentxs/SharedPimpl.hpp"
#include "opentxs/Types.hpp"
#include "opentxs/Version.hpp"
#include "opentxs/core/Identifier.hpp"
#include "opentxs/core/contract/UnitDefinition.hpp"
#include "opentxs/core/identifier/Server.hpp"
#include "opentxs/core/identifier/UnitDefinition.hpp"
#include "opentxs/network/zeromq/ListenCallback.hpp"
#include "opentxs/network/zeromq/socket/Dealer.hpp"
#include "opentxs/protobuf/ContactEnums.pb.h"
#include "opentxs/protobuf/PaymentWorkflowEnums.pb.h"
#include "opentxs/ui/AccountActivity.hpp"
#include "ui/AccountActivity.hpp"
#include "ui/List.hpp"
#include "ui/Widget.hpp"

namespace opentxs
{
namespace api
{
namespace client
{
namespace internal
{
struct Manager;
}  // namespace internal
}  // namespace client
}  // namespace api

namespace identifier
{
class Nym;
}  // namespace identifier

namespace network
{
namespace zeromq
{
namespace socket
{
class Publish;
}  // namespace socket

class Message;
}  // namespace zeromq
}  // namespace network

namespace proto
{
class PaymentEvent;
class PaymentWorkflow;
}  // namespace proto

class Identifier;
}  // namespace opentxs

namespace opentxs::ui::implementation
{
class BlockchainAccountActivity final : public AccountActivity
{
public:
    auto ContractID() const noexcept -> std::string final
    {
        return ui::UnitID(api_, chain_).str();
    }
    auto DepositAddress(const blockchain::Type) const noexcept
        -> std::string final;
    auto DepositChains() const noexcept -> std::vector<blockchain::Type> final
    {
        return {chain_};
    }
    auto DisplayBalance() const noexcept -> std::string final;
    auto DisplayUnit() const noexcept -> std::string final
    {
        return blockchain::internal::Ticker(chain_);
    }
    auto Name() const noexcept -> std::string final
    {
        return ui::AccountName(chain_);
    }
    auto NotaryID() const noexcept -> std::string final
    {
        return ui::NotaryID(api_, chain_).str();
    }
    auto NotaryName() const noexcept -> std::string final
    {
        return blockchain::internal::DisplayString(chain_);
    }
    auto Unit() const noexcept -> proto::ContactItemType final
    {
        return Translate(chain_);
    }

    BlockchainAccountActivity(
        const api::client::internal::Manager& api,
        const network::zeromq::socket::Publish& publisher,
        const identifier::Nym& nymID,
        const Identifier& accountID
#if OT_QT
        ,
        const bool qt
#endif
        ) noexcept;

    ~BlockchainAccountActivity() final = default;

private:
    const blockchain::Type chain_;
    OTZMQListenCallback balance_cb_;
    OTZMQDealerSocket balance_listener_;

    auto construct_row(
        const AccountActivityRowID& id,
        const AccountActivitySortKey& index,
        const CustomData& custom) const noexcept -> void* final;

    auto process_balance(const network::zeromq::Message& message) noexcept
        -> void;
    auto setup_listeners(const ListenerDefinitions& definitions) noexcept
        -> void final;
    auto startup() noexcept -> void final;

    BlockchainAccountActivity() = delete;
    BlockchainAccountActivity(const BlockchainAccountActivity&) = delete;
    BlockchainAccountActivity(BlockchainAccountActivity&&) = delete;
    BlockchainAccountActivity& operator=(const BlockchainAccountActivity&) =
        delete;
    BlockchainAccountActivity& operator=(BlockchainAccountActivity&&) = delete;
};
}  // namespace opentxs::ui::implementation
