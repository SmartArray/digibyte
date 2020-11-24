// Copyright (c) 2020 barrystyle
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DIGIBYTE_PROGPOW_H
#define DIGIBYTE_PROGPOW_H

#include <crypto/seedmgr.h>
#include <crypto/ethash/include/ethash/ethash.h>
#include <crypto/ethash/include/ethash/progpow.hpp>
#include <uint256.h>

#include <map>

class uint256;
class CBlockHeader;

class progpowhash {
public:
   void hash(const CBlockHeader& header, uint256& hash, int height);
};

#endif // DIGIBYTE_PROGPOW_H
