# Europa Core (ERA)

Europa Core is the reference client for the **Europa (ERA)** blockchain.

This repository contains the maintained Europa Core source code and is intended
to preserve the existing Europa network and blockchain while keeping the
software usable on modern systems.

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
