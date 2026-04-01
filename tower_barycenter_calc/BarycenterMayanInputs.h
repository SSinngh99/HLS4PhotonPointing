#ifndef BCCALCMAYANINPUTS_H
#define BCCALCMAYANINPUTS_H

#include <stdint.h>
#include <hls_stream.h>
#include <hls_math.h>
#include <ap_fixed.h>
#include <map>

#define EtaOverFlow -7
#define EtaIdxOverFlow -9999
#define fETWord 20
#define DecimalPrecision 11
#define fBCWord DecimalPrecision + 3
#define NCells 16
#define fEtaWord DecimalPrecision + 3 //

typedef  ap_fixed<fETWord, fETWord - DecimalPrecision> fET; // float for ET, can be changed on the fly.
typedef  ap_fixed<fBCWord, fBCWord - DecimalPrecision> fBC; // float for BC eta calc, 29 bits total, 14+1 for integer (+/-16384), 8 bit decimal
typedef  ap_fixed<fEtaWord, fEtaWord - DecimalPrecision> fEta; // float for sinh(x), 18 bits in total, 3 + 1 signed bit for the integer part (-8, 8) and 8 bits for the decimal (1/2^8)

enum State {
    COLLECT = 0,
    EMIT    = 1
};

extern "C" {
void BCCalcStreamMayanInputs(
    hls::stream<fET>&  InputStreamEt,
    hls::stream<fBC>&  InputStreamEta,
    hls::stream<bool>& InputStreamBitMask,
    hls::stream<fBC>&  OutStream
);
};

#endif // BCCALCMAYANINPUTS_H