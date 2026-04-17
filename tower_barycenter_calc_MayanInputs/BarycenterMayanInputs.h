#ifndef BCCALCMAYANINPUTS_H
#define BCCALCMAYANINPUTS_H

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
void BCCalcStreamMayanInputs(
    hls::stream<fET>&  InputStreamEt,
    hls::stream<fEta>&  InputStreamEtaIdx,
    hls::stream<bool>& InputStreamBitMask,
    hls::stream<fAlpha>&  OutStreamNum,
    hls::stream<fBeta>&  OutStreamDen
);
};

#endif // BCCALCMAYANINPUTS_H