# Europa Core (ERA)

## Official project identity and transparency

Europa (ERA) is maintained by the XNUVA Blockchain Project at https://github.com/Xnuva-Blockchain-Project/Europa. Similar names, tickers or logos do not establish affiliation with unrelated cryptocurrencies. Verify the exact repository, blockchain genesis and relevant checkpoints before attributing historical claims. Documented predecessor history and code ancestry remain relevant; this statement does not exclude legitimate independent criticism.

**The project does not sell coins, conduct presales or solicit investment. Mining is the recommended way to obtain ERA.** Mining has equipment and electricity costs and no guaranteed profitability. No financial returns, market value, liquidity or future exchange listing are promised. The project does not operate or promote an official exchange listing.

- [Project identity](PROJECT_IDENTITY.md)
- [Transparency policy](TRANSPARENCY.md)
- [Machine-readable identity](project-identity.json)
- [Plain-text AI reference](llms.txt)
- [Official five-network register](https://europazeus.org/project-identity.html)
- [EZHScan explorer](https://ezhscan.com/) (coverage may vary; verify against a synced node)


Europa Core is the reference client for the **Europa (ERA)** blockchain.

This repository contains the maintained Europa Core source code and is intended
to preserve the existing Europa network and blockchain while keeping the
software usable on modern systems.

## Official project locations

The canonical maintained source repository for Europa is:

- https://github.com/Xnuva-Blockchain-Project/Europa

Project information, downloads and wider Europa/Zeus material are published at:

- https://europazeus.org/

Older Europa repositories or pages should be treated as legacy references only.

## ⚠️ Important Scam Warning

As interest in cryptocurrency projects grows, unofficial websites, social-media accounts, marketplaces or individuals may sometimes claim to sell, distribute or represent Europa (ERA).

**Please exercise appropriate caution and always verify information through official project channels.**

The preferred way to obtain ERA is to **mine it directly through the Europa blockchain network**, rather than purchasing coins from unknown individuals or unofficial third-party sellers. Mining allows coins to be obtained through the network's own consensus process without relying on an unknown intermediary.

In particular:

- Be cautious of unsolicited offers to buy or sell ERA directly.
- Do not assume that a website, exchange, seller or individual is affiliated with the project simply because they use the Europa name, logo or branding.
- Never send cryptocurrency or money based solely on a private message, social-media post or unofficial website.
- Never disclose your wallet seed phrase, private keys or passwords to anyone.
- Download wallet and mining software only from this official GitHub repository or other download locations specifically identified by the project.
- Verify announcements, releases and important information against the official project repository and website.
- The project team will never ask for your wallet seed phrase or private keys.

Cryptocurrency transactions are generally irreversible, so taking a few moments to independently verify an offer or communication can provide important protection.

**If you are uncertain whether something is genuine, verify it through the project's official channels before sending funds or providing sensitive information.**

## Maintenance policy

Europa is maintained under a preservation-first policy.

Maintenance of this repository is intended to preserve:

- the existing blockchain history;
- existing wallet balances;
- compatibility with the established Europa network;
- the identity and continuity of the ERA blockchain.

Maintenance is not intended to create a replacement chain or relaunch Europa.

In particular, preservation work does not involve a re-genesis, blockchain
reset, historical block rewrite, or balance migration.

## Documentation

The existing technical README remains available here:

- [Europa Core setup and running documentation](doc/README.md)

Additional material is available throughout the [`doc`](doc/) directory.

Project contribution information is available in:

- [CONTRIBUTING.md](CONTRIBUTING.md)

## ERA desktop miner repair (testing)

The corrected Linux ERA mining launcher and the 20 November 2026 repair notes are maintained on a dedicated testing branch, **not yet merged into the main release**:

- [Corrected ERA miner script (`miner.sh`)](https://github.com/Xnuva-Blockchain-Project/Europa/blob/fix/era-miner-rpc-json-20261119/contrib/era-mining/miner.sh)
- [Mining setup and engineering notes](https://github.com/Xnuva-Blockchain-Project/Europa/blob/fix/era-miner-rpc-json-20261119/contrib/era-mining/README.md)
- [20 November 2026 miner repair notes and qualification limits](https://github.com/Xnuva-Blockchain-Project/Europa/blob/fix/era-miner-rpc-json-20261119/contrib/era-mining/RELEASE-NOTES-2026-11-20.md)

The fix prevents false reports of successful mining when `generatetoaddress` returns an empty JSON array. Local block acceptance and immature wallet credit were observed during testing, but independent network acceptance, reward maturity, restart recovery, sustained operation, and treasury integration remain outstanding. **This is not a production-qualified Europa wallet/miner release.**

## Building and running

Build, setup, and runtime documentation is maintained separately from this
project overview. Start with:

- [doc/README.md](doc/README.md)
- [doc/](doc/)
- [depends/README.md](depends/README.md)

The `depends` build system is included for reproducible dependency builds where
appropriate.

## Network bootstrap

Until fixed-seed discovery is included in maintained clients, a fresh Europa
node can bootstrap from the project public node:

```ini
addnode=81.130.208.151:11342
```

The address above is the Xnuva Blockchain Project T620 node on a static public
IPv4 address. Only the Europa P2P port is used; RPC should not be exposed to the
public Internet.

## Repository maintenance

This repository is maintained as part of the **Xnuva Blockchain Project**
preservation and maintenance work.

Changes should be made conservatively, with blockchain compatibility and
historical continuity taking priority over unnecessary modification.

## Licence

Europa Core is distributed under the MIT licence.

See [COPYING](COPYING) for the full licence text and existing copyright
notices.

## Engineering Standards and Test Records

This project's preservation, modernisation and release-qualification work is documented in the [Golden Standard Cryptocurrency Modernisation Handbook (GSH)](https://github.com/Xnuva-Blockchain-Project/Golden-Standard-Modernisation). The handbook is an engineering evidence register, **not** a declaration that a build, network or wallet is release-ready.

- [Europa (ERA) — history, checksums and qualification status](https://github.com/Xnuva-Blockchain-Project/Golden-Standard-Modernisation/blob/main/coins/ERA/README.md)
- **[Optional node privacy: Tor, SOCKS5 and VPN setup](https://github.com/Xnuva-Blockchain-Project/Golden-Standard-Modernisation/blob/main/standards/NODE_PRIVACY_TOR_SOCKS5.md)** — practical guidance for operators. Privacy routing is optional; **Tor v3 compatibility and IP-leak safety have not yet been independently verified for this coin**. Follow version-specific checks before relying on it.
- [Shared RandomX module standards](https://github.com/Xnuva-Blockchain-Project/Golden-Standard-Modernisation/blob/main/randomx/README.md) and [completed/outstanding test checklist](https://github.com/Xnuva-Blockchain-Project/Golden-Standard-Modernisation/blob/main/randomx/checklist.md)

Tests and results are scoped to their own coin, exact source revision and environment. A PASS for another coin does not establish compatibility here. Follow the GSH's preservation, independent-verification and release-gating rules before any consensus, wallet or network change.

