// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

// IWYU pragma: private
// IWYU pragma: friend ".*src/api/client/Wallet.cpp"

#pragma once

#include <memory>
#include <string>

#include "api/Wallet.hpp"
#include "opentxs/Types.hpp"
#include "opentxs/api/Editor.hpp"
#include "opentxs/network/zeromq/socket/Publish.hpp"

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

class Wallet;
}  // namespace api

namespace identifier
{
class Nym;
class Server;
}  // namespace identifier

namespace identity
{
class Nym;
}  // namespace identity

namespace internal
{
struct Context;
}  // namespace internal

namespace proto
{
class Context;
}  // namespace proto

class Context;
class Factory;
class Identifier;
class PasswordPrompt;
class ServerContext;
}  // namespace opentxs

namespace opentxs::api::client::implementation
{
class Wallet final : public api::implementation::Wallet
{
public:
    std::shared_ptr<const opentxs::Context> Context(
        const identifier::Server& notaryID,
        const identifier::Nym& clientNymID) const final;
    Editor<opentxs::Context> mutable_Context(
        const identifier::Server& notaryID,
        const identifier::Nym& clientNymID,
        const PasswordPrompt& reason) const final;
    Editor<opentxs::ServerContext> mutable_ServerContext(
        const identifier::Nym& localNymID,
        const Identifier& remoteID,
        const PasswordPrompt& reason) const final;
    std::shared_ptr<const opentxs::ServerContext> ServerContext(
        const identifier::Nym& localNymID,
        const Identifier& remoteID) const final;

    ~Wallet() = default;

private:
    friend opentxs::Factory;

    using ot_super = api::implementation::Wallet;

    const api::client::internal::Manager& client_;
    OTZMQPublishSocket request_sent_;
    OTZMQPublishSocket reply_received_;

    void instantiate_server_context(
        const proto::Context& serialized,
        const Nym_p& localNym,
        const Nym_p& remoteNym,
        std::shared_ptr<opentxs::internal::Context>& output) const final;
    void nym_to_contact(const identity::Nym& nym, const std::string& name) const
        noexcept final;
    Nym_p signer_nym(const identifier::Nym& id) const final;

    Wallet(const api::client::internal::Manager& client);
    Wallet() = delete;
    Wallet(const Wallet&) = delete;
    Wallet(Wallet&&) = delete;
    Wallet& operator=(const Wallet&) = delete;
    Wallet& operator=(Wallet&&) = delete;
};
}  // namespace opentxs::api::client::implementation
