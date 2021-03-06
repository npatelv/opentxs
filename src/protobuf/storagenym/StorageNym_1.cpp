// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "opentxs/Proto.hpp"  // IWYU pragma: associated

#include "opentxs/protobuf/Basic.hpp"
#include "opentxs/protobuf/verify/StorageNym.hpp"
#include "opentxs/protobuf/verify/VerifyStorage.hpp"
#include "protobuf/Check.hpp"

#define PROTO_NAME "storage nym"

namespace opentxs::proto
{
bool CheckProto_1(const StorageNym& input, const bool silent)
{
    OPTIONAL_SUBOBJECT(credlist, StorageNymAllowedStorageItemHash())
    OPTIONAL_SUBOBJECT(sentpeerrequests, StorageNymAllowedStorageItemHash())
    OPTIONAL_SUBOBJECT(incomingpeerrequests, StorageNymAllowedStorageItemHash())
    OPTIONAL_SUBOBJECT(sentpeerreply, StorageNymAllowedStorageItemHash())
    OPTIONAL_SUBOBJECT(incomingpeerreply, StorageNymAllowedStorageItemHash())
    OPTIONAL_SUBOBJECT(finishedpeerrequest, StorageNymAllowedStorageItemHash())
    OPTIONAL_SUBOBJECT(finishedpeerreply, StorageNymAllowedStorageItemHash())
    OPTIONAL_SUBOBJECT(processedpeerrequest, StorageNymAllowedStorageItemHash())
    OPTIONAL_SUBOBJECT(processedpeerreply, StorageNymAllowedStorageItemHash())
    CHECK_EXCLUDED(mailinbox)
    CHECK_EXCLUDED(mailoutbox)
    CHECK_EXCLUDED(threads)
    CHECK_EXCLUDED(contexts)
    CHECK_EXCLUDED(accounts)
    CHECK_NONE(blockchainaccountindex)
    CHECK_NONE(hdaccount)
    CHECK_EXCLUDED(issuers)
    CHECK_EXCLUDED(paymentworkflow)
    CHECK_EXCLUDED(bip47)
    CHECK_NONE(purse);
    CHECK_EXCLUDED(txo)

    return true;
}
}  // namespace opentxs::proto
