// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "opentxs/protobuf/verify/VerifyCash.hpp"  // IWYU pragma: associated

namespace opentxs::proto
{
const VersionMap& LucreTokenDataAllowedCiphertext() noexcept
{
    static const auto output = VersionMap{
        {1, {1, 1}},
    };

    return output;
}
const VersionMap& PurseAllowedCiphertext() noexcept
{
    static const auto output = VersionMap{
        {1, {1, 1}},
    };

    return output;
}
const VersionMap& PurseAllowedEnvelope() noexcept
{
    static const auto output = VersionMap{
        {1, {2, 2}},
    };

    return output;
}
const VersionMap& PurseAllowedSymmetricKey() noexcept
{
    static const auto output = VersionMap{
        {1, {1, 1}},
    };

    return output;
}
const VersionMap& PurseAllowedToken() noexcept
{
    static const auto output = VersionMap{
        {1, {1, 1}},
    };

    return output;
}
const VersionMap& PurseExchangeAllowedPurse() noexcept
{
    static const auto output = VersionMap{
        {1, {1, 1}},
    };

    return output;
}
const VersionMap& TokenAllowedLucreTokenData() noexcept
{
    static const auto output = VersionMap{
        {1, {1, 1}},
    };

    return output;
}
}  // namespace opentxs::proto
