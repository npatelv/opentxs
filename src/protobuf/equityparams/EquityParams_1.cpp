// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "opentxs/Proto.hpp"  // IWYU pragma: associated

#include "opentxs/protobuf/ContractEnums.pb.h"
#include "opentxs/protobuf/verify/EquityParams.hpp"
#include "protobuf/Check.hpp"

#define PROTO_NAME "equity params"

namespace opentxs
{
namespace proto
{

bool CheckProto_1(const EquityParams& input, const bool silent)
{
    if (!input.has_type()) { FAIL_1("") }

    switch (input.type()) {
        case EQUITYTYPE_SHARES: {
        } break;
        case EQUITYTYPE_ERROR:
        default: {
            FAIL_1("invalid type")
        }
    }

    return true;
}

bool CheckProto_2(const EquityParams& input, const bool silent)
{
    UNDEFINED_VERSION(2)
}

bool CheckProto_3(const EquityParams& input, const bool silent)
{
    UNDEFINED_VERSION(3)
}

bool CheckProto_4(const EquityParams& input, const bool silent)
{
    UNDEFINED_VERSION(4)
}

bool CheckProto_5(const EquityParams& input, const bool silent)
{
    UNDEFINED_VERSION(5)
}

bool CheckProto_6(const EquityParams& input, const bool silent)
{
    UNDEFINED_VERSION(6)
}

bool CheckProto_7(const EquityParams& input, const bool silent)
{
    UNDEFINED_VERSION(7)
}

bool CheckProto_8(const EquityParams& input, const bool silent)
{
    UNDEFINED_VERSION(8)
}

bool CheckProto_9(const EquityParams& input, const bool silent)
{
    UNDEFINED_VERSION(9)
}

bool CheckProto_10(const EquityParams& input, const bool silent)
{
    UNDEFINED_VERSION(10)
}

bool CheckProto_11(const EquityParams& input, const bool silent)
{
    UNDEFINED_VERSION(11)
}

bool CheckProto_12(const EquityParams& input, const bool silent)
{
    UNDEFINED_VERSION(12)
}

bool CheckProto_13(const EquityParams& input, const bool silent)
{
    UNDEFINED_VERSION(13)
}

bool CheckProto_14(const EquityParams& input, const bool silent)
{
    UNDEFINED_VERSION(14)
}

bool CheckProto_15(const EquityParams& input, const bool silent)
{
    UNDEFINED_VERSION(15)
}

bool CheckProto_16(const EquityParams& input, const bool silent)
{
    UNDEFINED_VERSION(16)
}

bool CheckProto_17(const EquityParams& input, const bool silent)
{
    UNDEFINED_VERSION(17)
}

bool CheckProto_18(const EquityParams& input, const bool silent)
{
    UNDEFINED_VERSION(18)
}

bool CheckProto_19(const EquityParams& input, const bool silent)
{
    UNDEFINED_VERSION(19)
}

bool CheckProto_20(const EquityParams& input, const bool silent)
{
    UNDEFINED_VERSION(20)
}
}  // namespace proto
}  // namespace opentxs
