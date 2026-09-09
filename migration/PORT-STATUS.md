# Europa Bitcoin Core 31.1 modernisation status

**Branch:** `modernisation/bitcoin-core-31.1-experimental-do-not-release`  
**Consensus:** Proof of Work only  
**Release status:** **UNTESTED / DO NOT RELEASE**

## Completed

- Authoritative source fixed to Europa `main` at `233946e88dfa7a70b509b8c7dfa6e2c29bd0fc65`.
- Confirmed there is no PoS/staking consensus layer.
- Bitcoin Core v31.1 pinned at `9be056a8a72b624dae9623b2f7bded92c2a21c91`.
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

## Mechanical port sequence

1. Preserve Europa genesis and network identity.
2. Preserve double-SHA256 block IDs while using Scrypt for PoW validation.
3. Preserve Europa retarget arithmetic and 300s/600s timing.
4. Preserve the 10 ERA / 210,000-block halving schedule and 4.2m money-range rule.
5. Preserve existing script/BIP/SegWit activation state and disable unhistorical Taproot activation.
6. Preserve main/test/regtest ports and address formats, including the secondary script prefix.
7. Port modern mining/wallet/RPC/GUI around the PoW-only core.
8. Complete legacy test/regtest versionbits and seed-format adapters.
9. Complete modern wallet/mining/RPC/branding integration.
10. Build.
11. Validate the existing chain from genesis to live tip.
12. Only after compatibility proof consider any non-consensus cleanup.

## Hard gate

Nothing from this branch may be merged to `main`, released, or used as a live authoritative node until historical-chain compatibility is proven.
