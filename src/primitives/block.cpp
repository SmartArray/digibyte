// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2019 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <primitives/block.h>

#include <arith_uint256.h>
#include <chainparams.h>
#include <crypto/hashgroestl.h>
#include <crypto/hashodo.h>
#include <crypto/hashqubit.h>
#include <crypto/hashskein.h>
#include <crypto/progpow.h>
#include <primitives/moneroheader.h>
#include <hash.h>
#include <util/strencodings.h>

std::map<int, uint256> seedcache;

uint256 CBlockHeader::GetHash() const
{
    return SerializeHash(*this);
}

uint256 CBlockHeader::GetPoWAlgoHash(int height) const
{
    const int hashalgo = GetAlgo(this->nVersion);
    const Consensus::Params& params = Params().GetConsensus();

    //! maintain seedcache conservatively
    if (height % params.nEpochLength == 0)
        seedcache[height] = this->hashMerkleRoot;

    switch (hashalgo) {
    case ALGO_SHA256D:
        return GetHash();
    case ALGO_SCRYPT: {
        uint256 thash;
        scrypt_1024_1_1_256(BEGIN(nVersion), BEGIN(thash));
        return thash;
    }
    case ALGO_GROESTL:
        return HashGroestl(BEGIN(nVersion), END(nNonce));
    case ALGO_SKEIN:
        return HashSkein(BEGIN(nVersion), END(nNonce));
    case ALGO_QUBIT:
        return HashQubit(BEGIN(nVersion), END(nNonce));
    case ALGO_ODO: {
        uint32_t key = OdoKey(params, nTime);
        return HashOdo(BEGIN(nVersion), END(nNonce), key);
    }
    case ALGO_RANDOMX: {
        seedmanager.updateheight(height);
        uint256 thash;
        serialize_monero_hash((const char*)this, BEGIN(thash), blk_reader, height);
        return thash;
    }
    case ALGO_PROGPOW: {
        uint256 thash;
        {
            progpowhash instance;
            instance.hash(*this, thash, height);
        }
        return thash;
    }
    case ALGO_UNKNOWN:
        return ArithToUint256(~arith_uint256(0));
    }
    assert(false);
    return GetHash();
}

std::string CBlock::ToString(const Consensus::Params& params) const
{
    std::stringstream s;
    s << strprintf("CBlock(hash=%s, ver=0x%08x, pow_algo=%d, hashPrevBlock=%s, hashMerkleRoot=%s, nTime=%u, nBits=%08x, nNonce=%u, vtx=%u)\n",
        GetHash().ToString(),
        nVersion,
        GetAlgo(nVersion),
        hashPrevBlock.ToString(),
        hashMerkleRoot.ToString(),
        nTime, nBits, nNonce,
        vtx.size());
    for (const auto& tx : vtx) {
        s << "  " << tx->ToString() << "\n";
    }
    return s.str();
}

int64_t GetBlockWeight(const CBlock& block)
{
    // This implements the weight = (stripped_size * 4) + witness_size formula,
    // using only serialization with and without witness data. As witness_size
    // is equal to total_size - stripped_size, this formula is identical to:
    // weight = (stripped_size * 3) + total_size.
    return ::GetSerializeSize(block, PROTOCOL_VERSION | SERIALIZE_TRANSACTION_NO_WITNESS) * (WITNESS_SCALE_FACTOR - 1) + ::GetSerializeSize(block, PROTOCOL_VERSION);
}
