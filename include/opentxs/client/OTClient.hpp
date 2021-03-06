// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef OPENTXS_CLIENT_OTCLIENT_HPP
#define OPENTXS_CLIENT_OTCLIENT_HPP

#include "opentxs/Forward.hpp"  // IWYU pragma: associated

#include <cstdint>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include "opentxs/Types.hpp"
#include "opentxs/consensus/ServerContext.hpp"

namespace opentxs
{
namespace api
{
namespace internal
{
struct Core;
}  // namespace internal
}  // namespace api

namespace contract
{
class Unit;
}  // namespace contract

class Account;
class Identifier;
class Message;
class PasswordPrompt;
class ServerContext;

class OTClient
{
public:
    std::int32_t ProcessUserCommand(
        const MessageType requestedCommand,
        ServerContext& context,
        Message& theMessage,
        const Identifier& pHisNymID,
        const Identifier& pHisAcctID,
        const PasswordPrompt& reason,
        const Amount lTransactionAmount = 0,
        const Account* pAccount = nullptr,
        const contract::Unit* pMyUnitDefinition = nullptr);

    explicit OTClient(const api::internal::Core& api);

protected:
    const api::internal::Core& api_;
};
}  // namespace opentxs
#endif
