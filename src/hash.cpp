// Copyright (c) 2013-2014 The Bitcoin developers
// Copyright (c) 2017 The PIVX developers
// Copyright (c) 2018 The Myce developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "hash.h"
#include "crypto/hmac_sha512.h"
#include "crypto/scrypt.h"

inline uint32_t ROTL32(uint32_t x, int8_t r)
{
    return (x << r) | (x >> (32 - r));
}

unsigned int MurmurHash3(unsigned int nHashSeed, const std::vector<unsigned char>& vDataToHash)
{
    // The following is MurmurHash3 (x86_32), see http://code.google.com/p/smhasher/source/browse/trunk/MurmurHash3.cpp
    uint32_t h1 = nHashSeed;
    if (vDataToHash.size() > 0) {
        const uint32_t c1 = 0xcc9e2d51;
        const uint32_t c2 = 0x1b873593;

        const int nblocks = vDataToHash.size() / 4;

        //----------
        // body
        const uint32_t* blocks = (const uint32_t*)(&vDataToHash[0] + nblocks * 4);

        for (int i = -nblocks; i; i++) {
            uint32_t k1 = blocks[i];

            k1 *= c1;
            k1 = ROTL32(k1, 15);
            k1 *= c2;

            h1 ^= k1;
            h1 = ROTL32(h1, 13);
            h1 = h1 * 5 + 0xe6546b64;
        }

        //----------
        // tail
        const uint8_t* tail = (const uint8_t*)(&vDataToHash[0] + nblocks * 4);

        uint32_t k1 = 0;

        switch (vDataToHash.size() & 3) {
        case 3:
            k1 ^= tail[2] << 16;
        case 2:
            k1 ^= tail[1] << 8;
        case 1:
            k1 ^= tail[0];
            k1 *= c1;
            k1 = ROTL32(k1, 15);
            k1 *= c2;
            h1 ^= k1;
        };
    }

    //----------
    // finalization
    h1 ^= vDataToHash.size();
    h1 ^= h1 >> 16;
    h1 *= 0x85ebca6b;
    h1 ^= h1 >> 13;
    h1 *= 0xc2b2ae35;
    h1 ^= h1 >> 16;

    return h1;
}

void BIP32Hash(const ChainCode chainCode, unsigned int nChild, unsigned char header, const unsigned char data[32], unsigned char output[64])
{
    unsigned char num[4];
    num[0] = (nChild >> 24) & 0xFF;
    num[1] = (nChild >> 16) & 0xFF;
    num[2] = (nChild >> 8) & 0xFF;
    num[3] = (nChild >> 0) & 0xFF;
    CHMAC_SHA512(chainCode.begin(), chainCode.size()).Write(&header, 1).Write(data, 32).Write(num, 4).Finalize(output);
}

uint256 scrypt_hash(const void* input, size_t inputlen)
{
    uint256 result = 0;

    scrypt((const char*)input, inputlen, (const char*)input, inputlen, (char*)&result, 1024, 1, 1, 32);

    return result;
}

uint256 scrypt_salted_hash(const void* input, size_t inputlen, const void* salt, size_t saltlen)
{
    uint256 result = 0;

    scrypt((const char*)input, inputlen, (const char*)salt, saltlen, (char*)&result, 1024, 1, 1, 32);

    return result;
}

uint256 scrypt_salted_multiround_hash(const void* input, size_t inputlen, const void* salt, size_t saltlen, const unsigned int nRounds)
{
    uint256 resultHash = scrypt_salted_hash(input, inputlen, salt, saltlen);
    uint256 transitionalHash = resultHash;

    for (unsigned int i = 1; i < nRounds; i++)
    {
        resultHash = scrypt_salted_hash(input, inputlen, (const void*)&transitionalHash, 32);
        transitionalHash = resultHash;
    }

    return resultHash;
}

uint256 scrypt_blockhash(const void* input)
{
    return scrypt_hash(input, 80);
}

void scrypt_hash(const char* pass, unsigned int pLen, const char* salt, unsigned int sLen, char* output, unsigned int N, unsigned int r, unsigned int p, unsigned int dkLen)
{
    scrypt(pass, pLen, salt, sLen, output, N, r, p, dkLen);
}