#!/usr/bin/env bash
set -euo pipefail

# Materialise the current untested Europa-on-Bitcoin-Core-31.1 draft.
# DO NOT DEPLOY OR CONNECT THIS TREE TO THE LIVE EUROPA NETWORK.

EXPECTED_UPSTREAM="9be056a8a72b624dae9623b2f7bded92c2a21c91"
HARDENED_MAIN="7e1fdabb11dda86f89a13e147d38a1d55a2b0419"
DEST="${1:-modern-europa-worktree}"

ROOT="$(git rev-parse --show-toplevel)"
cd "$ROOT"

if [[ -e "$DEST" ]]; then
    echo "STOP: destination already exists: $DEST" >&2
    exit 1
fi

git submodule update --init --recursive modern-core
actual="$(git -C modern-core rev-parse HEAD)"
if [[ "$actual" != "$EXPECTED_UPSTREAM" ]]; then
    echo "STOP: modern-core is not the pinned Bitcoin Core v31.1 commit" >&2
    echo "expected: $EXPECTED_UPSTREAM" >&2
    echo "actual:   $actual" >&2
    exit 1
fi

mkdir -p "$DEST"
cp -a modern-core/. "$DEST"/
cp -a migration/modern-europa-draft/. "$DEST"/

# The migration overlay predates the qualified ERA chain-identity incident
# hardening. Replace only the exact old mainnet chainwork/assumevalid pair in
# the materialised tree. Testnet/regtest are intentionally untouched.
python3 - "$DEST/src/kernel/chainparams.cpp" <<'PY'
from pathlib import Path
import sys

p = Path(sys.argv[1])
s = p.read_text()

old = '''        consensus.nMinimumChainWork =
            uint256{"0000000000000000000000000000000000000000000000000000000000200020"};
        consensus.defaultAssumeValid =
            uint256{"3371d05fac15f3aa5ceb494538e736ea3b9fcb5c78613ef57c0abe3205ac44a3"};'''

new = '''        // Qualified recognised ERA chain identity at buried height 12000.
        consensus.nMinimumChainWork =
            uint256{"000000000000000000000000000000000000000000000000000046e276e046d1"};
        consensus.defaultAssumeValid =
            uint256{"dd212cbd9315c45328e903bfa89e014878b09b2f6f16cff2cb3e9a24612715f1"};'''

if s.count(old) != 1:
    raise SystemExit("STOP: expected exactly one old ERA mainnet security pair")

p.write_text(s.replace(old, new, 1))
print("PASS: qualified ERA height-12000 chainwork/assumevalid applied")
PY

SECURITY_HEADER="$DEST/src/europa/chain_identity_security.h"
[[ -f "$SECURITY_HEADER" ]] || {
    echo "STOP: ERA chain-identity security policy missing from materialised tree" >&2
    exit 1
}

grep -Fq 'MAINNET_ANCHOR_HEIGHT = 12000' "$SECURITY_HEADER" || {
    echo "STOP: ERA buried-anchor policy not present" >&2
    exit 1
}

grep -Fq 'dd212cbd9315c45328e903bfa89e014878b09b2f6f16cff2cb3e9a24612715f1' "$DEST/src/kernel/chainparams.cpp" || {
    echo "STOP: qualified ERA anchor missing from materialised chainparams" >&2
    exit 1
}

cat <<EOF
Europa modern draft materialised at:
  $DEST

Baseline:
  Bitcoin Core v31.1 $EXPECTED_UPSTREAM

Overlay:
  migration/modern-europa-draft/

ERA security inheritance:
  hardened legacy main $HARDENED_MAIN
  canonical block 1 b4bfd0c47191b145e6a74fedc9398cf3b08102418fc23639cca263bb533104e7
  buried anchor height 12000
  buried anchor dd212cbd9315c45328e903bfa89e014878b09b2f6f16cff2cb3e9a24612715f1
  minimum chainwork 000000000000000000000000000000000000000000000000000046e276e046d1

Status:
  UNTESTED / DO NOT RELEASE / DO NOT CONNECT TO LIVE EUROPA
  RUNTIME BLOCK-PRODUCTION GUARD STILL MUST BE INTEGRATED AND QUALIFIED

This command creates a working source tree only. It does not alter Europa main,
does not launch a node, and does not create a blockchain.
EOF
