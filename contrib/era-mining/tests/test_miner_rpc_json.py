#!/usr/bin/env python3
"""Offline regression tests of the actual embedded JSON parsers in miner.sh.

No real Europa Core, blockchain, wallet or network is contacted.
"""

import json
import pathlib
import subprocess
import sys
import unittest

MINER = pathlib.Path(__file__).resolve().parents[1] / "miner.sh"
BLOCK_HASH = "ebe1202f93cfb01fa71a58bafd6c5cf371f0f3c173b4852d524107f91e07a60b"


def parser_after(anchor):
    """Extract the real inline python3 -c parser following the anchor."""
    shell = MINER.read_text(encoding="utf-8")
    start = shell.index(anchor)
    start = shell.index("python3 -c '\n", start) + len("python3 -c '\n")
    end = shell.index("\n' 2>/dev/null)", start)
    return shell[start:end]


def run_parser(anchor, data):
    return subprocess.run(
        [sys.executable, "-c", parser_after(anchor)],
        input=data, text=True, capture_output=True, check=False,
    )


class MiningJsonTest(unittest.TestCase):
    def test_empty_array_compact(self):
        p = run_parser('BLOCK_HASHES=', '[]')
        self.assertEqual((p.returncode, p.stdout.strip()), (0, ''))

    def test_empty_array_multiline(self):
        p = run_parser('BLOCK_HASHES=', '[\n]')
        self.assertEqual((p.returncode, p.stdout.strip()), (0, ''))

    def test_valid_block_hash(self):
        p = run_parser('BLOCK_HASHES=', json.dumps([BLOCK_HASH]))
        self.assertEqual((p.returncode, p.stdout.strip()), (0, BLOCK_HASH))

    def test_invalid_block_output_rejected(self):
        for value in ('not-json', '"false"', '["not-a-hash"]'):
            with self.subTest(value=value):
                p = run_parser('BLOCK_HASHES=', value)
                self.assertNotEqual(p.returncode, 0)

    def test_header_active_chain_confirmation(self):
        p = run_parser('HEADER_STATUS=', json.dumps({'height': 13887, 'confirmations': 1}))
        self.assertEqual((p.returncode, p.stdout.strip()), (0, '13887 1'))

    def test_header_inactive_chain_rejected(self):
        p = run_parser('HEADER_STATUS=', json.dumps({'height': 13887, 'confirmations': -1}))
        self.assertNotEqual(p.returncode, 0)


if __name__ == '__main__':
    unittest.main()
