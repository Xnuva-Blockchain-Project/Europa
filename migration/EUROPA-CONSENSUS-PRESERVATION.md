# Europa consensus preservation manifest

**Purpose:** migration reference only.  
**Authoritative source branch:** `main`  
**Authoritative source commit:** `233946e88dfa7a70b509b8c7dfa6e2c29bd0fc65`

The modern client must remain on the existing Europa blockchain. This is a software migration, not a new chain.

## Consensus model

Europa is **Proof of Work only**.

The authoritative source contains no PoS/coinstake/staking consensus layer. The modern target must not introduce one.

## Mainnet identity

- Genesis hash: `3371d05fac15f3aa5ceb494538e736ea3b9fcb5c78613ef57c0abe3205ac44a3`
- Genesis merkle root: `088e3e73609b492d5848cce9d2bc2eab6714fc7fd5c5580a4c40dd142cedb603`
- Genesis timestamp text: `Europa Time For Zeus 23032023 1645`
- Genesis time: `1679590894`
- Genesis nonce: `831965`
- Genesis bits: `0x1e0ffff0`
- Genesis version: `1`
- Genesis reward: `10 ERA`
- Message start: `44 5c fb 14`
- Main P2P port: `11342`
- Main RPC port: `11341`

## Mainnet address identity

- P2PKH prefix: `34`
- Primary P2SH prefix: `5`
- Secondary script-address prefix: `33`
- WIF/secret prefix: `161`
- Extended public key: `04 88 b2 1e`
- Extended secret key: `04 88 ad e4`
- Bech32 HRP: `era`

The legacy secondary script-address prefix is Europa-specific and must not be silently discarded during wallet/address modernisation.

## PoW and difficulty

- Block identifier: standard serialized double-SHA256 header hash
- Proof-of-Work hash: Scrypt `1024_1_1_256`
- Mainnet PoW limit: `00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff`
- Target spacing: `300` seconds
- Target timespan: `600` seconds
- Minimum-difficulty blocks: disabled on mainnet
- Retargeting: enabled
- Existing retarget implementation contains an Europa-specific intermediate-overflow guard; preserve its arithmetic behaviour.

This distinction is consensus critical: **block IDs are not the Scrypt PoW hashes**.

## Monetary policy

- Initial block subsidy: `10 ERA`
- Mainnet subsidy-halving interval: `210,000` blocks
- Subsidy right-shifts by one at each halving, as in the authoritative source
- `COIN = 100,000,000`
- `MAX_MONEY = 4,200,000 ERA`

No monetary-policy change is part of this mechanical migration.

## Mainnet script/deployment state

Authoritative source:

- BIP16/P2SH: active from height 0
- BIP34: active from height 0
- BIP65: active from height 0
- BIP66: active from height 0
- CSV/BIP68/BIP112/BIP113: ALWAYS_ACTIVE
- SegWit/BIP141/BIP143/BIP147: ALWAYS_ACTIVE
- Taproot is not part of the authoritative Europa deployment configuration

The Bitcoin Core 31.1 port must not silently introduce a new Taproot activation or otherwise change historical script validity.

## Testnet / regtest identities

Testnet:
- genesis `9ace97f53590e17a628ea812f13636179ff67ee56d1a34b7ac9cabf335c3b54f`
- time `1679591278`
- nonce `1622507`
- bits `0x1e0ffff0`
- magic `ed 20 b2 d8`
- P2P `21342`
- RPC `21341`
- Bech32 `tera`

Regtest:
- genesis `ea1e2b682394b7d10fc4efcf1dc01d78abd886bd93f6809db52119e4c5af6000`
- time `1679592022`
- nonce `1`
- bits `0x207fffff`
- magic `fa bf b5 da`
- P2P `19444`
- RPC `19443`
- Bech32 `rera`

## Release invariant

A release candidate is unacceptable unless it validates the existing Europa chain from the original mainnet genesis through the live tip and agrees with the authoritative client on historical block IDs, Scrypt PoW validity, difficulty, subsidy, script validity and UTXO state.
