// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

// IWYU pragma: private
// IWYU pragma: friend ".*src/blockchain/p2p/bitcoin/message/Feefilter.cpp"

#pragma once

#include "1_Internal.hpp"  // IWYU pragma: associated

#include <cstdint>
#include <memory>
#include <set>

#include "blockchain/p2p/bitcoin/Message.hpp"
#include "internal/blockchain/p2p/bitcoin/Bitcoin.hpp"
#include "opentxs/Types.hpp"
#include "opentxs/core/Data.hpp"

namespace opentxs
{
namespace api
{
namespace internal
{
struct Core;
}  // namespace internal
}  // namespace api

namespace blockchain
{
namespace p2p
{
namespace bitcoin
{
class Header;
}  // namespace bitcoin
}  // namespace p2p
}  // namespace blockchain

class Factory;
}  // namespace opentxs

namespace opentxs::blockchain::p2p::bitcoin::message
{
class Feefilter final : virtual public bitcoin::Message
{
public:
    std::uint64_t feeRate() const noexcept { return fee_rate_; }

    ~Feefilter() final = default;

    OTData payload() const noexcept final;

private:
    friend opentxs::Factory;

    const std::uint64_t fee_rate_{};

    Feefilter(
        const api::internal::Core& api,
        const blockchain::Type network,
        const std::uint64_t fee_rate) noexcept;
    Feefilter(
        const api::internal::Core& api,
        std::unique_ptr<Header> header,
        const std::uint64_t fee_rate) noexcept(false);
    Feefilter(const Feefilter&) = delete;
    Feefilter(Feefilter&&) = delete;
    Feefilter& operator=(const Feefilter&) = delete;
    Feefilter& operator=(Feefilter&&) = delete;
};
}  // namespace opentxs::blockchain::p2p::bitcoin::message
