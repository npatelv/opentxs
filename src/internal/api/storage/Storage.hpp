// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "opentxs/api/storage/Storage.hpp"

namespace opentxs::api::storage
{
class StorageInternal : virtual public Storage
{
public:
    virtual void InitBackup() = 0;
    virtual void InitEncryptedBackup(opentxs::crypto::key::Symmetric& key) = 0;
    virtual void start() = 0;

    virtual ~StorageInternal() override = default;

protected:
    StorageInternal() = default;

private:
    StorageInternal(const StorageInternal&) = delete;
    StorageInternal(StorageInternal&&) = delete;
    StorageInternal& operator=(const StorageInternal&) = delete;
    StorageInternal& operator=(StorageInternal&&) = delete;
};
}  // namespace opentxs::api::storage
