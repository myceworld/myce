// Copyright (c) 2012-2013 The Bitcoin Core developers
// Copyright (c) 2017 The PIVX developers
// Copyright (c) 2018 The Myce developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "rpcserver.h"
#include "rpcclient.h"

#include "base58.h"
#include "netbase.h"
#include "util.h"

#include <boost/algorithm/string.hpp>
#include <boost/test/unit_test.hpp>

#include <univalue.h>

using namespace std;

UniValue
createArgs(int nRequired, const char* address1=NULL, const char* address2=NULL)
{
    UniValue result(UniValue::VARR);
    result.push_back(nRequired);
    UniValue addresses(UniValue::VARR);
    if (address1) addresses.push_back(address1);
    if (address2) addresses.push_back(address2);
    result.push_back(addresses);
    return result;
}

UniValue CallRPC(string args)
{
    vector<string> vArgs;
    boost::split(vArgs, args, boost::is_any_of(" \t"));
    string strMethod = vArgs[0];
    vArgs.erase(vArgs.begin());
    UniValue params = RPCConvertValues(strMethod, vArgs);

    rpcfn_type method = tableRPC[strMethod]->actor;
    try {
        UniValue result = (*method)(params, false);
        return result;
    }
    catch (const UniValue& objError) {
        throw runtime_error(find_value(objError, "message").get_str());
    }
}


BOOST_AUTO_TEST_SUITE(rpc_tests)

BOOST_AUTO_TEST_CASE(rpc_rawparams)
{
    // Test raw transaction API argument handling
    UniValue r;

    BOOST_CHECK_THROW(CallRPC("getrawtransaction"), runtime_error);
    BOOST_CHECK_THROW(CallRPC("getrawtransaction not_hex"), runtime_error);
    BOOST_CHECK_THROW(CallRPC("getrawtransaction a3b807410df0b60fcb9736768df5823938b2f838694939ba45f3c0a1bff150ed not_int"), runtime_error);

    BOOST_CHECK_THROW(CallRPC("createrawtransaction"), runtime_error);
    BOOST_CHECK_THROW(CallRPC("createrawtransaction null null"), runtime_error);
    BOOST_CHECK_THROW(CallRPC("createrawtransaction not_array"), runtime_error);
    BOOST_CHECK_THROW(CallRPC("createrawtransaction [] []"), runtime_error);
    BOOST_CHECK_THROW(CallRPC("createrawtransaction {} {}"), runtime_error);
    BOOST_CHECK_NO_THROW(CallRPC("createrawtransaction [] {}"));
    BOOST_CHECK_THROW(CallRPC("createrawtransaction [] {} extra"), runtime_error);

    BOOST_CHECK_THROW(CallRPC("decoderawtransaction"), runtime_error);
    BOOST_CHECK_THROW(CallRPC("decoderawtransaction null"), runtime_error);
    BOOST_CHECK_THROW(CallRPC("decoderawtransaction DEADBEEF"), runtime_error);
    string rawtx = "01000000761c985a010000000000000000000000000000000000000000000000000000000000000000ffffffff03510101ffffffff0100ba1dd2050000001976a9143486936725524200810a429852fd1af23967b8fb88ac00000000";
    BOOST_CHECK_NO_THROW(r = CallRPC(string("decoderawtransaction ")+rawtx));
    BOOST_CHECK_EQUAL(find_value(r.get_obj(), "version").get_int(), 1);
    BOOST_CHECK_EQUAL(find_value(r.get_obj(), "locktime").get_int(), 0);
    BOOST_CHECK_THROW(r = CallRPC(string("decoderawtransaction ")+rawtx+" extra"), runtime_error);

    BOOST_CHECK_THROW(CallRPC("signrawtransaction"), runtime_error);
    BOOST_CHECK_THROW(CallRPC("signrawtransaction null"), runtime_error);
    BOOST_CHECK_THROW(CallRPC("signrawtransaction ff00"), runtime_error);
    BOOST_CHECK_NO_THROW(CallRPC(string("signrawtransaction ")+rawtx));
    BOOST_CHECK_NO_THROW(CallRPC(string("signrawtransaction ")+rawtx+" null null NONE|ANYONECANPAY"));
    BOOST_CHECK_NO_THROW(CallRPC(string("signrawtransaction ")+rawtx+" [] [] NONE|ANYONECANPAY"));
    BOOST_CHECK_THROW(CallRPC(string("signrawtransaction ")+rawtx+" null null badenum"), runtime_error);

    // Only check failure cases for sendrawtransaction, there's no network to send to...
    BOOST_CHECK_THROW(CallRPC("sendrawtransaction"), runtime_error);
    BOOST_CHECK_THROW(CallRPC("sendrawtransaction null"), runtime_error);
    BOOST_CHECK_THROW(CallRPC("sendrawtransaction DEADBEEF"), runtime_error);
    BOOST_CHECK_THROW(CallRPC(string("sendrawtransaction ")+rawtx+" extra"), runtime_error);
}

BOOST_AUTO_TEST_CASE(rpc_rawsign)
{
    UniValue r;
    // input is a 1-of-2 multisig (so is output):
    string prevout =
      "[{\"txid\":\"a46c7d66a7375e2e9c7e599eaba134e3615a28f0f8ef7abb67ee39d94df402d5\","
      "\"vout\":0,\"scriptPubKey\":\"a91442701e136e3511edf8f6ad98caf412a5fcb2e96787\","
      "\"redeemScript\":\"512103391ef29de5f845563101d04f5ec0700792ba29dea926b24ebd6d3e13a0801cb721033d28760dd79f96ec868b55be26a9e55dbc82d5c824c01dd0f81df128901f3a2252ae\"}]";
    r = CallRPC(string("createrawtransaction ")+prevout+" "+
      "{\"MDnRKKAMY4oqGT1dshkmp1wwEU4ZVr9x1k\":1}");
    string notsigned = r.get_str();
    string privkey1 = "\"PfeoHnJAgZ5hZK1oAYkGadxY86xvprPHfaF3ccmn1FGwFdrDFZt8\"";
    string privkey2 = "\"PjPR8syC4ae4ZmwWD93j5RuBvEExSJkfcU8HFPkCR4Cr626vSZcJ\"";
    r = CallRPC(string("signrawtransaction ")+notsigned+" "+prevout+" "+"[]");
    BOOST_CHECK(find_value(r.get_obj(), "complete").get_bool() == false);
    r = CallRPC(string("signrawtransaction ")+notsigned+" "+prevout+" "+"["+privkey1+","+privkey2+"]");
    BOOST_CHECK(find_value(r.get_obj(), "complete").get_bool() == true);
}

BOOST_AUTO_TEST_CASE(rpc_format_monetary_values)
{
    BOOST_CHECK(ValueFromAmount(0LL).write() == "0.00000000");
    BOOST_CHECK(ValueFromAmount(1LL).write() == "0.00000001");
    BOOST_CHECK(ValueFromAmount(17622195LL).write() == "0.17622195");
    BOOST_CHECK(ValueFromAmount(50000000LL).write() == "0.50000000");
    BOOST_CHECK(ValueFromAmount(89898989LL).write() == "0.89898989");
    BOOST_CHECK(ValueFromAmount(100000000LL).write() == "1.00000000");
    BOOST_CHECK(ValueFromAmount(2099999999999990LL).write() == "20999999.99999990");
    BOOST_CHECK(ValueFromAmount(2099999999999999LL).write() == "20999999.99999999");

    BOOST_CHECK_EQUAL(ValueFromAmount(0).write(), "0.00000000");
    BOOST_CHECK_EQUAL(ValueFromAmount((COIN/10000)*123456789).write(), "12345.67890000");
    BOOST_CHECK_EQUAL(ValueFromAmount(-COIN).write(), "-1.00000000");
    BOOST_CHECK_EQUAL(ValueFromAmount(-COIN/10).write(), "-0.10000000");

    BOOST_CHECK_EQUAL(ValueFromAmount(COIN*100000000).write(), "100000000.00000000");
    BOOST_CHECK_EQUAL(ValueFromAmount(COIN*10000000).write(), "10000000.00000000");
    BOOST_CHECK_EQUAL(ValueFromAmount(COIN*1000000).write(), "1000000.00000000");
    BOOST_CHECK_EQUAL(ValueFromAmount(COIN*100000).write(), "100000.00000000");
    BOOST_CHECK_EQUAL(ValueFromAmount(COIN*10000).write(), "10000.00000000");
    BOOST_CHECK_EQUAL(ValueFromAmount(COIN*1000).write(), "1000.00000000");
    BOOST_CHECK_EQUAL(ValueFromAmount(COIN*100).write(), "100.00000000");
    BOOST_CHECK_EQUAL(ValueFromAmount(COIN*10).write(), "10.00000000");
    BOOST_CHECK_EQUAL(ValueFromAmount(COIN).write(), "1.00000000");
    BOOST_CHECK_EQUAL(ValueFromAmount(COIN/10).write(), "0.10000000");
    BOOST_CHECK_EQUAL(ValueFromAmount(COIN/100).write(), "0.01000000");
    BOOST_CHECK_EQUAL(ValueFromAmount(COIN/1000).write(), "0.00100000");
    BOOST_CHECK_EQUAL(ValueFromAmount(COIN/10000).write(), "0.00010000");
    BOOST_CHECK_EQUAL(ValueFromAmount(COIN/100000).write(), "0.00001000");
    BOOST_CHECK_EQUAL(ValueFromAmount(COIN/1000000).write(), "0.00000100");
    BOOST_CHECK_EQUAL(ValueFromAmount(COIN/10000000).write(), "0.00000010");
    BOOST_CHECK_EQUAL(ValueFromAmount(COIN/100000000).write(), "0.00000001");
}

static UniValue ValueFromString(const std::string &str)
{
    UniValue value;
    BOOST_CHECK(value.setNumStr(str));
    return value;
}

BOOST_AUTO_TEST_CASE(rpc_parse_monetary_values)
{
    BOOST_CHECK_EQUAL(AmountFromValue(ValueFromString("0.00000001")), 1LL);
    BOOST_CHECK_EQUAL(AmountFromValue(ValueFromString("0.17622195")), 17622195LL);
    BOOST_CHECK_EQUAL(AmountFromValue(ValueFromString("0.5")), 50000000LL);
    BOOST_CHECK_EQUAL(AmountFromValue(ValueFromString("0.50000000")), 50000000LL);
    BOOST_CHECK_EQUAL(AmountFromValue(ValueFromString("0.89898989")), 89898989LL);
    BOOST_CHECK_EQUAL(AmountFromValue(ValueFromString("1.00000000")), 100000000LL);
    BOOST_CHECK_EQUAL(AmountFromValue(ValueFromString("20999999.9999999")), 2099999999999990LL);
    BOOST_CHECK_EQUAL(AmountFromValue(ValueFromString("20999999.99999999")), 2099999999999999LL);
}

BOOST_AUTO_TEST_CASE(json_parse_errors)
{
    // Valid
    BOOST_CHECK_EQUAL(ParseNonRFCJSONValue("1.0").get_real(), 1.0);
    // Valid, with leading or trailing whitespace
    BOOST_CHECK_EQUAL(ParseNonRFCJSONValue(" 1.0").get_real(), 1.0);
    BOOST_CHECK_EQUAL(ParseNonRFCJSONValue("1.0 ").get_real(), 1.0);
    // Invalid, initial garbage
    BOOST_CHECK_THROW(ParseNonRFCJSONValue("[1.0"), std::runtime_error);
    BOOST_CHECK_THROW(ParseNonRFCJSONValue("a1.0"), std::runtime_error);
    // Invalid, trailing garbage
    BOOST_CHECK_THROW(ParseNonRFCJSONValue("1.0sds"), std::runtime_error);
    BOOST_CHECK_THROW(ParseNonRFCJSONValue("1.0]"), std::runtime_error);
    // BTC addresses should fail parsing
    BOOST_CHECK_THROW(ParseNonRFCJSONValue("175tWpb8K1S7NmH4Zx6rewF9WQrcZv245W"), std::runtime_error);
    BOOST_CHECK_THROW(ParseNonRFCJSONValue("3J98t1WpEZ73CNmQviecrnyiWrnqRhWNL"), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(rpc_ban)
{
    BOOST_CHECK_NO_THROW(CallRPC(string("clearbanned")));

    UniValue r;
    BOOST_CHECK_NO_THROW(r = CallRPC(string("setban 127.0.0.0 add")));
    BOOST_CHECK_THROW(r = CallRPC(string("setban 127.0.0.0:8334")), runtime_error); //portnumber for setban not allowed
    BOOST_CHECK_NO_THROW(r = CallRPC(string("listbanned")));
    UniValue ar = r.get_array();
    UniValue o1 = ar[0].get_obj();
    UniValue adr = find_value(o1, "address");
    BOOST_CHECK_EQUAL(adr.get_str(), "127.0.0.0/32");
    BOOST_CHECK_NO_THROW(CallRPC(string("setban 127.0.0.0 remove")));;
    BOOST_CHECK_NO_THROW(r = CallRPC(string("listbanned")));
    ar = r.get_array();
    BOOST_CHECK_EQUAL(ar.size(), 0);

    BOOST_CHECK_NO_THROW(r = CallRPC(string("setban 127.0.0.0/24 add 1607731200 true")));
    BOOST_CHECK_NO_THROW(r = CallRPC(string("listbanned")));
    ar = r.get_array();
    o1 = ar[0].get_obj();
    adr = find_value(o1, "address");
    UniValue banned_until = find_value(o1, "banned_until");
    BOOST_CHECK_EQUAL(adr.get_str(), "127.0.0.0/24");
    BOOST_CHECK_EQUAL(banned_until.get_int64(), 1607731200); // absolute time check

    BOOST_CHECK_NO_THROW(CallRPC(string("clearbanned")));

    BOOST_CHECK_NO_THROW(r = CallRPC(string("setban 127.0.0.0/24 add 200")));
    BOOST_CHECK_NO_THROW(r = CallRPC(string("listbanned")));
    ar = r.get_array();
    o1 = ar[0].get_obj();
    adr = find_value(o1, "address");
    banned_until = find_value(o1, "banned_until");
    BOOST_CHECK_EQUAL(adr.get_str(), "127.0.0.0/24");
    int64_t now = GetTime();
    BOOST_CHECK(banned_until.get_int64() > now);
    BOOST_CHECK(banned_until.get_int64()-now <= 200);

    // must throw an exception because 127.0.0.1 is in already banned suubnet range
    BOOST_CHECK_THROW(r = CallRPC(string("setban 127.0.0.1 add")), runtime_error);

    BOOST_CHECK_NO_THROW(CallRPC(string("setban 127.0.0.0/24 remove")));;
    BOOST_CHECK_NO_THROW(r = CallRPC(string("listbanned")));
    ar = r.get_array();
    BOOST_CHECK_EQUAL(ar.size(), 0);

    BOOST_CHECK_NO_THROW(r = CallRPC(string("setban 127.0.0.0/255.255.0.0 add")));
    BOOST_CHECK_THROW(r = CallRPC(string("setban 127.0.1.1 add")), runtime_error);

    BOOST_CHECK_NO_THROW(CallRPC(string("clearbanned")));
    BOOST_CHECK_NO_THROW(r = CallRPC(string("listbanned")));
    ar = r.get_array();
    BOOST_CHECK_EQUAL(ar.size(), 0);

    BOOST_CHECK_THROW(r = CallRPC(string("setban test add")), runtime_error); //invalid IP

    //IPv6 tests
    BOOST_CHECK_NO_THROW(r = CallRPC(string("setban FE80:0000:0000:0000:0202:B3FF:FE1E:8329 add")));
    BOOST_CHECK_NO_THROW(r = CallRPC(string("listbanned")));
    ar = r.get_array();
    o1 = ar[0].get_obj();
    adr = find_value(o1, "address");
    BOOST_CHECK_EQUAL(adr.get_str(), "fe80::202:b3ff:fe1e:8329/128");

    BOOST_CHECK_NO_THROW(CallRPC(string("clearbanned")));
    BOOST_CHECK_NO_THROW(r = CallRPC(string("setban 2001:db8::/ffff:fffc:0:0:0:0:0:0 add")));
    BOOST_CHECK_NO_THROW(r = CallRPC(string("listbanned")));
    ar = r.get_array();
    o1 = ar[0].get_obj();
    adr = find_value(o1, "address");
    BOOST_CHECK_EQUAL(adr.get_str(), "2001:db8::/30");

    BOOST_CHECK_NO_THROW(CallRPC(string("clearbanned")));
    BOOST_CHECK_NO_THROW(r = CallRPC(string("setban 2001:4d48:ac57:400:cacf:e9ff:fe1d:9c63/128 add")));
    BOOST_CHECK_NO_THROW(r = CallRPC(string("listbanned")));
    ar = r.get_array();
    o1 = ar[0].get_obj();
    adr = find_value(o1, "address");
    BOOST_CHECK_EQUAL(adr.get_str(), "2001:4d48:ac57:400:cacf:e9ff:fe1d:9c63/128");
}

BOOST_AUTO_TEST_SUITE_END()
