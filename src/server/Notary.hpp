// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <memory>

#include "opentxs/Version.hpp"
#include "opentxs/core/Account.hpp"
#include "opentxs/core/Cheque.hpp"
#include "opentxs/core/Item.hpp"
#include "opentxs/core/Ledger.hpp"
#include "opentxs/core/OTTransaction.hpp"
#include "opentxs/network/zeromq/socket/Push.hpp"

namespace opentxs
{
namespace api
{
namespace server
{
namespace internal
{
struct Manager;
}  // namespace internal
}  // namespace server
}  // namespace api

namespace blind
{
class Mint;
class Purse;
class Token;
}  // namespace blind

namespace identifier
{
class Nym;
class Server;
class UnitDefinition;
}  // namespace identifier

namespace identity
{
class Nym;
}  // namespace identity

namespace server
{
class Server;
}  // namespace server

class ClientContext;
class Identifier;
class PasswordPrompt;
class String;
}  // namespace opentxs

namespace opentxs::server
{
class Notary
{
public:
    void NotarizeProcessInbox(
        ClientContext& context,
        ExclusiveAccount& account,
        OTTransaction& tranIn,
        OTTransaction& tranOut,
        Ledger& inbox,
        Ledger& outbox,
        bool& outSuccess);
    bool NotarizeProcessNymbox(
        ClientContext& context,
        OTTransaction& tranIn,
        OTTransaction& tranOut,
        bool& outSuccess);
    void NotarizeTransaction(
        ClientContext& context,
        OTTransaction& tranIn,
        OTTransaction& tranOut,
        bool& outSuccess);

private:
    friend class Server;

    class Finalize
    {
    public:
        Finalize(
            const identity::Nym& signer,
            Item& item,
            Item& balanceItem,
            const PasswordPrompt& reason);
        Finalize() = delete;

        ~Finalize();

    private:
        const identity::Nym& signer_;
        Item& item_;
        Item& balance_item_;
        const PasswordPrompt& reason_;
    };

    Server& server_;
    const PasswordPrompt& reason_;
    const opentxs::api::server::internal::Manager& manager_;
    OTZMQPushSocket notification_socket_;

    void AddHashesToTransaction(
        OTTransaction& transaction,
        const Ledger& inbox,
        const Ledger& outbox,
        const Identifier& accounthash) const;
    std::unique_ptr<Cheque> extract_cheque(
        const identifier::Server& serverID,
        const identifier::UnitDefinition& unitID,
        const Item& item) const;
    void send_push_notification(
        const Account& account,
        const std::shared_ptr<const Ledger>& inbox,
        const std::shared_ptr<const Ledger>& outbox,
        const std::shared_ptr<const OTTransaction>& item) const;

    void cancel_cheque(
        const OTTransaction& input,
        const Cheque& cheque,
        const Item& depositItem,
        const String& serializedDepositItem,
        const Item& balanceItem,
        ClientContext& context,
        Account& account,
        Ledger& inbox,
        const Ledger& outbox,
        OTTransaction& output,
        bool& success,
        Item& responseItem,
        Item& responseBalanceItem);
    void deposit_cheque(
        const OTTransaction& input,
        const Item& depositItem,
        const String& serializedDepositItem,
        const Item& balanceItem,
        const Cheque& cheque,
        ClientContext& depositorContext,
        ExclusiveAccount& depositorAccount,
        Ledger& depositorInbox,
        const Ledger& depositorOutbox,
        OTTransaction& output,
        bool& success,
        Item& responseItem,
        Item& responseBalanceItem);
    void deposit_cheque(
        const OTTransaction& input,
        const Item& depositItem,
        const String& serializedDepositItem,
        const Item& balanceItem,
        const Cheque& cheque,
        const bool isVoucher,
        const bool cancelling,
        const identifier::Nym& senderNymID,
        ClientContext& senderContext,
        Account& senderAccount,
        Ledger& senderInbox,
        std::shared_ptr<OTTransaction>& inboxItem,
        Account& sourceAccount,
        const ClientContext& depositorContext,
        Account& depositorAccount,
        const Ledger& depositorInbox,
        const Ledger& depositorOutbox,
        bool& success,
        Item& responseItem,
        Item& responseBalanceItem);
    void NotarizeCancelCronItem(
        ClientContext& context,
        ExclusiveAccount& assetAccount,
        OTTransaction& tranIn,
        OTTransaction& tranOut,
        bool& outSuccess);
    void NotarizeDeposit(
        ClientContext& context,
        ExclusiveAccount& account,
        OTTransaction& tranIn,
        OTTransaction& tranOut,
        Ledger& inbox,
        Ledger& outbox,
        bool& outSuccess);
    void NotarizeExchangeBasket(
        ClientContext& context,
        ExclusiveAccount& sourceAccount,
        OTTransaction& tranIn,
        OTTransaction& tranOut,
        Ledger& inbox,
        Ledger& outbox,
        bool& outSuccess);
    void NotarizeMarketOffer(
        ClientContext& context,
        ExclusiveAccount& assetAccount,
        OTTransaction& tranIn,
        OTTransaction& tranOut,
        bool& outSuccess);
    void NotarizePayDividend(
        ClientContext& context,
        ExclusiveAccount& account,
        OTTransaction& tranIn,
        OTTransaction& tranOut,
        Ledger& inbox,
        Ledger& outbox,
        bool& outSuccess);
    void NotarizePaymentPlan(
        ClientContext& context,
        ExclusiveAccount& depositorAccount,
        OTTransaction& tranIn,
        OTTransaction& tranOut,
        bool& outSuccess);
    void NotarizeSmartContract(
        ClientContext& context,
        ExclusiveAccount& activatingAccount,
        OTTransaction& tranIn,
        OTTransaction& tranOut,
        bool& outSuccess);
    void NotarizeTransfer(
        ClientContext& context,
        ExclusiveAccount& fromAccount,
        OTTransaction& tranIn,
        OTTransaction& tranOut,
        Ledger& inbox,
        Ledger& outbox,
        bool& outSuccess);
    void NotarizeWithdrawal(
        ClientContext& context,
        ExclusiveAccount& account,
        OTTransaction& tranIn,
        OTTransaction& tranOut,
        Ledger& inbox,
        Ledger& outbox,
        bool& outSuccess);
    void process_cash_deposit(
        const OTTransaction& input,
        const Item& depositItem,
        const Item& balanceItem,
        ClientContext& context,
        ExclusiveAccount& depositorAccount,
        OTTransaction& output,
        Ledger& inbox,
        Ledger& outbox,
        bool& success,
        Item& responseItem,
        Item& responseBalanceItem);
#if OT_CASH
    void process_cash_withdrawal(
        const OTTransaction& requestTransaction,
        const Item& requestItem,
        const Item& balanceItem,
        ClientContext& context,
        ExclusiveAccount& account,
        Identifier& accountHash,
        Ledger& inbox,
        Ledger& outbox,
        Item& responseItem,
        Item& responseBalanceItem,
        bool& success);
#endif
    void process_cheque_deposit(
        const OTTransaction& input,
        const Item& depositItem,
        const Item& balanceItem,
        ClientContext& context,
        ExclusiveAccount& depositorAccount,
        OTTransaction& output,
        Ledger& inbox,
        Ledger& outbox,
        bool& success,
        Item& responseItem,
        Item& responseBalanceItem);
#if OT_CASH
    bool process_token_deposit(
        ExclusiveAccount& reserveAccount,
        Account& depositAccount,
        blind::Token& token);
    bool process_token_withdrawal(
        const identifier::UnitDefinition& unit,
        ClientContext& context,
        ExclusiveAccount& reserveAccount,
        Account& account,
        blind::Purse& replyPurse,
        std::shared_ptr<blind::Token> pToken);
    bool verify_token(blind::Mint& mint, blind::Token& token);
#endif

    Notary(
        Server& server,
        const PasswordPrompt& reason,
        const opentxs::api::server::internal::Manager& manager);
    Notary() = delete;
    Notary(const Notary&) = delete;
    Notary(Notary&&) = delete;
    Notary& operator=(const Notary&) = delete;
    Notary& operator=(Notary&&) = delete;
};
}  // namespace opentxs::server
