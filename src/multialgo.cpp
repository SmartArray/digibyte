// Copyright (c) 2014-2020 The Digibyte Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chain.h>
#include <chainparams.h>
#include <multialgo.h>
#include <util/system.h>

int GetAlgoWorkFactor(int nHeight, int algo)
{
    if (nHeight < Params().GetConsensus().multiAlgoDiffChangeTarget)
        return 1;

    switch (algo) {
    case ALGO_SHA256D:
        return 1;
    case ALGO_SCRYPT:
        return 1024 * 4;
    case ALGO_GROESTL:
        return 64 * 8;
    case ALGO_SKEIN:
        return 4 * 6;
    case ALGO_QUBIT:
        return 128 * 8;
    case ALGO_RANDOMX:
        return 1;
    case ALGO_PROGPOW:
        return 1;
    default:
        return 1;
    }
}

int GetVersionForAlgo(int algo)
{
    switch (algo) {
    case ALGO_SHA256D:
        return BLOCK_VERSION_SHA256D;
    case ALGO_SCRYPT:
        return BLOCK_VERSION_SCRYPT;
    case ALGO_GROESTL:
        return BLOCK_VERSION_GROESTL;
    case ALGO_SKEIN:
        return BLOCK_VERSION_SKEIN;
    case ALGO_QUBIT:
        return BLOCK_VERSION_QUBIT;
    case ALGO_ODO:
        return BLOCK_VERSION_ODO;
    case ALGO_RANDOMX:
        return BLOCK_VERSION_RANDOMX;
    case ALGO_PROGPOW:
        return BLOCK_VERSION_PROGPOW;
    default:
        return 0;
    }
}

int GetAlgo(int nVersion)
{
    switch (nVersion & BLOCK_VERSION_ALGO) {
    case BLOCK_VERSION_SCRYPT:
        return ALGO_SCRYPT;
    case BLOCK_VERSION_SHA256D:
        return ALGO_SHA256D;
    case BLOCK_VERSION_GROESTL:
        return ALGO_GROESTL;
    case BLOCK_VERSION_SKEIN:
        return ALGO_SKEIN;
    case BLOCK_VERSION_QUBIT:
        return ALGO_QUBIT;
    case BLOCK_VERSION_ODO:
        return ALGO_ODO;
    case BLOCK_VERSION_RANDOMX:
        return ALGO_RANDOMX;
    case BLOCK_VERSION_PROGPOW:
        return ALGO_PROGPOW;
    }
    return ALGO_UNKNOWN;
}

std::string GetAlgoName(int Algo)
{
    switch (Algo) {
    case ALGO_SHA256D:
        return std::string("sha256d");
    case ALGO_SCRYPT:
        return std::string("scrypt");
    case ALGO_GROESTL:
        return std::string("groestl");
    case ALGO_SKEIN:
        return std::string("skein");
    case ALGO_QUBIT:
        return std::string("qubit");
    case ALGO_ODO:
        return std::string("odo");
    case ALGO_RANDOMX:
        return std::string("randomx");
    case ALGO_PROGPOW:
        return std::string("progpow");
    }
    return std::string("unknown");
}

int GetAlgoByIndex(const CBlockIndex* pindex)
{
    return GetAlgo(pindex->GetBlockHeader().nVersion);
}

std::string GetAlgoNameByIndex(const CBlockIndex* pindex)
{
    int hashalgo = GetAlgo(pindex->GetBlockHeader().nVersion);
    return GetAlgoName(hashalgo);
}

int GetAlgoByName(std::string strAlgo, int fallback)
{
    transform(strAlgo.begin(), strAlgo.end(), strAlgo.begin(), ::tolower);
    if (strAlgo == "sha" || strAlgo == "sha256" || strAlgo == "sha256d")
        return ALGO_SHA256D;
    else if (strAlgo == "scrypt")
        return ALGO_SCRYPT;
    else if (strAlgo == "groestl" || strAlgo == "groestlsha2")
        return ALGO_GROESTL;
    else if (strAlgo == "skein" || strAlgo == "skeinsha2")
        return ALGO_SKEIN;
    else if (strAlgo == "q2c" || strAlgo == "qubit")
        return ALGO_QUBIT;
    else if (strAlgo == "odo" || strAlgo == "odosha3")
        return ALGO_ODO;
    else if (strAlgo == "rxhash" || strAlgo == "randomx")
        return ALGO_RANDOMX;
    else if (strAlgo == "progpow")
        return ALGO_PROGPOW;
    else
        return fallback;
}

//! for special algorithms

uint32_t OdoKey(const Consensus::Params& params, uint32_t nTime)
{
    uint32_t nShapechangeInterval = params.nOdoShapechangeInterval;
    return nTime - nTime % nShapechangeInterval;
}
