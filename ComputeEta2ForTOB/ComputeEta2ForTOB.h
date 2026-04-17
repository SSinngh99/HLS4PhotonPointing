#ifndef COMPUTEETA2FORTOB_H
#define COMPUTEETA2FORTOB_H

#include <stdint.h>
#include <hls_stream.h>
#include <hls_math.h>
#include <ap_fixed.h>
#include <map>
#include "TypeDefs.h"

enum State {
    COLLECT = 0,
    EMIT    = 1
};

extern "C" {
void ComputeEta2ForTOB(
    hls::stream<fAlpha>&  InputStreamAlpha,
    hls::stream<fBeta>&  InputStreamBeta,
    hls::stream<bool>& InputStreamBitMask,
    hls::stream<fEtaPPZ>&  OutStreamEta2
);
};

#endif // COMPUTEETA2FORTOB_H