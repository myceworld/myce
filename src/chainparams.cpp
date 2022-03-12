// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018 The Myce developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "libzerocoin/Params.h"
#include "chainparams.h"
#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions
static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
    (0, uint256("0000c74cc66c72cb1a327c5c1d4893ae5276aa50be49fb23cec21df1a2f20d87"))
    (245145, uint256("d74019fd6d432b34e05febb5a8c32ca9e0ab500f21c378f1debcd8c939a59d95"))
    (341500, uint256("769f485854697c20088703e205017b96853f1a0bfbd288110bed45d39a3a60b4"))
    (378499, uint256("6aeb973ee1318ff18f69685dcf7052b1769fc90f8412d0388a8b9d3fdaae9ae1"))
    (378500, uint256("cecc0636b6b5c41cc852d45976243fe010a74797eeb056c1c3ee481b20c17d82"))
    (378501, uint256("032beb9154a0ae6cab2b6daaa6e9ba32468dae193fd78f745e875d3d2cdbec39"))
    (382600, uint256("552836176069ce72494583d566ee0427abb47b32c70138cb0a16a6fa8608caf1"))
    (384200, uint256("fab86b440d95a252b477128bd325153106a7220e3c7b5cbc74542f68945aa4f7"))
    (412450, uint256("32e0e14e4b18d2ef1b598f03a86496a690709899141e70885e2e23c0533985a4"))
    (525000, uint256("eb7f2d1c91266719c5323f41920cd963ffa7f79a01dacf41601046b06e182ff5"))
    (584800, uint256("73ede8b55f2054c14cf7cbade19fedcc18f5b3ea5d1a931e6f3a996ee46bbf57"))
    (609500, uint256("24303d0ec828ace9b375d6ccd4bfd63d279f670d6e39a232603583b4c893a9de"))
    (709999, uint256("f2f114a7e86bc4206c4da4bcc21aebe2f31798362720b9a6c017d166479d6a51"))
    (710000, uint256("6bf18c06e6aef68b04b2768867ddf8b227449ad0bc596070f00efeb5dbc67022"))
    (710001, uint256("61ccc10b583c9b23091398b24877ae2eafccc78ef88967ed12a75ccd49c00814"))
    (711000, uint256("5ebe513252484994fd2192c4395ffa63e0f9d46563dac7b463bb13e5d8c1ef62"))
    (712796, uint256("d5c88d0651ca72e05b3ae83d42b1bdc1f070bcf4a8aa014a53b6f599aca93c2f"))
    (712797, uint256("22f129920c98d55eb38e93eab055555eedaa36e8ae5af1d55a8a345f5037208b"))
    (712998, uint256("bbf33319fd9d7831896b5f99d0daec05c8f61aef463b48cb25642202d4ab3e23"))
    (714107, uint256("8f6c46fb7969bed9ee69299972e65ed79813be150c9949a9b31941dde7ec067c"))
    (714108, uint256("82c5429e3447a1602f92a39576db47f2af8d9b53842a89617da8540404599f34"))
    (714243, uint256("8ffafd4513119a95c31a0f4819e891294675073b3f0b01c333dc10be895640a0"))
    (714244, uint256("d0a5d59d3299732cad243d8a9a2be784057857198553007c9a4e25983b120b33"))
    (714600, uint256("456a262d238896a73df509f4dae2d75c7e5b23c7fedb3df77ca62d76488e8cc2"))
    (1000000, uint256("1fe3da1a5cb01dc742c5cffe2e3cd0cf95805aa6fe8944c5ea82f637194615a6"))
    (1199000, uint256("a864d8c070db56045f3952cb42a7901cbaa35162cb3aaaa6e64fdb050c42389a"))
    (1200000, uint256("2b6bafea41686849522cc8778b21a48fd454a43d4761abf8317c9c75a079fb06"))
    (1300000, uint256("590f1fc1c7015a36b198f75ed1ad0263db076b30fed67af8ebb5599225f9a732"))
    (1400000, uint256("831b9b7de3d73291cba9c33967f5c71e63632660fa57a15558a65136059b89c0"))
    (1500000, uint256("f35e805e1c4b8f93bce2090dd094b71975453e098c0cc647d40042d8177d5e6c"))
    (1600000, uint256("80be5bc4ecfe07baaacb66e6aaa735a75164a0d860bba533ac7378f5aa25de2c"))
    (1700000, uint256("d96aa73d3085cbc6fd89e10a7c2a0e2a15fd4db210c3ded69880b4f1f1699296"))
    (1800000, uint256("ac46b02d147c2d53f93377d356b854fd6c6fbe195c88a27d6596748e158b43cb"))
    (1830000, uint256("6166e5d8e3f60308ef234c24744186efc01a67206c5282e8c6d1d66bfaf94479"))
    (1850000, uint256("67a5a0bf3d6a887a19be8ffe023399a779cd1ae77897e55fc6caa4aead5213e7"))
    (1870000, uint256("bc52599aac461de34399e8ab6e9d2db83043cadd10c0e81ef30acdddeddab3ab"))
    (1890000, uint256("cc2986bcb6be4bbfb046454a0841d77066692e0bf5faf4368355ced334a62947"))
    (1910000, uint256("19b251a1644ecf5a014ddd06de63e0df8fb73b7570d5be355dff0c25197a152d"))
    (1930000, uint256("e81fe446519bfd6b543557584af4c824c820b54e3888890a6c330db742298224"))
    (1950000, uint256("1d67f34bb91c1d437eaa9d76ba10520b8c8a0252683c1e679426c5583584d57c"))
    (1955640, uint256("eaa89514ba7abb7516871599c92ebb788a8f8ef5c156a8b69698b874fb1db777"));

static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1646999284, // * UNIX timestamp of last checkpoint block
    3963163,    // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    2000        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1740710,
    0,
    250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1454124731,
    0,
    100};

libzerocoin::ZerocoinParams* CChainParams::Zerocoin_Params(bool useModulusV1) const
{
    assert(this);
    static CBigNum bnHexModulus = 0;
    if (!bnHexModulus)
        bnHexModulus.SetHex(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParamsHex = libzerocoin::ZerocoinParams(bnHexModulus);
    static CBigNum bnDecModulus = 0;
    if (!bnDecModulus)
        bnDecModulus.SetDec(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParamsDec = libzerocoin::ZerocoinParams(bnDecModulus);

    if (useModulusV1)
        return &ZCParamsHex;

    return &ZCParamsDec;
}

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0x23;
        pchMessageStart[2] = 0x43;
        pchMessageStart[3] = 0x65;
        vAlertPubKey = ParseHex("03f012092c5fe9ed406b43316fc87d8ace9e8eb7764999db00ef60009ddddfa723");
        nDefaultPort = 23511;
        bnProofOfWorkLimit = ~uint256(0) >> 16;
        nMaxReorganizationDepth = 100;
        nEnforceBlockUpgradeMajority = 8100; // 75%
        nRejectBlockOutdatedMajority = 10260; // 95%
        nToCheckBlockUpgradeMajority = 10800; // Approximate expected amount of blocks in 7 days (1440*7.5)
        nMinerThreads = 0;
        nTargetTimespan = 10 * 60; // Myce: 10 minutes
        nTargetSpacing = 64; // Myce: 64 seconds
        nMaturity = 5;
        nMasternodeCountDrift = 20;
        nMaxMoneyOut = 250000000 * COIN;

        /** Height or Time Based Activations **/
        nLastPOWBlock = 200000;
        nPOSStartBlock = 10000;
        nMandatoryUpgradeBlock = 378500;
        nUpgradeBlockVersion = 8;                                   // Block headers must be this version after upgrade block
        nZerocoinStartHeight = 2100000000;                          // OFF
        //nZerocoinStartTime = 4000000000;
        nModifierUpdateBlock = 710000;
        nBlockEnforceSerialRange = -1;                              // Enforce serial range starting this block
        nBlockRecalculateAccumulators = nZerocoinStartHeight + 10;  // Trigger a recalculation of accumulators
        nBlockFirstFraudulent = nZerocoinStartHeight;               // First block that bad serials emerged
        nBlockLastGoodCheckpoint = nZerocoinStartHeight;            // Last valid accumulator checkpoint
        nBlockEnforceInvalidUTXO = -1;                              // Start enforcing the invalid UTXO's
        nInvalidAmountFiltered = 0*COIN;                            // Amount of invalid coins filtered through exchanges, that should be considered valid
        nBlockZerocoinV2 = nZerocoinStartHeight + 20;               // The block that zerocoin v2 becomes active
        nEnforceNewSporkKey = 1559844000;                           // Sporks signed after (GMT): Thursday, June 6, 2019 18:00:00 GMT must use the new spork key
        nRejectOldSporkKey = nEnforceNewSporkKey;                   // Fully reject old spork key after (GMT): Thursday, June 6, 2019 18:00:00

        /**
         * Build the genesis block. Note that the output of the genesis coinbase cannot
         * be spent as it did not originally exist in the database.
         *
         * CBlock(hash=00000ffd590b14, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=e0028e, nTime=1390095618, nBits=1e0ffff0, nNonce=28917698, vtx=1)
         *   CTransaction(hash=e0028e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
         *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d01044c5957697265642030392f4a616e2f3230313420546865204772616e64204578706572696d656e7420476f6573204c6976653a204f76657273746f636b2e636f6d204973204e6f7720416363657074696e6720426974636f696e73)
         *     CTxOut(nValue=50.00000000, scriptPubKey=0xA9037BAC7050C479B121CF)
         *   vMerkleTree: e0028e
         */
        const char* pszTimestamp = "Myce masternodes";
        CMutableTransaction txNew;
        txNew.nVersion = 1;
        txNew.nTime = 1515436125;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 0 << CScriptNum(42) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].SetEmpty();
        //txNew.vout[0].scriptPubKey = CScript() << ParseHex("04c10e83b2703ccf322f7dbd62dd5855ac7c10bd055814ce121ba32607d573b8810c02c0582aed05b4deb9c4b77b26d92428c61256cd42774babea0a073b2ed0c9") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime = 1519813976;
        genesis.nBits = 0x1f00ffff;
        genesis.nNonce = 209474;

        hashGenesisBlock = genesis.GetHash();

        assert(genesis.hashMerkleRoot == uint256("0x8ea58063fe9e14c45dbf5efab5b3812291074f6b1af5305b7b45647bdbfdea90"));
        assert(hashGenesisBlock == uint256("0x0000c74cc66c72cb1a327c5c1d4893ae5276aa50be49fb23cec21df1a2f20d87"));

        vSeeds.push_back(CDNSSeedData("1", "yce.zentec.network"));
        vSeeds.push_back(CDNSSeedData("2", "seed1.myce.world"));
        vSeeds.push_back(CDNSSeedData("3", "seed2.myce.world"));
        vSeeds.push_back(CDNSSeedData("4", "seed3.myce.world"));
        vSeeds.push_back(CDNSSeedData("5", "seed4.myce.world"));
        vSeeds.push_back(CDNSSeedData("6", "seed5.myce.world"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 50);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 85);
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 153);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x10)(0x00)(0x00).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 3;
        strSporkKey = "03f012092c5fe9ed406b43316fc87d8ace9e8eb7764999db00ef60009ddddfa723";
        strSporkKeyOld = "0358d5fb8000c49d38aaab6dc5d0c0a0322eff3090eff026963eb819dc3dec8439";
        strObfuscationPoolDummyAddress = "MKuuMqXZj75T2UidnBWotoN8oQavbyVenR";
        nStartMasternodePayments = 1403728576; //Wed, 25 Jun 2014 20:36:16 GMT

        /** Zerocoin */
        zerocoinModulus = "25195908475657893494027183240048398571429282126204032027777137836043662020707595556264018525880784"
            "4069182906412495150821892985591491761845028084891200728449926873928072877767359714183472702618963750149718246911"
            "6507761337985909570009733045974880842840179742910064245869181719511874612151517265463228221686998754918242243363"
            "7259085141865462043576798423387184774447920739934236584823824281198163815010674810451660377306056201619676256133"
            "8441436038339044149526344321901146575444541784240209246165157233507787077498171257724679629263863563732899121548"
            "31438167899885040445364023527381951378636564391212010397122822120720357";
        nMaxZerocoinSpendsPerTransaction = 7; // Assume about 20kb each
        nMinZerocoinMintFee = 1 * CENT; //high fee required for zerocoin mints
        nMintRequiredConfirmations = 20; //the maximum amount of confirmations until accumulated in 19
        nRequiredAccumulation = 1;
        nDefaultSecurityLevel = 100; //full security level for accumulators
        nZerocoinHeaderVersion = 11; //Block headers must be this version once zerocoin is active
        nZerocoinRequiredStakeDepth = 200; //The required confirmations for a zyce to be stakable

        nBudget_Fee_Confirmations = 6; // Number of confirmations for the finalization fee
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0xfd;
        pchMessageStart[1] = 0x44;
        pchMessageStart[2] = 0x56;
        pchMessageStart[3] = 0x78;
        vAlertPubKey = ParseHex("022b2e19232d63dc26cab94a43576480c36027c9496a7d349fbaad467ce9a7a2b7");
        nDefaultPort = 20114;
        nEnforceBlockUpgradeMajority = 4320; // 75%
        nRejectBlockOutdatedMajority = 5472; // 95%
        nToCheckBlockUpgradeMajority = 5760; // 4 days
        nMinerThreads = 0;
        nTargetTimespan = 10 * 60; // Myce: 10 minutes
        nTargetSpacing = 48; // Myce: 48 seconds
        nLastPOWBlock = 2100000000;
        nPOSStartBlock = 100;
        nMaturity = 15;
        nMasternodeCountDrift = 4;
        nModifierUpdateBlock = -1; //approx Mon, 17 Apr 2017 04:00:00 GMT
        nMaxMoneyOut = 43199500 * COIN;
        nZerocoinStartHeight = 2100000000;
        //nZerocoinStartTime = 1501776000;
        nBlockEnforceSerialRange = -1; //Enforce serial range starting this block
        nBlockRecalculateAccumulators = nZerocoinStartHeight + 10; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = nZerocoinStartHeight; //First block that bad serials emerged
        nBlockLastGoodCheckpoint = nZerocoinStartHeight; //Last valid accumulator checkpoint
        nBlockEnforceInvalidUTXO = -1; //Start enforcing the invalid UTXO's
        nInvalidAmountFiltered = 0; //Amount of invalid coins filtered through exchanges, that should be considered valid
        nBlockZerocoinV2 = nZerocoinStartHeight + 20; //!> The block that zerocoin v2 becomes active
        nEnforceNewSporkKey = 1521604800; //!> Sporks signed after Wednesday, March 21, 2018 4:00:00 AM GMT must use the new spork key
        nRejectOldSporkKey = nEnforceNewSporkKey; //!> Reject old spork key after Saturday, March 31, 2018 12:00:00 AM GMT

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        //genesis.nTime = 1454124731;
        genesis.nBits = 0x1f00ffff;
        genesis.nNonce = 209474;

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x0000c74cc66c72cb1a327c5c1d4893ae5276aa50be49fb23cec21df1a2f20d87"));

        vFixedSeeds.clear();
        vSeeds.clear();

        vSeeds.push_back(CDNSSeedData("1", "testnet.myce.world"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 139); // Testnet myce addresses start with 'x' or 'y'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 19);  // Testnet myce script addresses start with '8' or '9'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);     // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        // Testnet myce BIP32 pubkeys start with 'DRKV'
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x3a)(0x80)(0x61)(0xa0).convert_to_container<std::vector<unsigned char> >();
        // Testnet myce BIP32 prvkeys start with 'DRKP'
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x3a)(0x80)(0x58)(0x37).convert_to_container<std::vector<unsigned char> >();
        // Testnet myce BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 2;
        strSporkKey = "022b2e19232d63dc26cab94a43576480c36027c9496a7d349fbaad467ce9a7a2b7";
        strSporkKeyOld = "022b2e19232d63dc26cab94a43576480c36027c9496a7d349fbaad467ce9a7a2b7";
        strObfuscationPoolDummyAddress = "y9zEimieLbYccV8jAVuD1EcsfPfm3gywyK";
        nStartMasternodePayments = 1420837558; //Fri, 09 Jan 2015 21:05:58 GMT
        nBudget_Fee_Confirmations = 3; // Number of confirmations for the finalization fee. We have to make this very short
                                       // here because we only have a 8 block finalization window on testnet
        //nZerocoinHeaderVersion = 11; //Block headers must be this version once zerocoin is active
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        strNetworkID = "regtest";
        pchMessageStart[0] = 0xa1;
        pchMessageStart[1] = 0xcf;
        pchMessageStart[2] = 0x7e;
        pchMessageStart[3] = 0xac;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetTimespan = 10 * 60; // Myce: 10 minutes
        nTargetSpacing = 48; // Myce: 48 seconds
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        genesis.nTime = 1454124731;
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 12345;

        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 51476;
        assert(hashGenesisBlock == uint256("0xc2854de687b4aa9d0113c2d7eeac592c540d1fc5683936266704f639d6fa7125"));

        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 51478;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority) { nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority) { nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority) { nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) { fAllowMinDifficultyBlocks = afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams* pCurrentParams = 0;

CModifiableParams* ModifiableParams()
{
    assert(pCurrentParams);
    assert(pCurrentParams == &unitTestParams);
    return (CModifiableParams*)&unitTestParams;
}

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    case CBaseChainParams::UNITTEST:
        return unitTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
