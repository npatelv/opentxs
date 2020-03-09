// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "Internal.hpp"

namespace opentxs::network::zeromq::implementation
{
class Message : virtual public zeromq::Message
{
public:
    auto at(const std::size_t index) const -> const Frame& final;
    auto begin() const -> FrameIterator final;
    auto Body() const -> const FrameSection final;
    auto Body_at(const std::size_t index) const -> const Frame& final;
    auto Body_begin() const -> FrameIterator final;
    auto Body_end() const -> FrameIterator final;
    auto end() const -> FrameIterator final;
    auto Header() const -> const FrameSection final;
    auto Header_at(const std::size_t index) const -> const Frame& final;
    auto Header_begin() const -> FrameIterator final;
    auto Header_end() const -> FrameIterator final;
    auto size() const -> std::size_t final;

    auto AddFrame() -> Frame& final;
    auto AddFrame(const ProtobufType& input) -> Frame& final;
    auto AddFrame(const void* input, const std::size_t size) -> Frame& final;
    auto at(const std::size_t index) -> Frame& final;

    auto Body() -> FrameSection final;
    auto EnsureDelimiter() -> void final;
    auto Header() -> FrameSection final;
    auto PrependEmptyFrame() -> void final;
    auto Replace(const std::size_t index, OTZMQFrame&& frame) -> Frame& final;

    ~Message() override = default;

protected:
    std::vector<OTZMQFrame> messages_{};

    auto body_position() const -> std::size_t;

    auto set_field(const std::size_t position, const zeromq::Frame& input)
        -> bool;

    Message();
    Message(const Message& rhs);

private:
    friend opentxs::Factory;
    friend network::zeromq::Message;

    auto clone() const -> Message* override { return new Message(*this); }
    auto hasDivider() const -> bool;
    auto findDivider() const -> std::size_t;

    Message(Message&&) = delete;
    Message& operator=(const Message&) = delete;
    Message& operator=(Message&&) = delete;
};
}  // namespace opentxs::network::zeromq::implementation
