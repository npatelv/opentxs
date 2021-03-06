// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef OPENTXS_PROTOBUF_CONNECTIONINFOREPLY_HPP
#define OPENTXS_PROTOBUF_CONNECTIONINFOREPLY_HPP

#include "opentxs/Version.hpp"  // IWYU pragma: associated

#include "opentxs/protobuf/verify/VerifyPeer.hpp"

namespace opentxs
{
namespace proto
{
OPENTXS_EXPORT bool CheckProto_1(
    const ConnectionInfoReply& reply,
    const bool silent);
OPENTXS_EXPORT bool CheckProto_2(
    const ConnectionInfoReply& reply,
    const bool silent);
OPENTXS_EXPORT bool CheckProto_3(
    const ConnectionInfoReply& reply,
    const bool silent);
OPENTXS_EXPORT bool CheckProto_4(
    const ConnectionInfoReply& reply,
    const bool silent);
OPENTXS_EXPORT bool CheckProto_5(const ConnectionInfoReply&, const bool);
OPENTXS_EXPORT bool CheckProto_6(const ConnectionInfoReply&, const bool);
OPENTXS_EXPORT bool CheckProto_7(const ConnectionInfoReply&, const bool);
OPENTXS_EXPORT bool CheckProto_8(const ConnectionInfoReply&, const bool);
OPENTXS_EXPORT bool CheckProto_9(const ConnectionInfoReply&, const bool);
OPENTXS_EXPORT bool CheckProto_10(const ConnectionInfoReply&, const bool);
OPENTXS_EXPORT bool CheckProto_11(const ConnectionInfoReply&, const bool);
OPENTXS_EXPORT bool CheckProto_12(const ConnectionInfoReply&, const bool);
OPENTXS_EXPORT bool CheckProto_13(const ConnectionInfoReply&, const bool);
OPENTXS_EXPORT bool CheckProto_14(const ConnectionInfoReply&, const bool);
OPENTXS_EXPORT bool CheckProto_15(const ConnectionInfoReply&, const bool);
OPENTXS_EXPORT bool CheckProto_16(const ConnectionInfoReply&, const bool);
OPENTXS_EXPORT bool CheckProto_17(const ConnectionInfoReply&, const bool);
OPENTXS_EXPORT bool CheckProto_18(const ConnectionInfoReply&, const bool);
OPENTXS_EXPORT bool CheckProto_19(const ConnectionInfoReply&, const bool);
OPENTXS_EXPORT bool CheckProto_20(const ConnectionInfoReply&, const bool);
}  // namespace proto
}  // namespace opentxs

#endif  // OPENTXS_PROTOBUF_CONNECTIONINFOREPLY_HPP
