// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <boost/endian/buffers.hpp>
#include <array>
#include <cstdint>
#include <memory>
#include <string>

#include "blockchain/block/Header.hpp"
#include "internal/blockchain/block/Block.hpp"
#include "opentxs/Bytes.hpp"
#include "opentxs/Types.hpp"
#include "opentxs/blockchain/Blockchain.hpp"
#include "opentxs/blockchain/NumericHash.hpp"
#include "opentxs/blockchain/Work.hpp"
#include "opentxs/blockchain/block/Header.hpp"
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

class Factory;
}  // namespace opentxs

namespace be = boost::endian;

namespace opentxs::blockchain::block::bitcoin::implementation
{
class Header final : virtual public internal::Header,
                     public block::implementation::Header
{
public:
    struct BitcoinFormat {
        be::little_int32_buf_t version_;
        std::array<char, 32> previous_;
        std::array<char, 32> merkle_;
        be::little_uint32_buf_t time_;
        be::little_int32_buf_t nbits_;
        be::little_uint32_buf_t nonce_;

        BitcoinFormat(
            const std::int32_t version,
            const std::string& previous,
            const std::string& merkle,
            const std::uint32_t time,
            const std::int32_t nbits,
            const std::uint32_t nonce) noexcept(false);
        BitcoinFormat() noexcept;
    };

    static const VersionNumber local_data_version_;

    std::unique_ptr<block::Header> clone() const noexcept final;
    OTData Encode() const noexcept final;
    const block::Hash& MerkleRoot() const noexcept final
    {
        return merkle_root_;
    }
    std::uint32_t Nonce() const noexcept final { return nonce_; }
    SerializedType Serialize() const noexcept final;
    auto Serialize(const AllocateOutput destination) const noexcept
        -> bool final;
    OTNumericHash Target() const noexcept final;
    Time Timestamp() const noexcept final { return timestamp_; }
    std::uint32_t Version() const noexcept final { return block_version_; }

    Header(
        const api::internal::Core& api,
        const blockchain::Type chain,
        const VersionNumber subversion,
        const block::Hash& hash,
        const std::int32_t version,
        const block::Hash& previous,
        const block::Hash& merkle,
        const Time timestamp,
        const std::int32_t nbits,
        const std::uint32_t nonce) noexcept;
    Header(
        const api::internal::Core& api,
        const blockchain::Type chain,
        const block::Hash& hash,
        const block::Hash& parentHash,
        const block::Height height) noexcept;
    Header(
        const api::internal::Core& api,
        const SerializedType& serialized) noexcept;

    ~Header() final = default;

private:
    friend opentxs::Factory;
    using ot_super = block::implementation::Header;

    static const VersionNumber subversion_default_;

    const VersionNumber subversion_;
    const std::int32_t block_version_;
    const OTData merkle_root_;
    const Time timestamp_;
    const std::int32_t nbits_;
    const std::uint32_t nonce_;

    static block::pHash calculate_hash(
        const api::internal::Core& api,
        const blockchain::Type chain,
        const ReadView serialized);
    static block::pHash calculate_hash(
        const api::internal::Core& api,
        const SerializedType& serialized);
    static OTWork calculate_work(const std::int32_t nbits);

    Header() = delete;
    Header(const Header& rhs) noexcept;
    Header(Header&&) = delete;
    Header& operator=(const Header&) = delete;
    Header& operator=(Header&&) = delete;
};
}  // namespace opentxs::blockchain::block::bitcoin::implementation
