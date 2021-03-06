// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "opentxs/Proto.hpp"  // IWYU pragma: associated

#include "opentxs/protobuf/verify/StorageRoot.hpp"
#include "protobuf/Check.hpp"

#define PROTO_NAME "storage root"

namespace opentxs
{
namespace proto
{
bool CheckProto_1(const StorageRoot& input, const bool silent)
{
    CHECK_IDENTIFIER(items)
    CHECK_EXCLUDED(sequence)

    return true;
}
}  // namespace proto
}  // namespace opentxs
