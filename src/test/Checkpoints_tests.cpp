// Copyright (c) 2011-2013 The Bitcoin Core developers
// Copyright (c) 2017 The PIVX developers
// Copyright (c) 2018 The Myce developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//
// Unit tests for block-chain checkpoints
//

#include "checkpoints.h"

#include "uint256.h"

#include <boost/test/unit_test.hpp>

using namespace std;

BOOST_AUTO_TEST_SUITE(Checkpoints_tests)

BOOST_AUTO_TEST_CASE(sanity)
{
    uint256 p341500 = uint256("0x769f485854697c20088703e205017b96853f1a0bfbd288110bed45d39a3a60b4");
    uint256 p584800 = uint256("0x73ede8b55f2054c14cf7cbade19fedcc18f5b3ea5d1a931e6f3a996ee46bbf57");
    BOOST_CHECK(Checkpoints::CheckBlock(341500, p341500));
    BOOST_CHECK(Checkpoints::CheckBlock(584800, p584800));


    // Wrong hashes at checkpoints should fail:
    BOOST_CHECK(!Checkpoints::CheckBlock(341500, p584800));
    BOOST_CHECK(!Checkpoints::CheckBlock(584800, p341500));

    // ... but any hash not at a checkpoint should succeed:
    BOOST_CHECK(Checkpoints::CheckBlock(341500+1, p584800));
    BOOST_CHECK(Checkpoints::CheckBlock(584800+1, p341500));

    BOOST_CHECK(Checkpoints::GetTotalBlocksEstimate() >= 584800);
}

BOOST_AUTO_TEST_SUITE_END()
