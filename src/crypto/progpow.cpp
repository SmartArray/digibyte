// Copyright (c) 2020 barrystyle
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <crypto/progpow.h>

#include <hash.h>
#include <chainparams.h>
#include <crypto/seedmgr.h>
#include <crypto/ethash/helpers.hpp>
#include <primitives/block.h>

void progpowhash::hash(const CBlockHeader& header, uint256& hash, int height)
{
     //! make a copy without nonce
     CBlockHeader testHeader = header;
     testHeader.nNonce = 0;

     //! equiv of SerializeHash
     CHashWriter ss(SER_GETHASH, PROTOCOL_VERSION);
     ss << testHeader;
     const ethash::hash256 headerhash = to_hash256(ss.GetHash().ToString());

     //! calculate progpow hash
     uint64_t wideNonce = header.nNonce;
     const auto& ctx = ethash::get_global_epoch_context(ethash::get_epoch_number(height));
     const auto& etresult = progpow::hash(ctx, height, headerhash, wideNonce);
     std::string result = to_hex(etresult.final_hash);
     hash = uint256S(result);
}

