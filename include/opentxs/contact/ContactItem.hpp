// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef OPENTXS_CONTACT_CONTACTITEM_HPP
#define OPENTXS_CONTACT_CONTACTITEM_HPP

#define NULL_START 0
#define NULL_END 0

// IWYU pragma: no_include "opentxs/Proto.hpp"

#include "opentxs/Forward.hpp"  // IWYU pragma: associated

#include <chrono>
#include <cstdint>
#include <ctime>
#include <set>
#include <string>

#include "opentxs/Proto.hpp"
#include "opentxs/Types.hpp"
#include "opentxs/core/Identifier.hpp"
#include "opentxs/protobuf/ContactEnums.pb.h"

namespace opentxs
{
namespace api
{
namespace internal
{
struct Core;
}  // namespace internal
}  // namespace api
}  // namespace opentxs

namespace opentxs
{
class ContactItem
{
public:
    OPENTXS_EXPORT ContactItem(
        const api::internal::Core& api,
        const std::string& nym,
        const VersionNumber version,
        const VersionNumber parentVersion,
        const proto::ContactSectionName section,
        const proto::ContactItemType& type,
        const std::string& value,
        const std::set<proto::ContactItemAttribute>& attributes,
        const std::time_t start,
        const std::time_t end,
        const std::string subtype);
    OPENTXS_EXPORT ContactItem(
        const api::internal::Core& api,
        const std::string& nym,
        const VersionNumber version,
        const VersionNumber parentVersion,
        const Claim& claim);
    OPENTXS_EXPORT ContactItem(
        const api::internal::Core& api,
        const std::string& nym,
        const VersionNumber parentVersion,
        const proto::ContactSectionName section,
        const proto::ContactItem& serialized);
    OPENTXS_EXPORT ContactItem(const ContactItem&);

    OPENTXS_EXPORT bool operator==(const ContactItem& rhs) const;

    // Includes IDs
    OPENTXS_EXPORT operator proto::ContactItem() const;

    OPENTXS_EXPORT const std::time_t& End() const;
    OPENTXS_EXPORT const Identifier& ID() const;
    OPENTXS_EXPORT bool isActive() const;
    OPENTXS_EXPORT bool isLocal() const;
    OPENTXS_EXPORT bool isPrimary() const;
    OPENTXS_EXPORT const proto::ContactSectionName& Section() const;
    OPENTXS_EXPORT proto::ContactItem Serialize(
        const bool withID = false) const;
    OPENTXS_EXPORT ContactItem SetActive(const bool active) const;
    OPENTXS_EXPORT ContactItem SetEnd(const std::time_t end) const;
    OPENTXS_EXPORT ContactItem SetLocal(const bool local) const;
    OPENTXS_EXPORT ContactItem SetPrimary(const bool primary) const;
    OPENTXS_EXPORT ContactItem SetStart(const std::time_t start) const;
    OPENTXS_EXPORT ContactItem SetValue(const std::string& value) const;
    OPENTXS_EXPORT const std::time_t& Start() const;
    OPENTXS_EXPORT const std::string& Subtype() const;
    OPENTXS_EXPORT const proto::ContactItemType& Type() const;
    OPENTXS_EXPORT const std::string& Value() const;
    OPENTXS_EXPORT VersionNumber Version() const;

    OPENTXS_EXPORT ~ContactItem() = default;

private:
    const api::internal::Core& api_;
    const VersionNumber version_{0};
    const std::string nym_{};
    const proto::ContactSectionName section_{proto::CONTACTSECTION_ERROR};
    const proto::ContactItemType type_{proto::CITEMTYPE_ERROR};
    const std::string value_;
    const std::time_t start_{0};
    const std::time_t end_{0};
    const std::set<proto::ContactItemAttribute> attributes_{};
    const OTIdentifier id_;
    const std::string subtype_;

    static VersionNumber check_version(
        const VersionNumber in,
        const VersionNumber targetVersion);
    static std::set<proto::ContactItemAttribute> extract_attributes(
        const proto::ContactItem& serialized);
    static std::set<proto::ContactItemAttribute> extract_attributes(
        const Claim& claim);

    ContactItem set_attribute(
        const proto::ContactItemAttribute& attribute,
        const bool value) const;

    ContactItem() = delete;
    ContactItem(ContactItem&&) = delete;
    ContactItem& operator=(const ContactItem&) = delete;
    ContactItem& operator=(ContactItem&&) = delete;
};
}  // namespace opentxs

#endif
