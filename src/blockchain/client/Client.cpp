// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "0_stdafx.hpp"                           // IWYU pragma: associated
#include "1_Internal.hpp"                         // IWYU pragma: associated
#include "internal/blockchain/client/Client.hpp"  // IWYU pragma: associated

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <algorithm>
#include <functional>
#include <thread>

#include "opentxs/Pimpl.hpp"
#include "opentxs/api/Core.hpp"
#include "opentxs/api/Endpoints.hpp"
#include "opentxs/core/Log.hpp"
#include "opentxs/core/LogSource.hpp"
#include "opentxs/network/zeromq/Context.hpp"
#include "opentxs/network/zeromq/Frame.hpp"
#include "opentxs/network/zeromq/FrameSection.hpp"
#include "opentxs/network/zeromq/Message.hpp"
#include "opentxs/network/zeromq/socket/Socket.hpp"

namespace opentxs::blockchain::client::internal
{
IO::IO(const api::Core& api) noexcept
    : api_(api)
    , lock_()
    , cb_(zmq::ListenCallback::Factory([this](auto& in) { callback(in); }))
    , socket_(
          api.ZeroMQ().RouterSocket(cb_, zmq::socket::Socket::Direction::Bind))
    , next_buffer_(-1)
    , buffers_()
    , context_()
    , work_(std::make_unique<boost::asio::io_context::work>(context_))
    , thread_pool_()
{
    OT_ASSERT(work_);

    const auto limit = std::min(2u, std::thread::hardware_concurrency());

    for (unsigned int i{0}; i < limit; ++i) {
        thread_pool_.create_thread(
            boost::bind(&boost::asio::io_context::run, &context_));
    }

    const auto listen =
        socket_->Start(api_.Endpoints().InternalBlockchainAsioContext());

    OT_ASSERT(listen);
}

auto IO::callback(zmq::Message& in) noexcept -> void
{
    const auto header = in.Header();

    OT_ASSERT(1 < header.size());

    const auto& connectionID = header.at(0);

    switch (header.at(1).as<OTZMQWorkType>()) {
        case OT_ZMQ_REGISTER_SIGNAL: {
            auto reply = api_.ZeroMQ().ReplyMessage(in);
            reply->AddFrame(connectionID);
            socket_->Send(reply);
        } break;
        default: {
            OT_FAIL;
        }
    }
}

auto IO::clear_buffer(const int id) const noexcept -> void
{
    Lock lock(lock_);
    buffers_.erase(id);
}

auto IO::Connect(
    const Space& id,
    const tcp::endpoint& endpoint,
    tcp::socket& socket) const noexcept -> void
{
    socket.async_connect(endpoint, [this, id](const auto& e) {
        auto work = api_.ZeroMQ().Message(id);

        if (e) {
            LogVerbose("asio connect error: ")(e.message()).Flush();
            work->AddFrame(OTZMQWorkType{OT_ZMQ_DISCONNECT_SIGNAL});
            work->AddFrame();
            work->AddFrame();
        } else {
            work->AddFrame(OTZMQWorkType{OT_ZMQ_CONNECT_SIGNAL});
            work->AddFrame();
            work->AddFrame();
        }

        socket_->Send(work);
    });
}

auto IO::get_buffer(const std::size_t bytes) const noexcept
    -> std::pair<int, WritableView>
{
    Lock lock(lock_);
    auto id = ++next_buffer_;
    auto& buffer = buffers_[id];

    return {id, writer(buffer)(bytes)};
}

auto IO::Receive(
    const Space& id,
    const OTZMQWorkType type,
    const std::size_t bytes,
    tcp::socket& socket) const noexcept -> void
{
    auto space = get_buffer(bytes);
    auto asioBuffer =
        boost::asio::buffer(space.second.data(), space.second.size());
    boost::asio::async_read(
        socket,
        asioBuffer,
        [this, id, type, space{std::move(space)}](const auto& e, auto size) {
            auto work = api_.ZeroMQ().Message(id);
            auto& buffer = space.second;

            if (e) {
                LogVerbose("asio receive error: ")(e.message()).Flush();
                work->AddFrame(OTZMQWorkType{OT_ZMQ_DISCONNECT_SIGNAL});
                work->AddFrame();
                work->AddFrame();
            } else {
                work->AddFrame(type);
                work->AddFrame();
                work->AddFrame(buffer.data(), buffer.size());
            }

            socket_->Send(work);
            clear_buffer(space.first);
        });
}

auto IO::Shutdown() noexcept -> void
{
    context_.stop();
    thread_pool_.join_all();
    work_.reset();
    socket_->Close();
}

IO::~IO() { Shutdown(); }
}  // namespace opentxs::blockchain::client::internal
