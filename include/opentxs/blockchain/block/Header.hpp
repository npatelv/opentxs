// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef OPENTXS_BLOCKCHAIN_BLOCK_HEADER_HPP
#define OPENTXS_BLOCKCHAIN_BLOCK_HEADER_HPP

// IWYU pragma: no_include "opentxs/Proto.hpp"

#include "opentxs/Forward.hpp"  // IWYU pragma: associated

#include <cstdint>

#include "opentxs/blockchain/Blockchain.hpp"
#include "opentxs/blockchain/NumericHash.hpp"
#include "opentxs/blockchain/Work.hpp"
#include "opentxs/Proto.hpp"

namespace opentxs
{
namespace blockchain
{
namespace block
{
class Header
{
public:
    using SerializedType = proto::BlockchainBlockHeader;

    enum class Status : std::uint32_t {
        Error,
        Normal,
        Disconnected,
        CheckpointBanned,
        Checkpoint
    };

    OPENTXS_EXPORT virtual std::unique_ptr<Header> clone() const noexcept = 0;
    OPENTXS_EXPORT virtual OTWork Difficulty() const noexcept = 0;
    OPENTXS_EXPORT virtual Status EffectiveState() const noexcept = 0;
    OPENTXS_EXPORT virtual const block::Hash& Hash() const noexcept = 0;
    OPENTXS_EXPORT virtual block::Height Height() const noexcept = 0;
    OPENTXS_EXPORT virtual Status InheritedState() const noexcept = 0;
    OPENTXS_EXPORT virtual bool IsBlacklisted() const noexcept = 0;
    OPENTXS_EXPORT virtual bool IsDisconnected() const noexcept = 0;
    OPENTXS_EXPORT virtual Status LocalState() const noexcept = 0;
    OPENTXS_EXPORT virtual OTNumericHash NumericHash() const noexcept = 0;
    OPENTXS_EXPORT virtual const block::Hash& ParentHash() const noexcept = 0;
    OPENTXS_EXPORT virtual OTWork ParentWork() const noexcept = 0;
    OPENTXS_EXPORT virtual block::Position Position() const noexcept = 0;
    OPENTXS_EXPORT virtual SerializedType Serialize() const noexcept = 0;
    OPENTXS_EXPORT virtual OTNumericHash Target() const noexcept = 0;
    OPENTXS_EXPORT virtual blockchain::Type Type() const noexcept = 0;
    OPENTXS_EXPORT virtual bool Valid() const noexcept = 0;
    OPENTXS_EXPORT virtual OTWork Work() const noexcept = 0;

    OPENTXS_EXPORT virtual void CompareToCheckpoint(
        const block::Position& checkpoint) noexcept = 0;
    /// Throws std::runtime_error if parent hash incorrect
    OPENTXS_EXPORT virtual void InheritHeight(const Header& parent) = 0;
    /// Throws std::runtime_error if parent hash incorrect
    OPENTXS_EXPORT virtual void InheritState(const Header& parent) = 0;
    OPENTXS_EXPORT virtual void InheritWork(
        const blockchain::Work& parent) noexcept = 0;
    OPENTXS_EXPORT virtual void RemoveBlacklistState() noexcept = 0;
    OPENTXS_EXPORT virtual void RemoveCheckpointState() noexcept = 0;
    OPENTXS_EXPORT virtual void SetDisconnectedState() noexcept = 0;

    OPENTXS_EXPORT virtual ~Header() = default;

protected:
    Header() noexcept = default;

private:
    Header(const Header&) = delete;
    Header(Header&&) = delete;
    Header& operator=(const Header&) = delete;
    Header& operator=(Header&&) = delete;
};
}  // namespace block
}  // namespace blockchain
}  // namespace opentxs
#endif
