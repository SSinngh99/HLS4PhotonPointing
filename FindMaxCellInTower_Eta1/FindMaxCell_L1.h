#ifndef BCCALCMAYANINPUTS_H
#define BCCALCMAYANINPUTS_H

#include <stdint.h>
#include <hls_stream.h>
#include <hls_math.h>
#include "TypeDefs.h"

enum State {
    COLLECT = 0,
    EMIT    = 1
};

extern "C" {
void FindMaxCellL1(
    hls::stream<fET>&  InputStreamEt,
    hls::stream<fEtaIdxL1>&  InputStreamEtaIdxL1,
    hls::stream<bool>& InputStreamBitMask,
    hls::stream<fEtaIdxL1>&  OutStreamEtaIdxL1,
    hls::stream<fET>&  OutStreamMaxET
);
};

#endif // BCCALCMAYANINPUTS_H