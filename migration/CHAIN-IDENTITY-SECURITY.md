# Europa / ERA chain-identity security contract

This document is part of the Bitcoin Core 31.1 modernisation branch and is a **release gate**.

## Purpose

The modernised ERA client must preserve the protection qualified after the 2024 alternative-chain incident. Modern Bitcoin Core infrastructure does not, by itself, know which historical Europa chain is canonical.

Europa is Proof of Work only. Every mainnet block-production surface in the modern client must therefore fail closed unless the active chain is proven to be the recognised ERA chain.

## Qualified canonical identity

These values were independently qualified against the recognised live ERA chain and against the reconstructed 2024 alternative history:

- Genesis: `3371d05fac15f3aa5ceb494538e736ea3b9fcb5c78613ef57c0abe3205ac44a3`
- Canonical block 1: `b4bfd0c47191b145e6a74fedc9398cf3b08102418fc23639cca263bb533104e7`
- Buried security-anchor height: `12000`
- Buried security-anchor hash: `dd212cbd9315c45328e903bfa89e014878b09b2f6f16cff2cb3e9a24612715f1`
- Minimum chainwork at the buried anchor: `000000000000000000000000000000000000000000000000000046e276e046d1`
- Current hardened legacy-source security commit: `7e1fdabb11dda86f89a13e147d38a1d55a2b0419`

The reconstructed 2024 alternative history diverges at block 1 and never reaches the recognised height-12000 anchor.

## Mandatory mainnet production guard

Before the modern code can create a mainnet block template or begin PoW work, the **central block-building path** must refuse production unless all of the following are true:

1. the active tip exists;
2. the active height is at least 12000;
3. active-chain block 1 exactly matches the canonical block-1 hash above;
4. active-chain block 12000 exactly matches the buried anchor above;
5. active-tip chainwork is at least the qualified minimum above;
6. the node is not in initial block download / synchronisation state;
7. at least one peer connection is present.

The check must live centrally so every mining RPC, built-in miner and external-template path inherits the same protection.

Testnet and regtest must remain unaffected by the mainnet-only guard.

## Modern chain parameters

The modern mainnet draft must use the qualified height-12000 chainwork for `nMinimumChainWork` and the height-12000 hash for `defaultAssumeValid`, replacing the old pre-hardening placeholder values.

Those parameters complement the production guard; they do not replace it.

## Qualification required before release

Negative path: run the materialised modern client against a disposable copy of the reconstructed 2024 alternative chain and prove that block-template/mining production is refused and the tip does not change.

Positive path: run it against a disposable copy of the recognised ERA chain, prove block 1 / height 12000 / chainwork / sync / peer gates, and create a valid next-block template without doing PoW or submitting a block.

The modernised client is not release-ready until both paths pass.
