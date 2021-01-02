// Copyright (c) 2020 barrystyle
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DIGIBYTE_MONEROHEADER_H
#define DIGIBYTE_MONEROHEADER_H

#include <crypto/randomx/randomx.h>
#include <primitives/block.h>

void serialize_monero_hash(const char* input, char* output, rxhash* rxinstance, int height);

#endif // DIGIBYTE_MONEROHEADER_H
