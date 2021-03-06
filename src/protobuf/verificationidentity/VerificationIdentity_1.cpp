// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "opentxs/Proto.hpp"  // IWYU pragma: associated

#include <stdexcept>
#include <string>
#include <utility>

#include "opentxs/protobuf/Basic.hpp"
#include "opentxs/protobuf/Check.hpp"
#include "opentxs/protobuf/verify/Verification.hpp"
#include "opentxs/protobuf/verify/VerificationIdentity.hpp"
#include "opentxs/protobuf/verify/VerifyContacts.hpp"
#include "protobuf/Check.hpp"

#define PROTO_NAME "verification identity"

namespace opentxs
{
namespace proto
{

bool CheckProto_1(
    const VerificationIdentity& input,
    const bool silent,
    VerificationNymMap& map,
    const VerificationType indexed)
{
    if (!input.has_nym()) { FAIL_1("missing nym") }

    if (MIN_PLAUSIBLE_IDENTIFIER > input.nym().size()) {
        FAIL_2("invalid nym", input.nym())
    }

    map[input.nym()] += 1;

    for (auto& it : input.verification()) {
        try {
            const bool verification = Check(
                it,
                VerificationIdentityAllowedVerification()
                    .at(input.version())
                    .first,
                VerificationIdentityAllowedVerification()
                    .at(input.version())
                    .second,
                silent,
                indexed);

            if (!verification) { FAIL_1("invalid verification") }
        } catch (const std::out_of_range&) {
            FAIL_2(
                "allowed verification version not defined for version",
                input.version())
        }
    }

    return true;
}

bool CheckProto_2(
    const VerificationIdentity& input,
    const bool silent,
    VerificationNymMap&,
    const VerificationType)
{
    UNDEFINED_VERSION(2)
}

bool CheckProto_3(
    const VerificationIdentity& input,
    const bool silent,
    VerificationNymMap&,
    const VerificationType)
{
    UNDEFINED_VERSION(3)
}

bool CheckProto_4(
    const VerificationIdentity& input,
    const bool silent,
    VerificationNymMap&,
    const VerificationType)
{
    UNDEFINED_VERSION(4)
}

bool CheckProto_5(
    const VerificationIdentity& input,
    const bool silent,
    VerificationNymMap&,
    const VerificationType)
{
    UNDEFINED_VERSION(5)
}

bool CheckProto_6(
    const VerificationIdentity& input,
    const bool silent,
    VerificationNymMap&,
    const VerificationType)
{
    UNDEFINED_VERSION(6)
}

bool CheckProto_7(
    const VerificationIdentity& input,
    const bool silent,
    VerificationNymMap&,
    const VerificationType)
{
    UNDEFINED_VERSION(7)
}

bool CheckProto_8(
    const VerificationIdentity& input,
    const bool silent,
    VerificationNymMap&,
    const VerificationType)
{
    UNDEFINED_VERSION(8)
}

bool CheckProto_9(
    const VerificationIdentity& input,
    const bool silent,
    VerificationNymMap&,
    const VerificationType)
{
    UNDEFINED_VERSION(9)
}

bool CheckProto_10(
    const VerificationIdentity& input,
    const bool silent,
    VerificationNymMap&,
    const VerificationType)
{
    UNDEFINED_VERSION(10)
}

bool CheckProto_11(
    const VerificationIdentity& input,
    const bool silent,
    VerificationNymMap&,
    const VerificationType)
{
    UNDEFINED_VERSION(11)
}

bool CheckProto_12(
    const VerificationIdentity& input,
    const bool silent,
    VerificationNymMap&,
    const VerificationType)
{
    UNDEFINED_VERSION(12)
}

bool CheckProto_13(
    const VerificationIdentity& input,
    const bool silent,
    VerificationNymMap&,
    const VerificationType)
{
    UNDEFINED_VERSION(13)
}

bool CheckProto_14(
    const VerificationIdentity& input,
    const bool silent,
    VerificationNymMap&,
    const VerificationType)
{
    UNDEFINED_VERSION(14)
}

bool CheckProto_15(
    const VerificationIdentity& input,
    const bool silent,
    VerificationNymMap&,
    const VerificationType)
{
    UNDEFINED_VERSION(15)
}

bool CheckProto_16(
    const VerificationIdentity& input,
    const bool silent,
    VerificationNymMap&,
    const VerificationType)
{
    UNDEFINED_VERSION(16)
}

bool CheckProto_17(
    const VerificationIdentity& input,
    const bool silent,
    VerificationNymMap&,
    const VerificationType)
{
    UNDEFINED_VERSION(17)
}

bool CheckProto_18(
    const VerificationIdentity& input,
    const bool silent,
    VerificationNymMap&,
    const VerificationType)
{
    UNDEFINED_VERSION(18)
}

bool CheckProto_19(
    const VerificationIdentity& input,
    const bool silent,
    VerificationNymMap&,
    const VerificationType)
{
    UNDEFINED_VERSION(19)
}

bool CheckProto_20(
    const VerificationIdentity& input,
    const bool silent,
    VerificationNymMap&,
    const VerificationType)
{
    UNDEFINED_VERSION(20)
}
}  // namespace proto
}  // namespace opentxs
