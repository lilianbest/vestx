// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2017 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>
#include <consensus/merkle.h>

#include <tinyformat.h>
#include <utilstrencodings.h>
#include <arith_uint256.h>

#include <assert.h>
#include <chainparamsseeds.h>

#ifdef __MINGW32__
#include <sys/time.h>
#endif

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "vestx chain launch mar2019";
    const CScript genesisOutputScript = CScript() << ParseHex("03042a235a39a72d7b1296313b0193ba3f93ad1a1fa2d72f1cab4f21d342c9d5b8") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    consensus.vDeployments[d].nStartTime = nStartTime;
    consensus.vDeployments[d].nTimeout = nTimeout;
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";

        consensus.nFirstPoSBlock = 500;
        consensus.nInstantSendKeepLock = 24;
        consensus.nBudgetPaymentsStartBlock = 0;
        consensus.nBudgetPaymentsCycleBlocks = 16616;
        consensus.nBudgetPaymentsWindowBlocks = 100;
        consensus.nBudgetProposalEstablishingTime = 60*60*24;
        consensus.nSuperblockCycle = 43200;
        consensus.nSuperblockStartBlock = consensus.nSuperblockCycle;
        consensus.nGovernanceMinQuorum = 10;
        consensus.nGovernanceFilterElements = 20000;
        consensus.BIP34Height = 10;
        consensus.BIP34Hash = uint256S("0000000000000000000000000000000000000000000000000000000000000000");
        consensus.BIP65Height = consensus.nFirstPoSBlock;
        consensus.BIP66Height = consensus.nFirstPoSBlock;
        consensus.powLimit = uint256S("0000ffff00000000000000000000000000000000000000000000000000000000");
        consensus.nPowTargetTimespan = 2 * 60;
        consensus.nPowTargetSpacing = 40;
        consensus.nPosTargetSpacing = consensus.nPowTargetSpacing;
        consensus.nPosTargetTimespan = consensus.nPowTargetTimespan;
        consensus.nMasternodeMinimumConfirmations = 15;
        consensus.nStakeMinAge = 60 * 60;
        consensus.nStakeMaxAge = 60 * 60 * 24 * 30;
        consensus.nModifierInterval = 60 * 20;
        consensus.nCoinbaseMaturity = 20;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1080;
        consensus.nMinerConfirmationWindow = 1440;

        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999;

        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1533167940;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1564012740;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0000000000000000000000000000000000000000000000000000000000000000");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0000000000000000000000000000000000000000000000000000000000000000");

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xc4;
        pchMessageStart[1] = 0x4d;
        pchMessageStart[2] = 0xe4;
        pchMessageStart[3] = 0x4f;
        nDefaultPort = 20000;
        nPruneAfterHeight = 100000;
        nMaxReorganizationDepth = 100;

        genesis = CreateGenesisBlock(1554250000, 42105, 0x1f00ffff, 1, 0 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0000beeb61cc43b60e855ceaad27378e90a59d6e9c54e1476406f48c5ffccda3"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,70);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,132);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,198);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};
        bech32_hrp = "vx";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        nCollateralLevels = { 15000000 };
        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 60*60;
        strSporkPubKey = "03cfd340c928af09534e019fa5c94ffe4ff36b478d7fa69a71d9365218fe9c2b8c"; // 02042019

        checkpointData = {
            {
                { 0, uint256S("0000beeb61cc43b60e855ceaad27378e90a59d6e9c54e1476406f48c5ffccda3") },
                {20, uint256S("00002699f530039cc972ae486e4d428ee54542ab3a800c082d07018b21c2bf6b") }
            }
        };

        chainTxData = ChainTxData{
            1554250000,
            1,
            1.0
        };

        /* disable fallback fee on mainnet */
        m_fallback_fee_enabled = true;
    }
};

/*
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";

	// quick note: test and temp networks operate on a existance window of 
        // approx 3 hours, window is persistent forever if daemon doesnt get restarted
        uint32_t current_time = (uint32_t)time(NULL);
	uint32_t elapsed_day = current_time % 86400;
	uint32_t start_of_day = current_time - elapsed_day;
	uint32_t quadrant = start_of_day % 10800;

        consensus.nFirstPoSBlock = 300;
        consensus.nInstantSendKeepLock = 24;
        consensus.nBudgetPaymentsStartBlock = 0;
        consensus.nBudgetPaymentsCycleBlocks = 16616;
        consensus.nBudgetPaymentsWindowBlocks = 100;
        consensus.nBudgetProposalEstablishingTime = 60*60*24;
        consensus.nSuperblockCycle = 43200;
        consensus.nSuperblockStartBlock = consensus.nSuperblockCycle;
        consensus.nGovernanceMinQuorum = 10;
        consensus.nGovernanceFilterElements = 20000;
        consensus.BIP34Height = 10;
        consensus.BIP34Hash = uint256S("0000000000000000000000000000000000000000000000000000000000000000");
        consensus.BIP65Height = consensus.nFirstPoSBlock;
        consensus.BIP66Height = consensus.nFirstPoSBlock;
        consensus.powLimit = uint256S("0000ffff00000000000000000000000000000000000000000000000000000000");
        consensus.nPowTargetTimespan = 2 * 60;
        consensus.nPowTargetSpacing = 40;
        consensus.nPosTargetSpacing = consensus.nPowTargetSpacing;
        consensus.nPosTargetTimespan = consensus.nPowTargetTimespan;
        consensus.nMasternodeMinimumConfirmations = 15;
        consensus.nStakeMinAge = 10 * 60;
        consensus.nStakeMaxAge = 60 * 60 * 24 * 30;
        consensus.nModifierInterval = 60 * 20;
        consensus.nCoinbaseMaturity = 15;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1080;
        consensus.nMinerConfirmationWindow = 1440;

        // genesis routine
        uint32_t nTime = start_of_day;
        for(int i=0; i<quadrant; i++)
	  nTime =+ 10800;
        uint32_t nNonce = 0;

	while (UintToArith256(genesis.GetHash()) > 
               UintToArith256(consensus.powLimit)) {
	  nNonce++;
	  genesis = CreateGenesisBlock(nTime, nNonce, 0x1f00ffff, 1, 0 * COIN);
	}
	genesis = CreateGenesisBlock(nTime, nNonce, 0x1f00ffff, 1, 0 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();

        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999;

        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1533167940;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1564012740;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0000000000000000000000000000000000000000000000000000000000000000");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0000000000000000000000000000000000000000000000000000000000000000");

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xb1;
        pchMessageStart[1] = 0xc1;
        pchMessageStart[2] = 0xd1;
        pchMessageStart[3] = 0xe1;
        nDefaultPort = 30000;
        nPruneAfterHeight = 100000;
        nMaxReorganizationDepth = 100;

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,70);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,132);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,198);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};
        bech32_hrp = "xc";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        nCollateralLevels = { 50000 };
        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 60*60;
        strSporkPubKey = "030a2b7fdf1f123f3686ebc00f1226a20275bc785570ef069e2c2d81b61d616e91";

        checkpointData = {};
        chainTxData = ChainTxData {};

        /* disable fallback fee on mainnet */
        m_fallback_fee_enabled = false;
    }
};

/*
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
    }
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}

void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    globalChainParams->UpdateVersionBitsParameters(d, nStartTime, nTimeout);
}
