// Europa subsidy compatibility for Bitcoin Core 31.1.
// UNTESTED / DO NOT RELEASE.
#ifndef EUROPA_SUBSIDY_H
#define EUROPA_SUBSIDY_H

#include <consensus/amount.h>

namespace Consensus { struct Params; }

namespace europa {

CAmount GetBlockSubsidy(int height, const Consensus::Params& params);

} // namespace europa

#endif // EUROPA_SUBSIDY_H
