// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

// IWYU pragma: private
// IWYU pragma: friend ".*src/network/zeromq/zap/Reply.cpp"

#pragma once

#include <map>
#include <string>

#include "Factory.hpp"
#include "network/zeromq/Message.hpp"
#include "opentxs/core/Data.hpp"
#include "opentxs/network/zeromq/Frame.hpp"
#include "opentxs/network/zeromq/zap/Reply.hpp"
#include "opentxs/network/zeromq/zap/ZAP.hpp"

namespace opentxs
{
namespace network
{
namespace zeromq
{
namespace zap
{
class Request;
}  // namespace zap
}  // namespace zeromq
}  // namespace network
}  // namespace opentxs

#define VERSION_POSITION 0
#define REQUEST_ID_POSITION 1
#define STATUS_CODE_POSITION 2
#define STATUS_TEXT_POSITION 3
#define USER_ID_POSITION 4
#define METADATA_POSITION 5

namespace opentxs::network::zeromq::zap::implementation
{
class Reply final : virtual zap::Reply, zeromq::implementation::Message
{
public:
    zap::Status Code() const override
    {
        return string_to_code(Body_at(STATUS_CODE_POSITION));
    }
    std::string Debug() const override;
    OTData Metadata() const override;
    OTData RequestID() const override;
    std::string Status() const override
    {
        return Body_at(STATUS_TEXT_POSITION);
    }
    std::string UserID() const override { return Body_at(USER_ID_POSITION); }
    std::string Version() const override { return Body_at(VERSION_POSITION); }

    bool SetCode(const zap::Status& code) override
    {
        const auto value = code_to_string(code);

        return set_field(
            STATUS_CODE_POSITION,
            OTZMQFrame{Factory::ZMQFrame(value.data(), value.size())});
    }
    bool SetMetadata(const Data& metadata) override
    {
        return set_field(
            METADATA_POSITION,
            OTZMQFrame{Factory::ZMQFrame(metadata.data(), metadata.size())});
    }
    bool SetStatus(const std::string& status) override
    {
        return set_field(
            STATUS_TEXT_POSITION,
            OTZMQFrame{Factory::ZMQFrame(status.data(), status.size())});
    }
    bool SetUserID(const std::string& userID) override
    {
        return set_field(
            USER_ID_POSITION,
            OTZMQFrame{Factory::ZMQFrame(userID.data(), userID.size())});
    }

    ~Reply() override = default;

private:
    friend network::zeromq::zap::Reply;

    using CodeMap = std::map<zap::Status, std::string>;
    using CodeReverseMap = std::map<std::string, zap::Status>;

    static const CodeMap code_map_;
    static const CodeReverseMap code_reverse_map_;

    static std::string code_to_string(const zap::Status& code);
    static CodeReverseMap invert_code_map(const CodeMap& input);
    static zap::Status string_to_code(const std::string& string);

    Reply* clone() const override { return new Reply(*this); }

    Reply(
        const Request& request,
        const zap::Status& code,
        const std::string& status,
        const std::string& userID,
        const Data& metadata,
        const std::string& version);
    Reply() = delete;
    Reply(const Reply&);
    Reply(Reply&&) = delete;
    Reply& operator=(const Reply&) = delete;
    Reply& operator=(Reply&&) = delete;
};
}  // namespace opentxs::network::zeromq::zap::implementation
