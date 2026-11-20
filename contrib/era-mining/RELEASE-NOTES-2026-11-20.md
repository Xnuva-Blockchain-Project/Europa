# Europa (ERA) desktop miner — repair notes (20 November 2026)

**Status: source-script repair available for testing; not a fully qualified Europa Core or wallet release.**

## Scope

The corrected `contrib/era-mining/miner.sh` repairs false reports of successful mining. This is a **launcher-only** change: no consensus rules, chain history, difficulty, address format, wallet data, or Europa Core binary is changed. It is not an upgrade to the Windows recovery release.

## Root cause

The previous launcher compared a raw `generatetoaddress` response with the exact text `[]`. The RPC server can return an empty JSON array formatted across multiple lines (`[\n]`). The old comparison treated that response as a found block despite receiving no block hash.

## Fix

- Parse RPC output as JSON, accepting only arrays containing valid 64-character hexadecimal block hashes.
- Interpret empty arrays as **no block found**, regardless of formatting.
- Reject malformed replies instead of claiming success.
- For each reported block hash, query `getblockheader` and require **positive local active-chain confirmations** before printing `ERA BLOCK ACCEPTED LOCALLY`.
- Explicitly distinguish local acceptance from independent network acceptance, wallet reward maturity, and game-treasury accounting.

## Usage

This is a source-script update for the **Linux desktop mining bundle**. It is not a self-contained installer.

1. Back up the existing `miner.sh` in your desktop bundle.
2. Copy `contrib/era-mining/miner.sh` to the root of that bundle, alongside the `bin/` directory containing `europa-cli`.
3. Have a running, synced local Europa wallet/node with RPC available through `~/.europa`. Bash and Python 3 are required.
4. Launch `./miner.sh`; keep the mining terminal open. Press Ctrl+C to request a clean stop.
5. The script stores a reward address in `.europa-miner.address` next to itself; preserve this file locally and do not commit it to GitHub.

The launcher repeatedly calls `generatetoaddress 1 <address> 100000`. It does not start the node, prove peer propagation, or guarantee reward maturity. Never expose RPC credentials or private wallet files.

## Evidence — 19 November 2026

- Desktop miner script SHA-256 recorded during development: `abfbbba0c5786cf166dd72e40930c063403a2f64e7497df855cea90f0e731562`.
- The development node advanced from block 13,886 to 13,887.
- Locally accepted block: `ebe1202f93cfb01fa71a58bafd6c5cf371f0f3c173b4852d524107f91e07a60b`.
- The local wallet showed **10 immature ERA**; transaction count increased **5,036 → 5,037**.
- Historical inactive mined balance remained 49,360 ERA.

These observations support **local** block acceptance and immature wallet credit, not unconditional network-wide acceptance.

## Outstanding qualification

Independent peer verification of the specific block, subsequent block acceptance, reward maturity, restart recovery, sustained operation, and game-treasury integration where applicable are **not yet qualified**. Do not represent this source-only script repair as a complete production wallet/miner release.

## Maintainer regression checks

```sh
bash -n contrib/era-mining/miner.sh
python3 contrib/era-mining/tests/test_miner_rpc_json.py
```

These offline checks are separate from live network verification. See `contrib/era-mining/README.md` for engineering context.

**Reliability first, release second.**
