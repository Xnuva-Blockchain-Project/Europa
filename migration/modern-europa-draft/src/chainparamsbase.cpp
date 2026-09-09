// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-present The Bitcoin Core developers
// Europa migration draft: UNTESTED / DO NOT RELEASE

#include <chainparamsbase.h>

#include <common/args.h>
#include <util/chaintype.h>

#include <cassert>
#include <memory>
#include <stdexcept>

void SetupChainParamsBaseOptions(ArgsManager& argsman)
{
    argsman.AddArg("-chain=<chain>",
                   "Use the Europa chain <chain> (default: main). Allowed values: " LIST_CHAIN_NAMES,
                   ArgsManager::ALLOW_ANY, OptionsCategory::CHAINPARAMS);
    argsman.AddArg("-regtest",
                   "Enter Europa regression-test mode.",
                   ArgsManager::ALLOW_ANY | ArgsManager::DEBUG_ONLY,
                   OptionsCategory::CHAINPARAMS);
    argsman.AddArg("-testnet",
                   "Use the Europa test network. Equivalent to -chain=test.",
                   ArgsManager::ALLOW_ANY, OptionsCategory::CHAINPARAMS);
    argsman.AddArg("-vbparams=deployment:start:end[:min_activation_height]",
                   "Set version-bits parameters on Europa regtest only.",
                   ArgsManager::ALLOW_ANY | ArgsManager::DEBUG_ONLY,
                   OptionsCategory::CHAINPARAMS);
}

static std::unique_ptr<CBaseChainParams> globalChainBaseParams;

const CBaseChainParams& BaseParams()
{
    assert(globalChainBaseParams);
    return *globalChainBaseParams;
}

std::unique_ptr<CBaseChainParams> CreateBaseChainParams(const ChainType chain)
{
    switch (chain) {
    case ChainType::MAIN:
        return std::make_unique<CBaseChainParams>("", 11341);
    case ChainType::TESTNET:
        // Preserve the existing Europa data-directory name.
        return std::make_unique<CBaseChainParams>("testnet4", 21341);
    case ChainType::REGTEST:
        return std::make_unique<CBaseChainParams>("regtest", 19443);
    case ChainType::TESTNET4:
        throw std::runtime_error("Europa migration draft: Bitcoin testnet4 is not an Europa network");
    case ChainType::SIGNET:
        throw std::runtime_error("Europa migration draft: signet is not an Europa network");
    }
    throw std::runtime_error("Europa migration draft: unsupported chain type");
}

void SelectBaseParams(const ChainType chain)
{
    globalChainBaseParams = CreateBaseChainParams(chain);
    gArgs.SelectConfigNetwork(ChainTypeToString(chain));
}
