// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "0_stdafx.hpp"          // IWYU pragma: associated
#include "1_Internal.hpp"        // IWYU pragma: associated
#include "opentxs/core/Log.hpp"  // IWYU pragma: associated

#include <thread>

#include "opentxs/Version.hpp"

namespace opentxs
{
const char* m_strVersion{OPENTXS_VERSION_STRING};
const char* m_strPathSeparator{"/"};

LogSource LogOutput{-1};
LogSource LogNormal{0};
LogSource LogDetail{1};
LogSource LogVerbose{2};
LogSource LogDebug{3};
LogSource LogTrace{4};
LogSource LogInsane{5};

const char* PathSeparator() { return m_strPathSeparator; }
const char* Version() { return m_strVersion; }

// static
bool Sleep(const std::chrono::microseconds us)
{
    auto start = std::chrono::high_resolution_clock::now();
    auto end = start + us;
    do {
        std::this_thread::yield();
        std::this_thread::sleep_for(us);
    } while (std::chrono::high_resolution_clock::now() < end);
    return true;
}
}  // namespace opentxs
