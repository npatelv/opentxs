// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "0_stdafx.hpp"                    // IWYU pragma: associated
#include "1_Internal.hpp"                  // IWYU pragma: associated
#include "network/zeromq/socket/Pull.hpp"  // IWYU pragma: associated

#include "Factory.hpp"
#include "network/zeromq/curve/Server.hpp"
#include "network/zeromq/socket/Receiver.tpp"
#include "network/zeromq/socket/Socket.hpp"
#include "opentxs/Pimpl.hpp"
#include "opentxs/core/Log.hpp"
#include "opentxs/network/zeromq/ListenCallback.hpp"

namespace opentxs
{
namespace network
{
namespace zeromq
{
namespace socket
{
class Pull;
}  // namespace socket

class Context;
class Message;
}  // namespace zeromq
}  // namespace network
}  // namespace opentxs

template class opentxs::Pimpl<opentxs::network::zeromq::socket::Pull>;

//#define OT_METHOD
//"opentxs::network::zeromq::socket::implementation::Pull::"

namespace opentxs
{
network::zeromq::socket::Pull* Factory::PullSocket(
    const network::zeromq::Context& context,
    const bool direction)
{
    using ReturnType = network::zeromq::socket::implementation::Pull;

    return new ReturnType(
        context,
        static_cast<network::zeromq::socket::Socket::Direction>(direction));
}

network::zeromq::socket::Pull* Factory::PullSocket(
    const network::zeromq::Context& context,
    const bool direction,
    const network::zeromq::ListenCallback& callback)
{
    using ReturnType = network::zeromq::socket::implementation::Pull;

    return new ReturnType(
        context,
        static_cast<network::zeromq::socket::Socket::Direction>(direction),
        callback);
}
}  // namespace opentxs

namespace opentxs::network::zeromq::socket::implementation
{
Pull::Pull(
    const zeromq::Context& context,
    const Socket::Direction direction,
    const zeromq::ListenCallback& callback,
    const bool startThread) noexcept
    : Receiver(context, SocketType::Pull, direction, startThread)
    , Server(this->get())
    , callback_(callback)
{
    init();
}

Pull::Pull(
    const zeromq::Context& context,
    const Socket::Direction direction,
    const zeromq::ListenCallback& callback) noexcept
    : Pull(context, direction, callback, true)
{
}

Pull::Pull(
    const zeromq::Context& context,
    const Socket::Direction direction) noexcept
    : Pull(context, direction, ListenCallback::Factory(), false)
{
}

Pull* Pull::clone() const noexcept
{
    return new Pull(context_, direction_, callback_);
}

bool Pull::have_callback() const noexcept { return true; }

void Pull::process_incoming(const Lock& lock, Message& message) noexcept
{
    OT_ASSERT(verify_lock(lock))

    callback_.Process(message);
}

Pull::~Pull() SHUTDOWN
}  // namespace opentxs::network::zeromq::socket::implementation
