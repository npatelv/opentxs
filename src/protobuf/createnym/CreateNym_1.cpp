// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "opentxs/Proto.hpp"  // IWYU pragma: associated

#include <set>

#include "opentxs/protobuf/Basic.hpp"
#include "opentxs/protobuf/Contact.hpp"
#include "opentxs/protobuf/verify/CreateNym.hpp"
#include "opentxs/protobuf/verify/VerifyRPC.hpp"
#include "protobuf/Check.hpp"

#define PROTO_NAME "create nym"

namespace opentxs
{
namespace proto
{

bool CheckProto_1(const CreateNym& input, const bool silent)
{
    const auto allowedtype =
        1 ==
        AllowedItemTypes().at({5, CONTACTSECTION_SCOPE}).count(input.type());

    if (false == allowedtype) { FAIL_1("Invalid type") }

    CHECK_NAME(name)
    OPTIONAL_IDENTIFIER(seedid)
    OPTIONAL_SUBOBJECTS(claims, CreateNymAllowedAddClaim())

    return true;
}

bool CheckProto_2(const CreateNym& input, const bool silent)
{
    return CheckProto_1(input, silent);
}

bool CheckProto_3(const CreateNym& input, const bool silent)
{
    UNDEFINED_VERSION(3)
}

bool CheckProto_4(const CreateNym& input, const bool silent)
{
    UNDEFINED_VERSION(4)
}

bool CheckProto_5(const CreateNym& input, const bool silent)
{
    UNDEFINED_VERSION(5)
}

bool CheckProto_6(const CreateNym& input, const bool silent)
{
    UNDEFINED_VERSION(6)
}

bool CheckProto_7(const CreateNym& input, const bool silent)
{
    UNDEFINED_VERSION(7)
}

bool CheckProto_8(const CreateNym& input, const bool silent)
{
    UNDEFINED_VERSION(8)
}

bool CheckProto_9(const CreateNym& input, const bool silent)
{
    UNDEFINED_VERSION(9)
}

bool CheckProto_10(const CreateNym& input, const bool silent)
{
    UNDEFINED_VERSION(10)
}

bool CheckProto_11(const CreateNym& input, const bool silent)
{
    UNDEFINED_VERSION(11)
}

bool CheckProto_12(const CreateNym& input, const bool silent)
{
    UNDEFINED_VERSION(12)
}

bool CheckProto_13(const CreateNym& input, const bool silent)
{
    UNDEFINED_VERSION(13)
}

bool CheckProto_14(const CreateNym& input, const bool silent)
{
    UNDEFINED_VERSION(14)
}

bool CheckProto_15(const CreateNym& input, const bool silent)
{
    UNDEFINED_VERSION(15)
}

bool CheckProto_16(const CreateNym& input, const bool silent)
{
    UNDEFINED_VERSION(16)
}

bool CheckProto_17(const CreateNym& input, const bool silent)
{
    UNDEFINED_VERSION(17)
}

bool CheckProto_18(const CreateNym& input, const bool silent)
{
    UNDEFINED_VERSION(18)
}

bool CheckProto_19(const CreateNym& input, const bool silent)
{
    UNDEFINED_VERSION(19)
}

bool CheckProto_20(const CreateNym& input, const bool silent)
{
    UNDEFINED_VERSION(20)
}
}  // namespace proto
}  // namespace opentxs
