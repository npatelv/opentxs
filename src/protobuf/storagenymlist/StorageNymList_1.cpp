// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "opentxs/Proto.hpp"  // IWYU pragma: associated

#include "opentxs/protobuf/Basic.hpp"
#include "opentxs/protobuf/verify/StorageNymList.hpp"
#include "opentxs/protobuf/verify/VerifyStorage.hpp"
#include "protobuf/Check.hpp"

#define PROTO_NAME "nym index"

namespace opentxs
{
namespace proto
{

bool CheckProto_1(const StorageNymList& input, const bool silent)
{
    CHECK_SUBOBJECTS(nym, StorageNymListAllowedStorageItemHash());
    CHECK_NONE(localnymid)
    CHECK_NONE(address)
    CHECK_NONE(transaction)

    return true;
}

bool CheckProto_2(const StorageNymList& input, const bool silent)
{
    return CheckProto_1(input, silent);
}

bool CheckProto_3(const StorageNymList& input, const bool silent)
{
    CHECK_SUBOBJECTS(nym, StorageNymListAllowedStorageItemHash());
    CHECK_IDENTIFIERS(localnymid)
    CHECK_NONE(address)
    CHECK_NONE(transaction)
    return true;
}

bool CheckProto_4(const StorageNymList& input, const bool silent)
{
    CHECK_SUBOBJECTS(nym, StorageNymListAllowedStorageItemHash());
    CHECK_IDENTIFIERS(localnymid)
    OPTIONAL_SUBOBJECTS(
        address, StorageNymListAllowedStorageBip47NymAddressIndex())
    OPTIONAL_SUBOBJECTS(
        transaction, StorageNymListAllowedStorageBip47NymAddressIndex())

    return true;
}

bool CheckProto_5(const StorageNymList& input, const bool silent)
{
    UNDEFINED_VERSION(5)
}

bool CheckProto_6(const StorageNymList& input, const bool silent)
{
    UNDEFINED_VERSION(6)
}

bool CheckProto_7(const StorageNymList& input, const bool silent)
{
    UNDEFINED_VERSION(7)
}

bool CheckProto_8(const StorageNymList& input, const bool silent)
{
    UNDEFINED_VERSION(8)
}

bool CheckProto_9(const StorageNymList& input, const bool silent)
{
    UNDEFINED_VERSION(9)
}

bool CheckProto_10(const StorageNymList& input, const bool silent)
{
    UNDEFINED_VERSION(10)
}

bool CheckProto_11(const StorageNymList& input, const bool silent)
{
    UNDEFINED_VERSION(11)
}

bool CheckProto_12(const StorageNymList& input, const bool silent)
{
    UNDEFINED_VERSION(12)
}

bool CheckProto_13(const StorageNymList& input, const bool silent)
{
    UNDEFINED_VERSION(13)
}

bool CheckProto_14(const StorageNymList& input, const bool silent)
{
    UNDEFINED_VERSION(14)
}

bool CheckProto_15(const StorageNymList& input, const bool silent)
{
    UNDEFINED_VERSION(15)
}

bool CheckProto_16(const StorageNymList& input, const bool silent)
{
    UNDEFINED_VERSION(16)
}

bool CheckProto_17(const StorageNymList& input, const bool silent)
{
    UNDEFINED_VERSION(17)
}

bool CheckProto_18(const StorageNymList& input, const bool silent)
{
    UNDEFINED_VERSION(18)
}

bool CheckProto_19(const StorageNymList& input, const bool silent)
{
    UNDEFINED_VERSION(19)
}

bool CheckProto_20(const StorageNymList& input, const bool silent)
{
    UNDEFINED_VERSION(20)
}
}  // namespace proto
}  // namespace opentxs
