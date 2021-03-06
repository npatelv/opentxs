// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "0_stdafx.hpp"                      // IWYU pragma: associated
#include "1_Internal.hpp"                    // IWYU pragma: associated
#include "network/zeromq/ReplyCallback.hpp"  // IWYU pragma: associated

#include "opentxs/Pimpl.hpp"
#include "opentxs/network/zeromq/Message.hpp"

template class opentxs::Pimpl<opentxs::network::zeromq::ReplyCallback>;

//#define OT_METHOD "opentxs::network::zeromq::implementation::ReplyCallback::"

namespace opentxs::network::zeromq
{
OTZMQReplyCallback ReplyCallback::Factory(
    zeromq::ReplyCallback::ReceiveCallback callback)
{
    return OTZMQReplyCallback(new implementation::ReplyCallback(callback));
}
}  // namespace opentxs::network::zeromq

namespace opentxs::network::zeromq::implementation
{
ReplyCallback::ReplyCallback(zeromq::ReplyCallback::ReceiveCallback callback)
    : callback_(callback)
{
}

ReplyCallback* ReplyCallback::clone() const
{
    return new ReplyCallback(callback_);
}

OTZMQMessage ReplyCallback::Process(const zeromq::Message& message) const
{
    return callback_(message);
}

ReplyCallback::~ReplyCallback() {}
}  // namespace opentxs::network::zeromq::implementation
