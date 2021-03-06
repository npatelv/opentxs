// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "0_stdafx.hpp"            // IWYU pragma: associated
#include "1_Internal.hpp"          // IWYU pragma: associated
#include "ui/AccountListItem.hpp"  // IWYU pragma: associated

#include <memory>
#include <string>

#include "internal/api/client/Client.hpp"
#include "opentxs/api/Core.hpp"
#include "opentxs/api/Factory.hpp"
#include "opentxs/api/Wallet.hpp"
#include "opentxs/core/contract/UnitDefinition.hpp"
#include "opentxs/core/identifier/Server.hpp"
#include "opentxs/core/identifier/UnitDefinition.hpp"
#include "ui/Widget.hpp"

// #define OT_METHOD "opentxs::ui::implementation::AccountListItem::"

namespace opentxs::factory
{
auto AccountListItem(
    const ui::implementation::AccountListInternalInterface& parent,
    const api::client::internal::Manager& api,
    const network::zeromq::socket::Publish& publisher,
    const ui::implementation::AccountListRowID& rowID,
    const ui::implementation::AccountListSortKey& sortKey,
    const ui::implementation::CustomData& custom) noexcept
    -> std::shared_ptr<ui::implementation::AccountListRowInternal>
{
    using ReturnType = ui::implementation::AccountListItem;

    return std::make_shared<ReturnType>(
        parent, api, publisher, rowID, sortKey, custom);
}
}  // namespace opentxs::factory

namespace opentxs::ui::implementation
{
AccountListItem::AccountListItem(
    const AccountListInternalInterface& parent,
    const api::client::internal::Manager& api,
    const network::zeromq::socket::Publish& publisher,
    const AccountListRowID& rowID,
    const AccountListSortKey& sortKey,
    const CustomData& custom) noexcept
    : AccountListItemRow(parent, api, publisher, rowID, true)
    , type_(AccountType::Custodial)
    , unit_(sortKey.first)
    , balance_(extract_custom<Amount>(custom, 1))
    , contract_(load_unit(api_, extract_custom<OTUnitID>(custom, 2)))
    , notary_(load_server(api_, api.Factory().ServerID(sortKey.second)))
    , name_(extract_custom<std::string>(custom, 3))
{
}

auto AccountListItem::DisplayBalance() const noexcept -> std::string
{
    std::string output{};
    const auto formatted =
        contract_->FormatAmountLocale(balance_, output, ",", ".");

    if (formatted) { return output; }

    return std::to_string(balance_);
}

auto AccountListItem::load_server(
    const api::Core& api,
    const identifier::Server& id) -> OTServerContract
{
    try {

        return api.Wallet().Server(id);
    } catch (...) {

        return api.Factory().ServerContract();
    }
}

auto AccountListItem::load_unit(
    const api::Core& api,
    const identifier::UnitDefinition& id) -> OTUnitDefinition
{
    try {

        return api.Wallet().UnitDefinition(id);
    } catch (...) {

        return api.Factory().UnitDefinition();
    }
}

#if OT_QT
auto AccountListItem::qt_data(const int column, int role) const noexcept
    -> QVariant
{
    switch (role) {
        case AccountListQt::NotaryIDRole: {
            return NotaryID().c_str();
        }
        case AccountListQt::UnitRole: {
            return static_cast<int>(Unit());
        }
        case AccountListQt::AccountIDRole: {
            return AccountID().c_str();
        }
        case AccountListQt::BalanceRole: {
            return static_cast<unsigned long long>(Balance());
        }
        case AccountListQt::PolarityRole: {
            return polarity(Balance());
        }
        case AccountListQt::AccountTypeRole: {
            return static_cast<int>(Type());
        }
        case AccountListQt::ContractIdRole: {
            return ContractID().c_str();
        }
        case Qt::DisplayRole: {
            switch (column) {
                case AccountListQt::NotaryNameColumn: {
                    return NotaryName().c_str();
                }
                case AccountListQt::DisplayUnitColumn: {
                    return DisplayUnit().c_str();
                }
                case AccountListQt::AccountNameColumn: {
                    return Name().c_str();
                }
                case AccountListQt::DisplayBalanceColumn: {
                    return DisplayBalance().c_str();
                }
                default: {
                    [[fallthrough]];
                }
            }
        }
        default: {
        }
    }
    return {};
}
#endif
}  // namespace opentxs::ui::implementation
