#ifndef EUROPA_CHAIN_IDENTITY_SECURITY_H
#define EUROPA_CHAIN_IDENTITY_SECURITY_H

#include <string_view>

namespace europa::security {

inline constexpr std::string_view MAINNET_GENESIS = "3371d05fac15f3aa5ceb494538e736ea3b9fcb5c78613ef57c0abe3205ac44a3";
inline constexpr std::string_view MAINNET_BLOCK1 = "b4bfd0c47191b145e6a74fedc9398cf3b08102418fc23639cca263bb533104e7";
inline constexpr int MAINNET_ANCHOR_HEIGHT = 12000;
inline constexpr std::string_view MAINNET_ANCHOR_HASH = "dd212cbd9315c45328e903bfa89e014878b09b2f6f16cff2cb3e9a24612715f1";
inline constexpr std::string_view MAINNET_MINIMUM_CHAINWORK = "000000000000000000000000000000000000000000000000000046e276e046d1";

inline constexpr bool REQUIRE_SYNCED = true;
inline constexpr bool REQUIRE_PEER = true;
inline constexpr bool PROTECT_POW = true;
inline constexpr bool PROTECT_POS = false;
inline constexpr bool VALUES_QUALIFIED = true;
inline constexpr bool RUNTIME_GUARD_INTEGRATED = false;

constexpr bool IdentityValuesComplete()
{
    return VALUES_QUALIFIED &&
           MAINNET_GENESIS.size() == 64 &&
           MAINNET_BLOCK1.size() == 64 &&
           MAINNET_ANCHOR_HEIGHT == 12000 &&
           MAINNET_ANCHOR_HASH.size() == 64 &&
           MAINNET_MINIMUM_CHAINWORK.size() == 64;
}

static_assert(IdentityValuesComplete());

} // namespace europa::security

#endif
