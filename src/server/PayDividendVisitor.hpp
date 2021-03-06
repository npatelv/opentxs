// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <cstdint>

#include "opentxs/core/Account.hpp"
#include "opentxs/core/AccountVisitor.hpp"
#include "opentxs/core/Identifier.hpp"
#include "opentxs/core/String.hpp"
#include "opentxs/core/identifier/Nym.hpp"
#include "opentxs/core/identifier/UnitDefinition.hpp"

namespace opentxs
{
namespace identifier
{
class Server;
}  // namespace identifier

namespace server
{
class Server;
}  // namespace server

class PasswordPrompt;
}  // namespace opentxs

namespace opentxs
{
// Note: from OTUnitDefinition.h and .cpp.
// This is a subclass of AccountVisitor, which is used whenever OTUnitDefinition
// needs to loop through all the accounts for a given instrument definition (its
// own.) This subclass needs to call Server method to do its job, so it can't be
// defined in otlib, but must be defined here in otserver (so it can see the
// methods that it needs...)
class PayDividendVisitor final : public AccountVisitor
{
    server::Server& server_;
    const OTNymID nymId_;
    const OTUnitID payoutUnitTypeId_;
    const OTIdentifier voucherAcctId_;
    OTString m_pstrMemo;  // contains the original payDividend item from
                          // the payDividend transaction request.
                          // (Stored in the memo field for each
                          // voucher.)
    std::int64_t m_lPayoutPerShare{0};
    std::int64_t m_lAmountPaidOut{0};   // as we pay each voucher out, we keep a
                                        // running count.
    std::int64_t m_lAmountReturned{0};  // as we pay each voucher out, we keep a
                                        // running count.

    PayDividendVisitor() = delete;

public:
    PayDividendVisitor(
        server::Server& theServer,
        const identifier::Server& theNotaryID,
        const identifier::Nym& theNymID,
        const identifier::UnitDefinition& thePayoutUnitTypeId,
        const Identifier& theVoucherAcctID,
        const String& strMemo,
        std::int64_t lPayoutPerShare);

    const identifier::Nym& GetNymID() { return nymId_; }
    const identifier::UnitDefinition& GetPayoutUnitTypeId()
    {
        return payoutUnitTypeId_;
    }
    const Identifier& GetVoucherAcctID() { return voucherAcctId_; }
    OTString GetMemo() { return m_pstrMemo; }
    server::Server& GetServer() { return server_; }
    std::int64_t GetPayoutPerShare() { return m_lPayoutPerShare; }
    std::int64_t GetAmountPaidOut() { return m_lAmountPaidOut; }
    std::int64_t GetAmountReturned() { return m_lAmountReturned; }

    bool Trigger(const Account& theAccount, const PasswordPrompt& reason) final;

    ~PayDividendVisitor() final;
};
}  // namespace opentxs
