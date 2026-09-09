#!/usr/bin/env bash
set -euo pipefail

# Materialise the current untested Europa-on-Bitcoin-Core-31.1 draft.
# DO NOT DEPLOY OR CONNECT THIS TREE TO THE LIVE EUROPA NETWORK.

EXPECTED_UPSTREAM="9be056a8a72b624dae9623b2f7bded92c2a21c91"
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

cat <<EOF
Europa modern draft materialised at:
  $DEST

Baseline:
  Bitcoin Core v31.1 $EXPECTED_UPSTREAM

Overlay:
  migration/modern-europa-draft/

Status:
  UNTESTED / DO NOT RELEASE / DO NOT CONNECT TO LIVE EUROPA

This command creates a working source tree only. It does not alter Europa main,
does not launch a node, and does not create a blockchain.
EOF
