# Copyright (c) 2010-2020 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

set(cxx-sources Factory.cpp Manager.cpp Wallet.cpp)
set(
  cxx-install-headers
  "${opentxs_SOURCE_DIR}/include/opentxs/api/server/Manager.hpp"
)
set(
  cxx-headers
  ${cxx-install-headers}
  "${opentxs_SOURCE_DIR}/src/internal/api/server/Server.hpp"
  Factory.hpp
  Manager.hpp
  Wallet.hpp
)

add_library(opentxs-api-server OBJECT ${cxx-sources} ${cxx-headers})
target_link_libraries(opentxs-api-server PRIVATE opentxs::messages)
set_property(TARGET opentxs-api-server PROPERTY POSITION_INDEPENDENT_CODE 1)

install(
  FILES ${cxx-install-headers}
  DESTINATION include/opentxs/api/server
  COMPONENT dev
)
