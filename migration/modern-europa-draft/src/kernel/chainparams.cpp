// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-present The Bitcoin Core developers
// Copyright (c) 2023-present The Europa Core developers
// Europa migration draft: UNTESTED / DO NOT RELEASE

#include <kernel/chainparams.h>

#include <consensus/amount.h>
#include <consensus/merkle.h>
#include <consensus/params.h>
#include <primitives/block.h>
#include <primitives/transaction.h>
#include <script/script.h>
#include <uint256.h>
#include <util/chaintype.h>
#include <util/strencodings.h>

#include <algorithm>
#include <cassert>
#include <cstring>
#include <limits>
#include <memory>
#include <ranges>
#include <stdexcept>
#include <vector>

namespace {

static CBlock CreateEuropaGenesisBlock(uint32_t nTime,
                                      uint32_t nNonce,
                                      uint32_t nBits,
                                      int32_t nVersion,
                                      const CAmount& genesisReward)
{
    const char* pszTimestamp = "Europa Time For Zeus 23032023 1645";
    const CScript genesisOutputScript =
        CScript() << ParseHex("04c59ce68603af394b4c4d8155c17eaad7de6d987b2c4e3c0490c255b32f9be060f94d7f66416275fc6cd30f2dc4b087c42113a30f094486a5d46d1484432e810e")
                  << OP_CHECKSIG;

    CMutableTransaction txNew;
    txNew.version = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig =
        CScript() << 486604799 << CScriptNum(4)
                  << std::vector<unsigned char>(
                         reinterpret_cast<const unsigned char*>(pszTimestamp),
                         reinterpret_cast<const unsigned char*>(pszTimestamp) + std::strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime = nTime;
    genesis.nBits = nBits;
    genesis.nNonce = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

static void DisableTaproot(Consensus::Params& consensus)
{
    consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].bit = 2;
    consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nStartTime =
        Consensus::BIP9Deployment::NEVER_ACTIVE;
    consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nTimeout =
        Consensus::BIP9Deployment::NO_TIMEOUT;
    consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].min_activation_height = 0;
}

static void SetTestDummy(Consensus::Params& consensus,
                         int64_t start,
                         int64_t timeout,
                         uint32_t threshold,
                         uint32_t period)
{
    consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
    consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = start;
    consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = timeout;
    consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].min_activation_height = 0;
    consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].threshold = threshold;
    consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].period = period;
}

class CMainParams final : public CChainParams
{
public:
    CMainParams()
    {
        m_chain_type = ChainType::MAIN;
        consensus.signet_blocks = false;
        consensus.signet_challenge.clear();

        consensus.nSubsidyHalvingInterval = 210000;
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256{};
        consensus.BIP65Height = 0;
        consensus.BIP66Height = 0;
        consensus.CSVHeight = 0;
        consensus.SegwitHeight = 0;
        consensus.MinBIP9WarningHeight = std::numeric_limits<int>::max();

        consensus.powLimit = uint256{"00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"};
        consensus.nPowTargetTimespan = 10 * 60;
        consensus.nPowTargetSpacing = 5 * 60;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.enforce_BIP94 = false;
        consensus.fPowNoRetargeting = false;

        consensus.nLegacyRuleChangeActivationThreshold = 2;
        consensus.nLegacyMinerConfirmationWindow = 8;

        SetTestDummy(consensus, 1199145601, 1230767999, 2, 8);
        DisableTaproot(consensus);

        consensus.nMinimumChainWork =
            uint256{"0000000000000000000000000000000000000000000000000000000000200020"};
        consensus.defaultAssumeValid =
            uint256{"3371d05fac15f3aa5ceb494538e736ea3b9fcb5c78613ef57c0abe3205ac44a3"};

        pchMessageStart = {0x44, 0x5c, 0xfb, 0x14};
        nDefaultPort = 11342;
        nPruneAfterHeight = 100000;
        m_assumed_blockchain_size = 2;
        m_assumed_chain_state_size = 2;

        genesis = CreateEuropaGenesisBlock(1679590894, 831965, 0x1e0ffff0, 1, 10 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock ==
               uint256{"3371d05fac15f3aa5ceb494538e736ea3b9fcb5c78613ef57c0abe3205ac44a3"});
        assert(genesis.hashMerkleRoot ==
               uint256{"088e3e73609b492d5848cce9d2bc2eab6714fc7fd5c5580a4c40dd142cedb603"});

        vSeeds.clear();
        vFixedSeeds.clear(); // authoritative fixed seeds need modern format conversion

        base58Prefixes[PUBKEY_ADDRESS] = {34};
        base58Prefixes[SCRIPT_ADDRESS] = {5};
        base58Prefixes[SCRIPT_ADDRESS2] = {33};
        base58Prefixes[SECRET_KEY] = {161};
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};
        bech32_hrp = "era";
        m_legacy_witness_unknown_bech32 = true;

        fDefaultConsistencyChecks = false;
        m_is_mockable_chain = false;
        m_assumeutxo_data.clear();
        chainTxData = {1679590894, 0, 0.0};

        // Non-consensus header-sync tuning; calibrate during testing.
        m_headers_sync_params = {.commitment_period = 641, .redownload_buffer_size = 15218};
    }
};

class CTestNetParams final : public CChainParams
{
public:
    CTestNetParams()
    {
        m_chain_type = ChainType::TESTNET;
        consensus.signet_blocks = false;
        consensus.signet_challenge.clear();

        consensus.nSubsidyHalvingInterval = 210000;
        consensus.BIP34Height = 76;
        consensus.BIP34Hash = uint256{};
        consensus.BIP65Height = 76;
        consensus.BIP66Height = 76;

        // Legacy Europa testnet used BIP9 for CSV and SegWit. Do not pretend
        // they were active from genesis; the deployment adapter must map these.
        consensus.CSVHeight = std::numeric_limits<int>::max();
        consensus.SegwitHeight = std::numeric_limits<int>::max();
        consensus.fUseLegacyCSVVersionBits = true;
        consensus.fUseLegacySegwitVersionBits = true;
        consensus.nLegacyCSVStartTime = 1483228800;
        consensus.nLegacyCSVTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.nLegacySegwitStartTime = 1483228800;
        consensus.nLegacySegwitTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.nLegacyRuleChangeActivationThreshold = 1512;
        consensus.nLegacyMinerConfirmationWindow = 2016;
        consensus.MinBIP9WarningHeight = std::numeric_limits<int>::max();

        consensus.powLimit = uint256{"00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"};
        consensus.nPowTargetTimespan = 10 * 60;
        consensus.nPowTargetSpacing = 5 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.enforce_BIP94 = false;
        consensus.fPowNoRetargeting = false;

        SetTestDummy(consensus, 1199145601, 1230767999, 1512, 2016);
        DisableTaproot(consensus);

        consensus.nMinimumChainWork =
            uint256{"0000000000000000000000000000000000000000000000000000000000200020"};
        consensus.defaultAssumeValid =
            uint256{"9ace97f53590e17a628ea812f13636179ff67ee56d1a34b7ac9cabf335c3b54f"};

        pchMessageStart = {0xed, 0x20, 0xb2, 0xd8};
        nDefaultPort = 21342;
        nPruneAfterHeight = 1000;
        m_assumed_blockchain_size = 2;
        m_assumed_chain_state_size = 1;

        genesis = CreateEuropaGenesisBlock(1679591278, 1622507, 0x1e0ffff0, 1, 10 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock ==
               uint256{"9ace97f53590e17a628ea812f13636179ff67ee56d1a34b7ac9cabf335c3b54f"});
        assert(genesis.hashMerkleRoot ==
               uint256{"088e3e73609b492d5848cce9d2bc2eab6714fc7fd5c5580a4c40dd142cedb603"});

        vSeeds.clear();
        vFixedSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = {111};
        base58Prefixes[SCRIPT_ADDRESS] = {196};
        base58Prefixes[SCRIPT_ADDRESS2] = {33};
        base58Prefixes[SECRET_KEY] = {239};
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};
        bech32_hrp = "tera";
        m_legacy_witness_unknown_bech32 = true;

        fDefaultConsistencyChecks = false;
        m_is_mockable_chain = false;
        m_assumeutxo_data.clear();
        chainTxData = {1679591278, 0, 0.0};
        m_headers_sync_params = {.commitment_period = 673, .redownload_buffer_size = 14460};
    }
};

class CRegTestParams final : public CChainParams
{
public:
    explicit CRegTestParams(const RegTestOptions& opts)
    {
        m_chain_type = ChainType::REGTEST;
        consensus.signet_blocks = false;
        consensus.signet_challenge.clear();

        consensus.nSubsidyHalvingInterval = 150;
        consensus.BIP34Height = 500;
        consensus.BIP34Hash = uint256{};
        consensus.BIP65Height = 1351;
        consensus.BIP66Height = 1251;

        // Legacy regtest CSV used BIP9 start=0; SegWit was ALWAYS_ACTIVE.
        consensus.CSVHeight = std::numeric_limits<int>::max();
        consensus.fUseLegacyCSVVersionBits = true;
        consensus.nLegacyCSVStartTime = 0;
        consensus.nLegacyCSVTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.SegwitHeight = 0;
        consensus.nLegacyRuleChangeActivationThreshold = 108;
        consensus.nLegacyMinerConfirmationWindow = 144;
        consensus.MinBIP9WarningHeight = std::numeric_limits<int>::max();

        consensus.powLimit =
            uint256{"7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"};
        // Preserve the authoritative expression: 3.5 days.
        consensus.nPowTargetTimespan = 302400;
        consensus.nPowTargetSpacing = 5 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.enforce_BIP94 = false;
        consensus.fPowNoRetargeting = true;

        SetTestDummy(consensus, 0, Consensus::BIP9Deployment::NO_TIMEOUT, 108, 144);
        DisableTaproot(consensus);

        consensus.nMinimumChainWork = uint256{};
        consensus.defaultAssumeValid = uint256{};

        for (const auto& [deployment_pos, version_bits_params] : opts.version_bits_parameters) {
            consensus.vDeployments[deployment_pos].nStartTime = version_bits_params.start_time;
            consensus.vDeployments[deployment_pos].nTimeout = version_bits_params.timeout;
            consensus.vDeployments[deployment_pos].min_activation_height =
                version_bits_params.min_activation_height;
        }
        for (const auto& [deployment, height] : opts.activation_heights) {
            switch (deployment) {
            case Consensus::DEPLOYMENT_HEIGHTINCB: consensus.BIP34Height = height; break;
            case Consensus::DEPLOYMENT_CLTV: consensus.BIP65Height = height; break;
            case Consensus::DEPLOYMENT_DERSIG: consensus.BIP66Height = height; break;
            case Consensus::DEPLOYMENT_CSV: consensus.CSVHeight = height; break;
            case Consensus::DEPLOYMENT_SEGWIT: consensus.SegwitHeight = height; break;
            }
        }

        pchMessageStart = {0xfa, 0xbf, 0xb5, 0xda};
        nDefaultPort = 19444;
        nPruneAfterHeight = opts.fastprune ? 100 : 1000;
        m_assumed_blockchain_size = 0;
        m_assumed_chain_state_size = 0;

        genesis = CreateEuropaGenesisBlock(1679592022, 1, 0x207fffff, 1, 10 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock ==
               uint256{"ea1e2b682394b7d10fc4efcf1dc01d78abd886bd93f6809db52119e4c5af6000"});
        assert(genesis.hashMerkleRoot ==
               uint256{"088e3e73609b492d5848cce9d2bc2eab6714fc7fd5c5580a4c40dd142cedb603"});

        vSeeds.clear();
        vFixedSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = {111};
        base58Prefixes[SCRIPT_ADDRESS] = {196};
        base58Prefixes[SCRIPT_ADDRESS2] = {33};
        base58Prefixes[SECRET_KEY] = {239};
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};
        bech32_hrp = "rera";
        m_legacy_witness_unknown_bech32 = true;

        fDefaultConsistencyChecks = true;
        m_is_mockable_chain = true;
        m_assumeutxo_data.clear();
        chainTxData = {0, 0, 0.0};
        m_headers_sync_params = {.commitment_period = 275, .redownload_buffer_size = 7017};
    }
};

} // namespace

std::unique_ptr<const CChainParams> CChainParams::Main()
{
    return std::make_unique<const CMainParams>();
}

std::unique_ptr<const CChainParams> CChainParams::TestNet()
{
    return std::make_unique<const CTestNetParams>();
}

std::unique_ptr<const CChainParams> CChainParams::RegTest(const RegTestOptions& options)
{
    return std::make_unique<const CRegTestParams>(options);
}

std::unique_ptr<const CChainParams> CChainParams::TestNet4()
{
    throw std::runtime_error("Europa migration draft: Bitcoin testnet4 is not an Europa network");
}

std::unique_ptr<const CChainParams> CChainParams::SigNet(const SigNetOptions&)
{
    throw std::runtime_error("Europa migration draft: signet is not an Europa network");
}

std::vector<int> CChainParams::GetAvailableSnapshotHeights() const
{
    std::vector<int> heights;
    heights.reserve(m_assumeutxo_data.size());
    for (const auto& data : m_assumeutxo_data) heights.push_back(data.height);
    return heights;
}

std::optional<ChainType> GetNetworkForMagic(const MessageStartChars& message)
{
    const auto mainnet = CChainParams::Main()->MessageStart();
    const auto testnet = CChainParams::TestNet()->MessageStart();
    const auto regtest = CChainParams::RegTest({})->MessageStart();

    if (std::ranges::equal(message, mainnet)) return ChainType::MAIN;
    if (std::ranges::equal(message, testnet)) return ChainType::TESTNET;
    if (std::ranges::equal(message, regtest)) return ChainType::REGTEST;
    return std::nullopt;
}
