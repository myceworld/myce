// Copyright (c) 2012-2013 The Bitcoin Core developers
// Copyright (c) 2017 The PIVX developers
// Copyright (c) 2018 The Myce developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "key.h"

#include "base58.h"
#include "script/script.h"
#include "uint256.h"
#include "util.h"
#include "utilstrencodings.h"

#include <string>
#include <vector>

#include <boost/test/unit_test.hpp>

using namespace std;

static const string strSecret1     ("69BgmPCYt1jAPKsBkGNvdT1WwA1ZgeAj3yhVk8MZ4bhUEHC73zM");
static const string strSecret2     ("68qgYNnddZq7hKnCeEGJYpuGgHLhhb8FaaTDgAWXDmE2JTJpVTw");
static const string strSecret1C    ("Ph8AWtyyiAD6RYGWRxsmhsoH82mVFy3vc9FjsMTiWt9dBBwcU5qA");
static const string strSecret2C    ("PfbsXVAQgPuNYytWPKYYTURiCP7xe9d1MCeiwryJPLCUjfnH6VYN");
static const CBitcoinAddress addr1 ("MWkhf4V2po6BgQJEPA6m1togURBeVSvFyD");
static const CBitcoinAddress addr2 ("MMntZVk1SZrdpc79SJKAuWJN3fz35jEdfX");
static const CBitcoinAddress addr1C("MMW26G5MaJytvshUQyGFdc7DPPqyNvegFq");
static const CBitcoinAddress addr2C("MJ53bUnWe4NpxG95DLF3G64aaiW9F7f6cB");


static const string strAddressBad("Xta1praZQjyELweyMByXyiREw1ZRsjXzVP");


#ifdef KEY_TESTS_DUMPINFO
void dumpKeyInfo(uint256 privkey)
{
    CKey key;
    key.resize(32);
    memcpy(&secret[0], &privkey, 32);
    vector<unsigned char> sec;
    sec.resize(32);
    memcpy(&sec[0], &secret[0], 32);
    printf("  * secret (hex): %s\n", HexStr(sec).c_str());

    for (int nCompressed=0; nCompressed<2; nCompressed++)
    {
        bool fCompressed = nCompressed == 1;
        printf("  * %s:\n", fCompressed ? "compressed" : "uncompressed");
        CBitcoinSecret bsecret;
        bsecret.SetSecret(secret, fCompressed);
        printf("    * secret (base58): %s\n", bsecret.ToString().c_str());
        CKey key;
        key.SetSecret(secret, fCompressed);
        vector<unsigned char> vchPubKey = key.GetPubKey();
        printf("    * pubkey (hex): %s\n", HexStr(vchPubKey).c_str());
        printf("    * address (base58): %s\n", CBitcoinAddress(vchPubKey).ToString().c_str());
    }
}
#endif


BOOST_AUTO_TEST_SUITE(key_tests)

BOOST_AUTO_TEST_CASE(key_test1)
{
    CBitcoinSecret bsecret1, bsecret2, bsecret1C, bsecret2C, baddress1;
    BOOST_CHECK( bsecret1.SetString (strSecret1));
    BOOST_CHECK( bsecret2.SetString (strSecret2));
    BOOST_CHECK( bsecret1C.SetString(strSecret1C));
    BOOST_CHECK( bsecret2C.SetString(strSecret2C));
    BOOST_CHECK(!baddress1.SetString(strAddressBad));

    CKey key1  = bsecret1.GetKey();
    BOOST_CHECK(key1.IsCompressed() == false);
    CKey key2  = bsecret2.GetKey();
    BOOST_CHECK(key2.IsCompressed() == false);
    CKey key1C = bsecret1C.GetKey();
    BOOST_CHECK(key1C.IsCompressed() == true);
    CKey key2C = bsecret2C.GetKey();
    BOOST_CHECK(key2C.IsCompressed() == true);

    CPubKey pubkey1  = key1. GetPubKey();
    CPubKey pubkey2  = key2. GetPubKey();
    CPubKey pubkey1C = key1C.GetPubKey();
    CPubKey pubkey2C = key2C.GetPubKey();

    BOOST_CHECK(key1.VerifyPubKey(pubkey1));
    BOOST_CHECK(!key1.VerifyPubKey(pubkey1C));
    BOOST_CHECK(!key1.VerifyPubKey(pubkey2));
    BOOST_CHECK(!key1.VerifyPubKey(pubkey2C));

    BOOST_CHECK(!key1C.VerifyPubKey(pubkey1));
    BOOST_CHECK(key1C.VerifyPubKey(pubkey1C));
    BOOST_CHECK(!key1C.VerifyPubKey(pubkey2));
    BOOST_CHECK(!key1C.VerifyPubKey(pubkey2C));

    BOOST_CHECK(!key2.VerifyPubKey(pubkey1));
    BOOST_CHECK(!key2.VerifyPubKey(pubkey1C));
    BOOST_CHECK(key2.VerifyPubKey(pubkey2));
    BOOST_CHECK(!key2.VerifyPubKey(pubkey2C));

    BOOST_CHECK(!key2C.VerifyPubKey(pubkey1));
    BOOST_CHECK(!key2C.VerifyPubKey(pubkey1C));
    BOOST_CHECK(!key2C.VerifyPubKey(pubkey2));
    BOOST_CHECK(key2C.VerifyPubKey(pubkey2C));

    BOOST_CHECK(addr1.Get()  == CTxDestination(pubkey1.GetID()));
    BOOST_CHECK(addr2.Get()  == CTxDestination(pubkey2.GetID()));
    BOOST_CHECK(addr1C.Get() == CTxDestination(pubkey1C.GetID()));
    BOOST_CHECK(addr2C.Get() == CTxDestination(pubkey2C.GetID()));

    for (int n=0; n<16; n++)
    {
        string strMsg = strprintf("Very secret message %i: 11", n);
        uint256 hashMsg = Hash(strMsg.begin(), strMsg.end());

        // normal signatures

        vector<unsigned char> sign1, sign2, sign1C, sign2C;

        BOOST_CHECK(key1.Sign (hashMsg, sign1));
        BOOST_CHECK(key2.Sign (hashMsg, sign2));
        BOOST_CHECK(key1C.Sign(hashMsg, sign1C));
        BOOST_CHECK(key2C.Sign(hashMsg, sign2C));

        BOOST_CHECK( pubkey1.Verify(hashMsg, sign1));
        BOOST_CHECK(!pubkey1.Verify(hashMsg, sign2));
        BOOST_CHECK( pubkey1.Verify(hashMsg, sign1C));
        BOOST_CHECK(!pubkey1.Verify(hashMsg, sign2C));

        BOOST_CHECK(!pubkey2.Verify(hashMsg, sign1));
        BOOST_CHECK( pubkey2.Verify(hashMsg, sign2));
        BOOST_CHECK(!pubkey2.Verify(hashMsg, sign1C));
        BOOST_CHECK( pubkey2.Verify(hashMsg, sign2C));

        BOOST_CHECK( pubkey1C.Verify(hashMsg, sign1));
        BOOST_CHECK(!pubkey1C.Verify(hashMsg, sign2));
        BOOST_CHECK( pubkey1C.Verify(hashMsg, sign1C));
        BOOST_CHECK(!pubkey1C.Verify(hashMsg, sign2C));

        BOOST_CHECK(!pubkey2C.Verify(hashMsg, sign1));
        BOOST_CHECK( pubkey2C.Verify(hashMsg, sign2));
        BOOST_CHECK(!pubkey2C.Verify(hashMsg, sign1C));
        BOOST_CHECK( pubkey2C.Verify(hashMsg, sign2C));

        // compact signatures (with key recovery)

        vector<unsigned char> csign1, csign2, csign1C, csign2C;

        BOOST_CHECK(key1.SignCompact (hashMsg, csign1));
        BOOST_CHECK(key2.SignCompact (hashMsg, csign2));
        BOOST_CHECK(key1C.SignCompact(hashMsg, csign1C));
        BOOST_CHECK(key2C.SignCompact(hashMsg, csign2C));

        CPubKey rkey1, rkey2, rkey1C, rkey2C;

        BOOST_CHECK(rkey1.RecoverCompact (hashMsg, csign1));
        BOOST_CHECK(rkey2.RecoverCompact (hashMsg, csign2));
        BOOST_CHECK(rkey1C.RecoverCompact(hashMsg, csign1C));
        BOOST_CHECK(rkey2C.RecoverCompact(hashMsg, csign2C));

        BOOST_CHECK(rkey1  == pubkey1);
        BOOST_CHECK(rkey2  == pubkey2);
        BOOST_CHECK(rkey1C == pubkey1C);
        BOOST_CHECK(rkey2C == pubkey2C);
    }

    // test deterministic signing

    std::vector<unsigned char> detsig, detsigc;
    string strMsg = "Very deterministic message";
    uint256 hashMsg = Hash(strMsg.begin(), strMsg.end());
    BOOST_CHECK(key1.Sign(hashMsg, detsig));
    BOOST_CHECK(key1C.Sign(hashMsg, detsigc));
    BOOST_CHECK(detsig == detsigc);
    BOOST_CHECK(detsig == ParseHex("30450221008920A4167FB36FD23E00D7409CD3C8AE9263BD4CC43BBC52351CE41AD29702E202203FBFC9C268F089969F12C6C957FCF6DE1C64E138473454A4E906912B7825E945"));
    BOOST_CHECK(key2.Sign(hashMsg, detsig));
    BOOST_CHECK(key2C.Sign(hashMsg, detsigc));
    BOOST_CHECK(detsig == detsigc);
    BOOST_CHECK(detsig == ParseHex("3045022100D90BB5C040A5B441F5C867A1878E7C6363B6913CCC9BE1A12C800549B3F966AE022064C4D93C7E68160B7A42F6B2A7B56A85BB3D862B71CBA52FEED82BBB8E5EEA76"));
    BOOST_CHECK(key1.SignCompact(hashMsg, detsig));
    BOOST_CHECK(key1C.SignCompact(hashMsg, detsigc));
    BOOST_CHECK(detsig == ParseHex("1C8920A4167FB36FD23E00D7409CD3C8AE9263BD4CC43BBC52351CE41AD29702E23FBFC9C268F089969F12C6C957FCF6DE1C64E138473454A4E906912B7825E945"));
    BOOST_CHECK(detsigc == ParseHex("208920A4167FB36FD23E00D7409CD3C8AE9263BD4CC43BBC52351CE41AD29702E23FBFC9C268F089969F12C6C957FCF6DE1C64E138473454A4E906912B7825E945"));
    BOOST_CHECK(key2.SignCompact(hashMsg, detsig));
    BOOST_CHECK(key2C.SignCompact(hashMsg, detsigc));
    BOOST_CHECK(detsig == ParseHex("1BD90BB5C040A5B441F5C867A1878E7C6363B6913CCC9BE1A12C800549B3F966AE64C4D93C7E68160B7A42F6B2A7B56A85BB3D862B71CBA52FEED82BBB8E5EEA76"));
    BOOST_CHECK(detsigc == ParseHex("1FD90BB5C040A5B441F5C867A1878E7C6363B6913CCC9BE1A12C800549B3F966AE64C4D93C7E68160B7A42F6B2A7B56A85BB3D862B71CBA52FEED82BBB8E5EEA76"));
}

BOOST_AUTO_TEST_SUITE_END()
