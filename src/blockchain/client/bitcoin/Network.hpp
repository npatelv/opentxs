// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <memory>
#include <string>

#include "blockchain/client/Network.hpp"
#include "opentxs/Bytes.hpp"
#include "opentxs/Types.hpp"

namespace opentxs
{
namespace api
{
namespace client
{
namespace internal
{
struct Blockchain;
}  // namespace internal
}  // namespace client

namespace internal
{
struct Core;
}  // namespace internal
}  // namespace api

namespace blockchain
{
namespace block
{
class Header;
}  // namespace block
}  // namespace blockchain
}  // namespace opentxs

namespace opentxs::blockchain::client::bitcoin::implementation
{
class Network final : public client::implementation::Network
{
public:
    std::unique_ptr<block::Header> instantiate_header(
        const ReadView payload) const noexcept final;

    Network(
        const api::internal::Core& api,
        const api::client::internal::Blockchain& blockchain,
        const Type type,
        const std::string& seednode,
        const std::string& shutdown);
    ~Network() final;

private:
    using ot_super = client::implementation::Network;

    Network() = delete;
    Network(const Network&) = delete;
    Network(Network&&) = delete;
    Network& operator=(const Network&) = delete;
    Network& operator=(Network&&) = delete;
};
}  // namespace opentxs::blockchain::client::bitcoin::implementation
