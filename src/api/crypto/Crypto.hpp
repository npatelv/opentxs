// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

// IWYU pragma: private
// IWYU pragma: friend ".*src/api/crypto/Crypto.cpp"

#pragma once

#include <map>
#include <memory>

#include "opentxs/Proto.hpp"
#include "opentxs/Types.hpp"
#include "opentxs/Version.hpp"
#include "opentxs/api/crypto/Config.hpp"
#include "opentxs/api/crypto/Crypto.hpp"
#include "opentxs/api/crypto/Encode.hpp"
#include "opentxs/api/crypto/Hash.hpp"
#include "opentxs/api/crypto/Util.hpp"
#include "opentxs/crypto/Bip32.hpp"
#include "opentxs/crypto/Bip39.hpp"
#include "opentxs/crypto/library/AsymmetricProvider.hpp"
#include "opentxs/crypto/library/EcdsaProvider.hpp"
#include "opentxs/crypto/library/SymmetricProvider.hpp"

namespace opentxs
{
namespace api
{
namespace crypto
{
class Config;
class Encode;
class Hash;
class Util;
}  // namespace crypto

class Settings;
}  // namespace api

namespace crypto
{
namespace key
{
class Symmetric;
}  // namespace key

class OpenSSL;
class Ripemd160;
class Secp256k1;
class Sodium;
}  // namespace crypto

class Factory;
}  // namespace opentxs

namespace opentxs::api::implementation
{
class Crypto final : virtual public opentxs::api::Crypto
{
public:
    const crypto::Config& Config() const final;

    // Encoding function interface
    const crypto::Encode& Encode() const final;

    // Hash function interface
    const crypto::Hash& Hash() const final;

    // Utility class for misc OpenSSL-provided functions
    const crypto::Util& Util() const final;

#if OT_CRYPTO_SUPPORTED_KEY_ED25519
    const opentxs::crypto::EcdsaProvider& ED25519() const final;
#endif  // OT_CRYPTO_SUPPORTED_KEY_ED25519
#if OT_CRYPTO_SUPPORTED_KEY_RSA
    const opentxs::crypto::AsymmetricProvider& RSA() const final;
#endif  // OT_CRYPTO_SUPPORTED_KEY_RSA
#if OT_CRYPTO_SUPPORTED_KEY_SECP256K1
    const opentxs::crypto::EcdsaProvider& SECP256K1() const final;
#endif  // OT_CRYPTO_SUPPORTED_KEY_SECP256K1
    const opentxs::crypto::SymmetricProvider& Sodium() const final;
    const opentxs::crypto::Bip32& BIP32() const final;
    const opentxs::crypto::Bip39& BIP39() const final;

    ~Crypto() final;

private:
    friend opentxs::Factory;

    std::unique_ptr<api::crypto::Config> config_;
    std::unique_ptr<opentxs::crypto::Sodium> sodium_;
#if OT_CRYPTO_USING_OPENSSL
    std::unique_ptr<opentxs::crypto::OpenSSL> ssl_;
#endif  // OT_CRYPTO_USING_OPENSSL
    const api::crypto::Util& util_;
#if OT_CRYPTO_USING_LIBSECP256K1
    std::unique_ptr<opentxs::crypto::Secp256k1> secp256k1_p_;
#endif  // OT_CRYPTO_USING_LIBSECP256K1
    std::unique_ptr<opentxs::crypto::Bip39> bip39_p_;
    const opentxs::crypto::Ripemd160& ripemd160_;
    std::unique_ptr<opentxs::crypto::Bip32> bip32_p_;
    const opentxs::crypto::Bip32& bip32_;
    const opentxs::crypto::Bip39& bip39_;
#if OT_CRYPTO_SUPPORTED_KEY_SECP256K1
    const opentxs::crypto::EcdsaProvider& secp256k1_;
#endif  // OT_CRYPTO_SUPPORTED_KEY_SECP256K1
    std::unique_ptr<crypto::Encode> encode_;
    std::unique_ptr<crypto::Hash> hash_;

    void Init();
    void Cleanup();

    Crypto(const api::Settings& settings);
    Crypto() = delete;
    Crypto(const Crypto&) = delete;
    Crypto(Crypto&&) = delete;
    Crypto& operator=(const Crypto&) = delete;
    Crypto& operator=(Crypto&&) = delete;
};
}  // namespace opentxs::api::implementation
