// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <cstdint>
#include <memory>
#include <set>

#include "opentxs/Types.hpp"
#include "opentxs/api/Editor.hpp"
#include "opentxs/core/Message.hpp"
#include "opentxs/core/identifier/Server.hpp"
#include "opentxs/identity/Nym.hpp"

namespace opentxs
{
namespace api
{
class Wallet;
}  // namespace api

namespace identity
{
class Nym;
}  // namespace identity

namespace server
{
class Server;
class UserCommandProcessor;
}  // namespace server

class Armored;
class ClientContext;
class Data;
class Identifier;
class PasswordPrompt;
class String;
}  // namespace opentxs

namespace opentxs::server
{
class ReplyMessage
{
public:
    ReplyMessage(
        const UserCommandProcessor& parent,
        const opentxs::api::Wallet& wallet,
        const identifier::Server& notaryID,
        const identity::Nym& signer,
        const Message& input,
        Server& server,
        const MessageType& type,
        Message& output,
        const PasswordPrompt& reason);

    std::set<RequestNumber> Acknowledged() const;
    bool HaveContext() const;
    const bool& Init() const;
    const Message& Original() const;
    const bool& Success() const;

    ClientContext& Context();
    void ClearRequest();
    void DropToNymbox(const bool success);
    bool LoadContext(const PasswordPrompt& reason);
    void OverrideType(const String& accountID);
    void SetAccount(const String& accountID);
    void SetAcknowledgments(const ClientContext& context);
    void SetBool(const bool value);
    void SetDepth(const std::int64_t depth);
    void SetEnum(const std::uint8_t value);
    void SetInboxHash(const Identifier& hash);
    void SetInstrumentDefinitionID(const String& id);
    void SetNymboxHash(const Identifier& hash);
    void SetOutboxHash(const Identifier& hash);
    bool SetPayload(const String& payload);
    bool SetPayload(const Data& payload);
    void SetPayload(const Armored& payload);
    bool SetPayload2(const String& payload);
    bool SetPayload3(const String& payload);
    void SetRequestNumber(const RequestNumber number);
    void SetSuccess(const bool success);
    void SetTargetNym(const String& nymID);
    void SetTransactionNumber(const TransactionNumber& number);

    ~ReplyMessage();

private:
    const UserCommandProcessor& parent_;
    const opentxs::api::Wallet& wallet_;
    const identity::Nym& signer_;
    const Message& original_;
    const PasswordPrompt& reason_;
    const OTServerID notary_id_;
    Message& message_;
    Server& server_;
    bool init_{false};
    bool drop_{false};
    bool drop_status_{false};
    Nym_p sender_nym_{nullptr};
    std::unique_ptr<Editor<ClientContext>> context_{nullptr};

    void attach_request();
    void clear_request();
    bool init();
    bool init_nym();

    ReplyMessage() = delete;
    ReplyMessage(const ReplyMessage&) = delete;
    ReplyMessage(ReplyMessage&&) = delete;
    ReplyMessage& operator=(const ReplyMessage&) = delete;
    ReplyMessage& operator=(ReplyMessage&&) = delete;
};
}  // namespace opentxs::server
