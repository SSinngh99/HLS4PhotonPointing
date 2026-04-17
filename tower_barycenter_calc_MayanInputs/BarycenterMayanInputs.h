#ifndef BCCALCMAYANINPUTS_H
#define BCCALCMAYANINPUTS_H

#include <stdint.h>
#include <hls_stream.h>
#include <hls_math.h>
#include <ap_fixed.h>
#include <map>

#define NCells 16 // Number of cells --> 16 in L2
#define fETWord 12 // Size of word for single ET in cell
#define fETDecimalBits 4 // Decimal precision for ET in cell
#define DecimalPrecisionBits 16 // Decimal precision we need/want for barycenter calc

#define fETIntBits fETWord - fETDecimalBits

#define fBetaWord fETWord + 4 // Max(fBeta) = 16 * ET_cell --> 4 more integer bits than fETWord, use same decimal precision as fETDecimalBits
#define fBetaIntBits fBetaWord - fETDecimalBits

#define fAlphaIntBits fBetaIntBits + 2
#define fAlphaWord fAlphaIntBits + DecimalPrecisionBits // max(alpha) = 2.5 * beta --> 2 integer bits more than beta, but w/ full precision in decimal bits


typedef  ap_fixed<fETWord, fETIntBits> fET; // float for ET, can be changed on the fly.
typedef  ap_fixed<DecimalPrecisionBits + 2, 2> fEta; // Decimal precision needed for 0.025
typedef  ap_uint<1> SingleBitInt; // Single bit for state
typedef  ap_uint<5> FiveBitInt; // Five bit int for CellIdx and NETIdx --> easier to test since it has a 16th idx --> EMIT signal


typedef  ap_fixed<fAlphaWord, fAlphaIntBits> fAlpha; // Should have 2 more integer bits than fBeta, and DecimalPrecisionBits number of fractional bits --> Total word = fETWord - fETDecimalPrec + 2 + DecimalPrecision
typedef  ap_fixed<fBetaWord, fBetaIntBits> fBeta; // Can change to be something else, preferably the scheme for full tower ET


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