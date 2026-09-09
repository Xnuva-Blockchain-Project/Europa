# Modern Europa (ERA) draft source

**UNTESTED — DO NOT RELEASE — DO NOT CONNECT TO LIVE EUROPA**

This directory is the mechanical Bitcoin Core 31.1 migration overlay for the **existing** Europa blockchain.

It is intentionally separate from:

- authoritative Europa `main` at `233946e88dfa7a70b509b8c7dfa6e2c29bd0fc65`; and
- the pristine pinned Bitcoin Core v31.1 `modern-core` submodule at `9be056a8a72b624dae9623b2f7bded92c2a21c91`.

## Consensus model

Europa is **Proof of Work only**. Do not introduce staking, coinstake, PoS rewards, stake modifiers, or PoS block signatures.

## Drafted so far

- modern Bitcoin Core 31.1 block primitives with Europa `GetPoWHash()`;
- standard serialized double-SHA256 block IDs preserved;
- Scrypt `1024_1_1_256` PoW using Bitcoin Core's internal SHA-256 implementation;
- Europa-specific retarget arithmetic and first-retarget behaviour;
- 10 ERA subsidy with 210,000-block halving schedule;
- `MAX_MONEY = 4,200,000 ERA`;
- main/test/regtest genesis, magic, ports, timing and address parameters;
- mainnet CSV and SegWit historical activation represented as active from genesis;
- Taproot disabled for the migration because it was not activated by the authoritative Europa source;
- legacy test/regtest CSV/SegWit BIP9 parameters retained for an explicit deployment-state adapter;
- legacy script-address compatibility: encode with prefix 33, decode prefixes 5 and 33;
- legacy pre-Bech32m v1+ witness-address compatibility;
- Bitcoin Core 31.1 validation draft changed to validate Scrypt PoW while retaining normal block IDs;
- CMake source wiring for the Europa Scrypt and subsidy modules.

## Still to port before build/testing

- legacy testnet/regtest versionbits state adapter;
- fixed seed conversion into current Bitcoin Core seed format;
- final mining/RPC/wallet integration review;
- Europa executable/package/GUI branding;
- any compatibility changes required by modern descriptor/SQLite wallet architecture;
- compilation fixes revealed by the first build.

## Later validation gate

After the mechanical source migration is complete:

1. compile on the target platforms;
2. reproduce all three genesis blocks;
3. compare block IDs and Scrypt PoW hashes;
4. reindex the existing Europa main chain from genesis;
5. compare difficulty transitions and subsidy acceptance with the authoritative client;
6. verify old Base58/Bech32 addresses and wallet keys;
7. verify SegWit history;
8. confirm the modern client reaches the existing live tip and never creates a replacement chain.

Nothing in this directory is release-ready merely because it is based on modern Bitcoin Core.
