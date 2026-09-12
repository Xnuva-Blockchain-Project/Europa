# Europa Bitcoin Core 31.1 modernisation status

**Branch:** `modernisation/bitcoin-core-31.1-experimental-do-not-release`  
**Consensus:** Proof of Work only  
**Release status:** **UNTESTED / DO NOT RELEASE**

## Baselines

- Original migration source baseline: Europa `main` at `233946e88dfa7a70b509b8c7dfa6e2c29bd0fc65`.
- Current authoritative/hardened legacy security baseline: `7e1fdabb11dda86f89a13e147d38a1d55a2b0419`.
- Bitcoin Core v31.1 baseline: `9be056a8a72b624dae9623b2f7bded92c2a21c91`.

The modernisation must preserve historical consensus compatibility **and** inherit the chain-identity protection qualified after the original migration baseline.

## Completed

- Confirmed there is no PoS/staking consensus layer.
- Full Bitcoin Core v31.1 tree available as the pinned `modern-core` submodule.
- Europa consensus-preservation manifest recorded.
- No monetary or consensus correction is being mixed into the migration.
- Snapshotted the authoritative consensus/address/mining source under `migration/legacy-consensus-snapshot/`.
- Began the actual Bitcoin Core 31.1 overlay under `migration/modern-europa-draft/`.
- Drafted modern block primitives preserving standard block IDs plus Europa Scrypt PoW.
- Ported Europa's retarget arithmetic, including the historical first-retarget walk-back and overflow guard.
- Ported `MAX_MONEY = 4,200,000 ERA` and the 10 ERA / 210,000-block halving schedule.
- Drafted main/test/regtest genesis, magic, RPC/P2P ports, PoW timing and address identity.
- Preserved mainnet CSV/SegWit historical activation and disabled unhistorical Taproot activation.
- Preserved legacy ERA script-address and pre-Bech32m witness-address compatibility.
- Drafted Bitcoin Core 31.1 validation integration so PoW checks use Scrypt while block IDs remain normal serialized hashes.
- Wired the Europa Scrypt/subsidy modules into a draft Bitcoin Core 31.1 CMake source list.
- Added a deterministic materialiser for producing a separate modern Europa working tree without launching it.
- Added `migration/CHAIN-IDENTITY-SECURITY.md` and qualified chain-identity constants in `migration/modern-europa-draft/src/europa/chain_identity_security.h`.
- Carried the recognised ERA identity into the modernisation: canonical block 1, buried height-12000 anchor and exact minimum chainwork.
- Updated the materialiser so the generated modern tree replaces the pre-hardening mainnet `nMinimumChainWork` / `defaultAssumeValid` pair with the qualified height-12000 values while leaving testnet/regtest untouched.

## Qualified ERA security identity

- Genesis: `3371d05fac15f3aa5ceb494538e736ea3b9fcb5c78613ef57c0abe3205ac44a3`
- Canonical block 1: `b4bfd0c47191b145e6a74fedc9398cf3b08102418fc23639cca263bb533104e7`
- Buried anchor height: `12000`
- Buried anchor hash: `dd212cbd9315c45328e903bfa89e014878b09b2f6f16cff2cb3e9a24612715f1`
- Minimum chainwork: `000000000000000000000000000000000000000000000000000046e276e046d1`

These values are already qualified from the recognised live ERA chain and the reconstructed 2024 alternative-chain tests; they are not placeholders.

## Mechanical port sequence

1. Preserve Europa genesis and network identity.
2. Preserve double-SHA256 block IDs while using Scrypt for PoW validation.
3. Preserve Europa retarget arithmetic and 300s/600s timing.
4. Preserve the 10 ERA / 210,000-block halving schedule and 4.2m money-range rule.
5. Preserve existing script/BIP/SegWit activation state and disable unhistorical Taproot activation.
6. Preserve main/test/regtest ports and address formats, including the secondary script prefix.
7. Port modern mining/wallet/RPC/GUI around the PoW-only core.
8. Integrate the mainnet-only chain-identity guard into the **central modern block-building path** so all block-template/mining surfaces inherit it.
9. Complete legacy test/regtest versionbits and seed-format adapters.
10. Complete modern wallet/mining/RPC/branding integration.
11. Build.
12. Validate the existing chain from genesis to live tip.
13. Negative security test: reconstructed 2024 alternative chain must be unable to create a mainnet block candidate/template and its tip must remain unchanged.
14. Positive security test: recognised chain must pass block 1 / height 12000 / chainwork / sync / peer gates and create a valid next template without PoW or submission.
15. Only after compatibility and both security paths pass consider any non-consensus cleanup or release.

## Hard gate

Nothing from this branch may be merged to `main`, released, or used as a live authoritative node until historical-chain compatibility is proven **and** the runtime central block-production guard consumes the qualified policy and passes both the negative and positive ERA security tests.

The policy constants and materialised chain parameters are now present, but `RUNTIME_GUARD_INTEGRATED` remains deliberately false until the modern mining/block-template path is actually ported and wired to the policy.
