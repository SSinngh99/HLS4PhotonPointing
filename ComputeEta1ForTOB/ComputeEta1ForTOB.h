#ifndef COMPUTEETA1FORTOB_H
#define COMPUTEETA1FORTOB_H

#include <stdint.h>
#include <hls_stream.h>
#include <hls_math.h>
#include "TypeDefs.h"

enum State {
    COLLECT = 0,
    EMIT    = 1
};

extern "C" {
void ComputeMaxCellL1_TOB(
    hls::stream<fET>&  InputStreamEt,
    hls::stream<fEtaIdxL1>&  InputStreamEtaIdxL1,
    hls::stream<bool>& InputStreamBitMask,
    hls::stream<fEtaIdxL1>&  OutStreamEtaIdxL1
);
};

#endif // COMPUTEETA1FORTOB_H