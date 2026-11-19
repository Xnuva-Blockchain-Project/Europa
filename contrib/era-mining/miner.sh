#!/usr/bin/env bash

set +e
set +u
set +o pipefail

BASE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
CLI="$BASE/bin/europa-cli"
DATADIR="$HOME/.europa"

ADDRFILE="$BASE/.europa-miner.address"

COIN="ERA"
FULLNAME="EUROPA"

separator()
{
    echo "============================================================"
}

pause_close()
{
    echo
    read -r -p "Press Enter to close this window..." _
}

fatal()
{
    echo
    separator
    echo "              ERA MINER COULD NOT START"
    separator
    echo
    echo "STATUS: NOT MINING"
    echo
    printf '%s\n' "$1"

    if [ -n "${2:-}" ]; then
        echo
        echo "Technical details:"
        printf '%s\n' "$2"
    fi

    echo
    echo "Nothing is currently mining from this terminal."
    pause_close
    exit 1
}

stop_miner()
{
    trap - INT TERM

    echo
    separator
    echo "                 STOPPING ERA MINER"
    separator
    echo
    echo "Stopping mining..."
    echo
    separator
    echo "                  ERA MINER STOPPED"
    separator
    echo
    echo "STATUS: MINER STOPPED"
    echo
    echo "ERA mining has stopped successfully."
    echo
    echo "No ERA miner is running from this terminal."
    echo
    echo "It is now safe to close this window."
    pause_close
    exit 0
}

trap stop_miner INT TERM

separator
echo "                 ERA / EUROPA MINER"
separator
echo
echo "This terminal window runs the ERA miner."
echo
echo "KEEP THIS WINDOW OPEN while you want to mine ERA."
echo
echo "TO STOP MINING SAFELY:"
echo
echo "    1. Press Ctrl+C once."
echo "    2. Wait for \"ERA MINER STOPPED\"."
echo "    3. Press Enter to close this window."
echo
echo "Do not type normal shell commands into this terminal."
echo
echo "Closing this terminal window will also stop this miner."
echo
separator
echo
echo "STATUS: STARTING"

if [ ! -x "$CLI" ]; then
    fatal \
        "The Europa command-line program could not be found." \
        "$CLI"
fi

rpc()
{
    "$CLI" -datadir="$DATADIR" "$@"
}

if ! rpc getblockcount >/dev/null 2>&1; then
    fatal \
        "The Europa wallet/node does not appear to be running. Start the Europa wallet or node first, then run the miner again."
fi

if [ -s "$ADDRFILE" ]; then
    ADDRESS="$(cat "$ADDRFILE")"
else
    ADDRESS="$(rpc getnewaddress 2>&1)"
    RC=$?

    if [ "$RC" -ne 0 ] || [ -z "$ADDRESS" ]; then
        fatal \
            "Europa could not obtain a mining reward address from the wallet." \
            "$ADDRESS"
    fi

    printf '%s\n' "$ADDRESS" > "$ADDRFILE"
fi

echo
echo "Mining reward address:"
echo "  $ADDRESS"
echo
separator
echo "STATUS: MINING ACTIVE"
separator
echo
echo "ERA is now mining."
echo
echo "Press Ctrl+C once when you want to stop."
echo

ATTEMPTS=0

while :
do
    ATTEMPTS=$((ATTEMPTS + 1))

    RESULT="$(rpc generatetoaddress 1 "$ADDRESS" 100000 2>&1)"
    RC=$?

    if [ "$RC" -ne 0 ]; then
        echo
        echo "WARNING: A mining attempt returned an error."
        echo "The miner will retry automatically in 2 seconds."
        echo
        echo "Details:"
        printf '%s\n' "$RESULT"
        echo
        sleep 2
        continue
    fi

    # generatetoaddress returns a JSON array of mined block hashes.
    # An empty array may be pretty-printed as "[\n]", not just "[]".
    # Parse the JSON rather than comparing the raw text.
    BLOCK_HASHES="$(printf '%s\n' "$RESULT" | python3 -c '
import json, re, sys
try:
    value = json.load(sys.stdin)
    if not isinstance(value, list) or not all(
        isinstance(item, str) and re.fullmatch(r"[0-9a-fA-F]{64}", item)
        for item in value
    ):
        raise ValueError("not an array of block hashes")
    print("\n".join(value))
except (ValueError, json.JSONDecodeError):
    sys.exit(1)
' 2>/dev/null)"
    PARSE_RC=$?

    if [ "$PARSE_RC" -ne 0 ]; then
        echo "WARNING: Unexpected generatetoaddress response; not claiming a block."
        printf '%s\n' "$RESULT"
    elif [ -n "$BLOCK_HASHES" ]; then
        while IFS= read -r HASH; do
            [ -n "$HASH" ] || continue
            HEADER="$(rpc getblockheader "$HASH" 2>&1)"
            HEADER_RC=$?

            if [ "$HEADER_RC" -ne 0 ]; then
                echo "WARNING: Mining RPC returned $HASH, but header lookup failed."
                printf '%s\n' "$HEADER"
                continue
            fi

            # A positive confirmation count means the block is on this
            # node's active chain. It is NOT independent peer acceptance.
            HEADER_STATUS="$(printf '%s\n' "$HEADER" | python3 -c '
import json, sys
try:
    header = json.load(sys.stdin)
    height = header.get("height")
    confs = header.get("confirmations")
    if isinstance(height, int) and isinstance(confs, int) and confs > 0:
        print("%d %d" % (height, confs))
    else:
        sys.exit(1)
except (ValueError, AttributeError, TypeError, json.JSONDecodeError):
    sys.exit(1)
' 2>/dev/null)"

            if [ -n "$HEADER_STATUS" ]; then
                echo
                separator
                echo "          ERA BLOCK ACCEPTED LOCALLY"
                separator
                echo "Block hash: $HASH"
                echo "Height / confirmations: $HEADER_STATUS"
                echo "NOTE: Independent peer acceptance and wallet credit unverified."
                echo
            else
                echo "WARNING: Mining RPC returned $HASH but active-chain acceptance is unconfirmed."
            fi
        done <<< "$BLOCK_HASHES"
    fi

    if [ $((ATTEMPTS % 20)) -eq 0 ]; then
        echo "STATUS: MINING ACTIVE — attempts completed: $ATTEMPTS"
    fi
done
