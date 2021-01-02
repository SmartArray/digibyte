// Copyright (c) 2020 barrystyle
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <crypto/randomx.h>
#include <crypto/randomx/randomx.h>
#include <primitives/block.h>
#include <crypto/scrypt.h>

void serialize_monero_hash(const char* input, char* output, rxhash* rxinstance, int height)
{
    unsigned char moneroHeader[76], midstate[32];
    memset(&moneroHeader, 0, sizeof(moneroHeader));
    SHA256((unsigned char*)&input,76,midstate);
    SHA256((unsigned char*)&midstate,32,moneroHeader);
    memcpy(&moneroHeader[39],&input[76],4);
    rxinstance->hash((const char*)moneroHeader, (char*)&output, 76, height);
}
