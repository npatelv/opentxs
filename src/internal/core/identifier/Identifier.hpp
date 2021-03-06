// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "opentxs/core/identifier/Nym.hpp"
#include "opentxs/core/identifier/Server.hpp"
#include "opentxs/core/identifier/UnitDefinition.hpp"

namespace opentxs
{
template <>
struct make_blank<OTNymID> {
    static OTNymID value(const api::Core&)
    {
        return identifier::Nym::Factory();
    }
};

template <>
struct make_blank<OTServerID> {
    static OTServerID value(const api::Core&)
    {
        return identifier::Server::Factory();
    }
};

template <>
struct make_blank<OTUnitID> {
    static OTUnitID value(const api::Core&)
    {
        return identifier::UnitDefinition::Factory();
    }
};
}  // namespace opentxs
