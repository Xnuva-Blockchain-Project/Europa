# Europa (ERA) desktop miner wrapper

This directory preserves the corrected shell launcher used in the Europa desktop
mining bundle. It is separate from Europa Core's consensus implementation:
**no block validation, difficulty, chain history, network rules, or wallet
recovery data are modified.**

## Usage and deployment

Copy contrib/era-mining/miner.sh to the **root of the desktop bundle**,
alongside its bin/europa-cli executable, retaining the name miner.sh.
The script expects a running local Europa wallet/node, Bash, Python 3, and
RPC access to the user's ~/.europa data directory. It stores a mining reward
address locally in .europa-miner.address (do not commit that file).

Back up the existing desktop miner.sh before copying a new one. Do not
upload wallet data, RPC credentials, seed phrases, private keys, or local
runtime files to GitHub. A source commit is not a packaged wallet release.

The wrapper repeatedly requests one block from generatetoaddress with
up to 100,000 attempts per call. An empty result means no block was found
by that call. This wrapper does not independently attest peer propagation.

## 19 November 2026: false-positive mining success repair

### Confirmed original defect

The original miner compared the raw RPC result with the exact string "[]".
Europa RPC can return an empty array pretty-printed over two lines (opening
and closing brackets). As a result the script printed "ERA BLOCK FOUND"
despite receiving no block hash. These were **false positive status reports**.

### Correction

The corrected miner:
- Parses the JSON response and requires an array of valid 64-character hashes.
- Treats empty arrays and malformed responses as **not mined**.
- For returned hashes, queries getblockheader and checks for positive
  local active-chain confirmations before reporting
  "ERA BLOCK ACCEPTED LOCALLY".
- Explicitly distinguishes local acceptance from independent peer acceptance,
  reward maturity, and treasury accounting.

The development-machine deployed miner.sh SHA-256 was:

    abfbbba0c5786cf166dd72e40930c063403a2f64e7497df855cea90f0e731562

### Live evidence from ERA repair checkpoint 17

On the development machine on 19 November 2026:
- ERA locally advanced from block 13,886 to block 13,887.
- Locally accepted hash:
  ebe1202f93cfb01fa71a58bafd6c5cf371f0f3c173b4852d524107f91e07a60b
- The wallet recorded immature_balance of 10 ERA.
- Wallet txcount increased from 5,036 to 5,037.
- Historical inactive_mined_balance remained 49,360 ERA.

**This verifies local block acceptance and immature wallet credit only.**
The following remain release-blocking: another independent ERA node verifying
the same block hash and accepting subsequent blocks; peer propagation;
coinbase maturity/spendable reward; wallet/node restart recovery; sustained
operation; and game-treasury integration where applicable. No ERA release
qualification is claimed.

## Regression tests

From the root of the repository, run:

    bash -n contrib/era-mining/miner.sh
    python3 contrib/era-mining/tests/test_miner_rpc_json.py

The offline tests extract and run the actual embedded JSON parsers. They
verify compact and multiline empty arrays, returned hashes, malformed output,
and acceptance/rejection of active vs inactive header confirmations without
contacting the blockchain. Real-network end-to-end qualification remains
separate and mandatory.

**Reliability first, release second.**
