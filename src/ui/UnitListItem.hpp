// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

// IWYU pragma: private
// IWYU pragma: friend ".*src/ui/UnitListItem.cpp"

#pragma once

#include <string>

#include "1_Internal.hpp"
#include "internal/ui/UI.hpp"
#include "opentxs/Pimpl.hpp"
#include "opentxs/Proto.hpp"
#include "opentxs/SharedPimpl.hpp"
#include "opentxs/Types.hpp"
#include "opentxs/Version.hpp"
#include "opentxs/core/Identifier.hpp"
#include "opentxs/core/contract/ServerContract.hpp"
#include "opentxs/core/contract/UnitDefinition.hpp"
#include "opentxs/protobuf/ContactEnums.pb.h"
#include "opentxs/ui/UnitListItem.hpp"
#include "ui/Row.hpp"

namespace opentxs
{
namespace api
{
namespace client
{
namespace internal
{
struct Manager;
}  // namespace internal
}  // namespace client

class Core;
}  // namespace api

namespace network
{
namespace zeromq
{
namespace socket
{
class Publish;
}  // namespace socket
}  // namespace zeromq
}  // namespace network

namespace ui
{
class UnitListItem;
}  // namespace ui
}  // namespace opentxs

namespace opentxs::ui::implementation
{
using UnitListItemRow =
    Row<UnitListRowInternal, UnitListInternalInterface, UnitListRowID>;

class UnitListItem final : public UnitListItemRow
{
public:
    auto Name() const noexcept -> std::string final { return name_; }
    auto Unit() const noexcept -> proto::ContactItemType final
    {
        return row_id_;
    }

#if OT_QT
    QVariant qt_data(const int column, const int role) const noexcept final;
#endif

    void reindex(const UnitListSortKey&, const CustomData&) noexcept final {}

    UnitListItem(
        const UnitListInternalInterface& parent,
        const api::client::internal::Manager& api,
        const network::zeromq::socket::Publish& publisher,
        const UnitListRowID& rowID,
        const UnitListSortKey& sortKey,
        const CustomData& custom) noexcept;

    ~UnitListItem() = default;

private:
    const UnitListSortKey name_;

    UnitListItem() = delete;
    UnitListItem(const UnitListItem&) = delete;
    UnitListItem(UnitListItem&&) = delete;
    UnitListItem& operator=(const UnitListItem&) = delete;
    UnitListItem& operator=(UnitListItem&&) = delete;
};
}  // namespace opentxs::ui::implementation

template class opentxs::SharedPimpl<opentxs::ui::UnitListItem>;
