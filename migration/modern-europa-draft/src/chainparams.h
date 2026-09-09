// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-present The Bitcoin Core developers
// Europa migration draft: UNTESTED / DO NOT RELEASE
#ifndef BITCOIN_CHAINPARAMS_H
#define BITCOIN_CHAINPARAMS_H

#include <kernel/chainparams.h>

#include <memory>

class ArgsManager;

std::unique_ptr<const CChainParams> CreateChainParams(const ArgsManager& args, ChainType chain);
const CChainParams& Params();
void SelectParams(ChainType chain);

#endif // BITCOIN_CHAINPARAMS_H
