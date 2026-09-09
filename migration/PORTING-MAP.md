# Europa -> Bitcoin Core 31.1 porting map

Europa is substantially closer to modern Bitcoin than Zeus/HOPE because it is PoW-only and already has SegWit support.

| Legacy Europa area | Modern target | Required action |
|---|---|---|
| `src/primitives/block.*` | modern block primitives | retain standard block ID plus Europa Scrypt `GetPoWHash()` |
| `src/pow.*` | modern PoW module | retain Scrypt proof input and Europa retarget arithmetic |
| `src/chainparams.*` | modern kernel chainparams | preserve genesis, magic, ports, prefixes, timing and deployments |
| `src/amount.h` | modern consensus amount range | preserve `MAX_MONEY = 4,200,000 ERA` |
| `GetBlockSubsidy` | modern subsidy module | preserve 10 ERA and 210,000-block halvings |
| legacy CSV/SegWit deployments | modern buried heights | represent existing ALWAYS_ACTIVE behaviour without changing history |
| legacy secondary script prefix | modern address/key I/O | retain compatibility explicitly |
| validation PoW check | modern validation | validate `GetPoWHash()`, not block ID, against nBits |
| wallet/miner/RPC | modern modules | port after consensus primitives |

## Non-negotiable

When Bitcoin Core 31.1 defaults differ from already-established Europa consensus, Europa's historical rules win for existing chain history.
