// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-present The Bitcoin Core developers
// Europa migration draft: UNTESTED / DO NOT RELEASE

#include <chainparams.h>

#include <chainparamsbase.h>
#include <common/args.h>
#include <consensus/params.h>
#include <tinyformat.h>
#include <util/chaintype.h>
#include <util/string.h>

#include <cassert>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <vector>

using util::SplitString;

static std::unique_ptr<const CChainParams> globalChainParams;

static void ReadEuropaRegTestArgs(const ArgsManager& args, CChainParams::RegTestOptions& options)
{
    if (auto value = args.GetBoolArg("-fastprune")) options.fastprune = *value;

    for (const std::string& strDeployment : args.GetArgs("-vbparams")) {
        std::vector<std::string> parts = SplitString(strDeployment, ':');
        if (parts.size() < 3 || parts.size() > 4 || parts[0] != "testdummy") {
            throw std::runtime_error(
                "Europa migration draft: only testdummy:start:end[:min_activation_height] is supported");
        }

        const auto start = ToIntegral<int64_t>(parts[1]);
        const auto timeout = ToIntegral<int64_t>(parts[2]);
        if (!start || !timeout) throw std::runtime_error("Invalid Europa -vbparams time");

        CChainParams::VersionBitsParameters p{};
        p.start_time = *start;
        p.timeout = *timeout;
        p.min_activation_height = 0;

        if (parts.size() == 4) {
            const auto height = ToIntegral<int>(parts[3]);
            if (!height || *height < 0) throw std::runtime_error("Invalid Europa -vbparams height");
            p.min_activation_height = *height;
        }
        options.version_bits_parameters[Consensus::DEPLOYMENT_TESTDUMMY] = p;
    }
}

const CChainParams& Params()
{
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<const CChainParams> CreateChainParams(const ArgsManager& args, const ChainType chain)
{
    switch (chain) {
    case ChainType::MAIN:
        return CChainParams::Main();
    case ChainType::TESTNET:
        return CChainParams::TestNet();
    case ChainType::REGTEST: {
        CChainParams::RegTestOptions opts{};
        ReadEuropaRegTestArgs(args, opts);
        return CChainParams::RegTest(opts);
    }
    case ChainType::TESTNET4:
        throw std::runtime_error("Europa migration draft: Bitcoin testnet4 is not an Europa network");
    case ChainType::SIGNET:
        throw std::runtime_error("Europa migration draft: signet is not an Europa network");
    }
    throw std::runtime_error("Europa migration draft: unsupported chain type");
}

void SelectParams(const ChainType chain)
{
    SelectBaseParams(chain);
    globalChainParams = CreateChainParams(gArgs, chain);
}
