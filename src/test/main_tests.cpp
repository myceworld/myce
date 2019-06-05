// Copyright (c) 2014 The Bitcoin Core developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2017 The PIVX developers
// Copyright (c) 2018 The Myce developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "primitives/transaction.h"
#include "main.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(main_tests)

BOOST_AUTO_TEST_CASE(subsidy_test)
{
    for (int nHeight = 2; nHeight <= 11; nHeight += 1) {
        /* premine in block 2-11 */
        CAmount nSubsidy = GetBlockValue(nHeight, false);
        BOOST_CHECK(nSubsidy <= 3500000 * COIN);
    }

    for (int nHeight = 12; nHeight <= 20000; nHeight += 1) {
        /* PoW Phase One */
        CAmount nSubsidy = GetBlockValue(nHeight, false);
        BOOST_CHECK(nSubsidy <= 50 * COIN);
    }

    for (int nHeight = 20001; nHeight <= 50000; nHeight += 1) {
        /* PoW Phase Two */
        CAmount nSubsidy = GetBlockValue(nHeight, false);
        BOOST_CHECK(nSubsidy <= 25 * COIN);
    }

    for (int nHeight = 50001; nHeight <= 100000; nHeight += 1) {
        /* PoW Phase Two */
        CAmount nSubsidy = GetBlockValue(nHeight, false);
        BOOST_CHECK(nSubsidy <= 20 * COIN);
    }
}

BOOST_AUTO_TEST_SUITE_END()
