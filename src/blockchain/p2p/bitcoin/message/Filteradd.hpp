// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

// IWYU pragma: private
// IWYU pragma: friend ".*src/blockchain/p2p/bitcoin/message/Filteradd.cpp"

#pragma once

#include <memory>

#include "internal/blockchain/p2p/bitcoin/message/Message.hpp"
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

namespace opentxs::blockchain::p2p::bitcoin::message::implementation
{
class Filteradd final : virtual public internal::Filteradd
{
public:
    OTData Element() const noexcept final { return element_; }

    ~Filteradd() final = default;

private:
    friend opentxs::Factory;

    const OTData element_;

    OTData payload() const noexcept final;

    Filteradd(
        const api::internal::Core& api,
        const blockchain::Type network,
        const Data& element) noexcept;
    Filteradd(
        const api::internal::Core& api,
        std::unique_ptr<Header> header,
        const Data& element) noexcept;
    Filteradd(const Filteradd&) = delete;
    Filteradd(Filteradd&&) = delete;
    Filteradd& operator=(const Filteradd&) = delete;
    Filteradd& operator=(Filteradd&&) = delete;
};
}  // namespace opentxs::blockchain::p2p::bitcoin::message::implementation
