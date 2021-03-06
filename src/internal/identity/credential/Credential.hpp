// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "opentxs/identity/credential/Base.hpp"
#include "opentxs/identity/credential/Contact.hpp"
#include "opentxs/identity/credential/Key.hpp"
#include "opentxs/identity/credential/Primary.hpp"
#include "opentxs/identity/credential/Secondary.hpp"
#include "opentxs/identity/credential/Verification.hpp"

namespace opentxs::identity::credential::internal
{
struct Base : virtual public identity::credential::Base {
    virtual void ReleaseSignatures(const bool onlyPrivate) = 0;

#ifdef _MSC_VER
    Base() {}
#endif  // _MSC_VER
    virtual ~Base() = default;
};
struct Contact : virtual public Base,
                 virtual public identity::credential::Contact {

#ifdef _MSC_VER
    Contact() {}
#endif  // _MSC_VER
    virtual ~Contact() = default;
};
struct Key : virtual public Base, virtual public identity::credential::Key {
    virtual bool SelfSign(
        const PasswordPrompt& reason,
        const OTPassword* exportPassword = nullptr,
        const bool onlyPrivate = false) = 0;

#ifdef _MSC_VER
    Key() {}
#endif  // _MSC_VER
    virtual ~Key() = default;
};
struct Primary : virtual public Key,
                 virtual public identity::credential::Primary {

#ifdef _MSC_VER
    Primary() {}
#endif  // _MSC_VER
    virtual ~Primary() = default;
};
struct Secondary : virtual public Key,
                   virtual public identity::credential::Secondary {

#ifdef _MSC_VER
    Secondary() {}
#endif  // _MSC_VER
    virtual ~Secondary() = default;
};
struct Verification : virtual public Base,
                      virtual public identity::credential::Verification {
#ifdef _MSC_VER
    Verification() {}
#endif  // _MSC_VER
    virtual ~Verification() = default;
};
}  // namespace opentxs::identity::credential::internal
