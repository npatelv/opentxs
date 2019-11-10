// Copyright (c) 2010-2019 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

namespace opentxs::api::crypto::implementation
{
class Hash final : public api::crypto::Hash
{
public:
    bool Digest(
        const proto::HashType hashType,
        const OTPassword& data,
        OTPassword& digest) const noexcept final;
    bool Digest(const proto::HashType hashType, const Data& data, Data& digest)
        const noexcept final;
    bool Digest(
        const proto::HashType hashType,
        const String& data,
        Data& digest) const noexcept final;
    bool Digest(
        const proto::HashType hashType,
        const std::string& data,
        Data& digest) const noexcept final;
    bool Digest(
        const proto::HashType hashType,
        const opentxs::network::zeromq::Frame& data,
        Data& digest) const noexcept final;
    bool Digest(
        const proto::HashType hashType,
        const void* data,
        const std::size_t size,
        Data& digest) const noexcept final;
    bool Digest(
        const std::uint32_t type,
        const std::string& data,
        std::string& encodedDigest) const noexcept final;
    bool HMAC(
        const proto::HashType hashType,
        const OTPassword& key,
        const Data& data,
        OTPassword& digest) const noexcept final;
    void MurmurHash3_32(
        const std::uint32_t& key,
        const Data& data,
        std::uint32_t& output) const noexcept final;
    bool PKCS5_PBKDF2_HMAC(
        const Data& input,
        const Data& salt,
        const std::size_t iterations,
        const proto::HashType hashType,
        const std::size_t bytes,
        Data& output) const noexcept final;
    bool PKCS5_PBKDF2_HMAC(
        const OTPassword& input,
        const Data& salt,
        const std::size_t iterations,
        const proto::HashType hashType,
        const std::size_t bytes,
        Data& output) const noexcept final;
    bool PKCS5_PBKDF2_HMAC(
        const std::string& input,
        const Data& salt,
        const std::size_t iterations,
        const proto::HashType hashType,
        const std::size_t bytes,
        Data& output) const noexcept final;
    bool SipHash(
        const OTPassword& key,
        const Data& data,
        std::uint64_t& output,
        const int c,
        const int d) const noexcept final;

    Hash(
        const api::crypto::Encode& encode,
        const opentxs::crypto::HashingProvider& ssl,
        const opentxs::crypto::HashingProvider& sodium,
        const opentxs::crypto::Pbkdf2& pbkdf2,
        const opentxs::crypto::Ripemd160& ripe) noexcept;

    ~Hash() = default;

private:
    friend opentxs::Factory;

    const api::crypto::Encode& encode_;
    const opentxs::crypto::HashingProvider& ssl_;
    const opentxs::crypto::HashingProvider& sodium_;
    const opentxs::crypto::Pbkdf2& pbkdf2_;
    const opentxs::crypto::Ripemd160& ripe_;

    static bool Allocate(
        const proto::HashType hashType,
        OTPassword& input) noexcept;
    static bool Allocate(const proto::HashType hashType, Data& input) noexcept;

    bool Digest(
        const proto::HashType hashType,
        const std::uint8_t* input,
        const size_t size,
        std::uint8_t* output) const noexcept;
    bool HMAC(
        const proto::HashType hashType,
        const std::uint8_t* input,
        const size_t inputSize,
        const std::uint8_t* key,
        const size_t keySize,
        std::uint8_t* output) const noexcept;
    const opentxs::crypto::HashingProvider& SHA2() const noexcept
    {
        return ssl_;
    }
    const opentxs::crypto::HashingProvider& Sodium() const noexcept
    {
        return sodium_;
    }

    Hash(const Hash&) = delete;
    Hash(Hash&&) = delete;
    Hash& operator=(const Hash&) = delete;
    Hash& operator=(Hash&&) = delete;
};
}  // namespace opentxs::api::crypto::implementation
