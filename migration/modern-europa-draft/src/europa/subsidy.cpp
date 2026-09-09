// Europa subsidy compatibility for Bitcoin Core 31.1.
// UNTESTED / DO NOT RELEASE.

#include <europa/subsidy.h>

#include <consensus/params.h>

namespace europa {

CAmount GetBlockSubsidy(const int height, const Consensus::Params& params)
{
    const int halvings = height / params.nSubsidyHalvingInterval;
    if (halvings >= 64) return 0;

    CAmount subsidy = 10 * COIN;
    subsidy >>= halvings;
    return subsidy;
}

} // namespace europa
