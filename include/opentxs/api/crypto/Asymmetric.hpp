// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef OPENTXS_API_CRYPTO_ASYMMETRIC_HPP
#define OPENTXS_API_CRYPTO_ASYMMETRIC_HPP

// IWYU pragma: no_include "opentxs/Proto.hpp"

#include "opentxs/Forward.hpp"  // IWYU pragma: associated

#include <memory>

#include "opentxs/crypto/key/Asymmetric.hpp"
#include "opentxs/crypto/key/EllipticCurve.hpp"
#if OT_CRYPTO_SUPPORTED_KEY_SECP256K1
#include "opentxs/crypto/key/Secp256k1.hpp"
#endif  // OT_CRYPTO_SUPPORTED_KEY_SECP256K1
#if OT_CRYPTO_WITH_BIP32
#include "opentxs/crypto/Bip32.hpp"
#endif  // OT_CRYPTO_WITH_BIP32
#include "opentxs/Proto.hpp"
#include "opentxs/Types.hpp"

namespace opentxs
{
namespace api
{
namespace crypto
{
class Asymmetric
{
public:
    using ECKey = std::unique_ptr<opentxs::crypto::key::EllipticCurve>;
    using Key = std::unique_ptr<opentxs::crypto::key::Asymmetric>;
    using HDKey = std::unique_ptr<opentxs::crypto::key::HD>;
#if OT_CRYPTO_SUPPORTED_KEY_SECP256K1
    using Secp256k1Key = std::unique_ptr<opentxs::crypto::key::Secp256k1>;
#endif  // OT_CRYPTO_SUPPORTED_KEY_SECP256K1

    OPENTXS_EXPORT virtual ECKey InstantiateECKey(
        const proto::AsymmetricKey& serialized) const = 0;
    OPENTXS_EXPORT virtual HDKey InstantiateHDKey(
        const proto::AsymmetricKey& serialized) const = 0;
#if OT_CRYPTO_WITH_BIP32
    OPENTXS_EXPORT virtual HDKey InstantiateKey(
        const proto::AsymmetricKeyType type,
        const std::string& seedID,
        const opentxs::crypto::Bip32::Key& serialized,
        const PasswordPrompt& reason,
        const proto::KeyRole role = proto::KEYROLE_SIGN,
        const VersionNumber version =
            opentxs::crypto::key::EllipticCurve::DefaultVersion) const = 0;
#endif  // OT_CRYPTO_WITH_BIP32
    OPENTXS_EXPORT virtual Key InstantiateKey(
        const proto::AsymmetricKey& serialized) const = 0;
#if OT_CRYPTO_WITH_BIP32
    OPENTXS_EXPORT virtual HDKey NewHDKey(
        const std::string& seedID,
        const OTPassword& seed,
        const EcdsaCurve& curve,
        const opentxs::crypto::Bip32::Path& path,
        const PasswordPrompt& reason,
        const proto::KeyRole role = proto::KEYROLE_SIGN,
        const VersionNumber version =
            opentxs::crypto::key::EllipticCurve::DefaultVersion) const = 0;
#if OT_CRYPTO_SUPPORTED_KEY_SECP256K1
    OPENTXS_EXPORT virtual Secp256k1Key NewSecp256k1Key(
        const std::string& seedID,
        const OTPassword& seed,
        const opentxs::crypto::Bip32::Path& path,
        const PasswordPrompt& reason,
        const proto::KeyRole role = proto::KEYROLE_SIGN,
        const VersionNumber version =
            opentxs::crypto::key::Secp256k1::DefaultVersion) const = 0;
#endif  // OT_CRYPTO_SUPPORTED_KEY_SECP256K1
#endif  // OT_CRYPTO_WITH_BIP32
    OPENTXS_EXPORT virtual Key NewKey(
        const NymParameters& params,
        const PasswordPrompt& reason,
        const proto::KeyRole role = proto::KEYROLE_SIGN,
        const VersionNumber version =
            opentxs::crypto::key::Asymmetric::DefaultVersion) const = 0;

    OPENTXS_EXPORT virtual ~Asymmetric() = default;

protected:
    Asymmetric() = default;

private:
    Asymmetric(const Asymmetric&) = delete;
    Asymmetric(Asymmetric&&) = delete;
    Asymmetric& operator=(const Asymmetric&) = delete;
    Asymmetric& operator=(Asymmetric&&) = delete;
};
}  // namespace crypto
}  // namespace api
}  // namespace opentxs
#endif
