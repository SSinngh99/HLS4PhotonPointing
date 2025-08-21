#ifndef PPZCALC_H
#define PPZCALC_H

#include <stdint.h>
#include <hls_stream.h>
#include <hls_math.h>
#include <ap_fixed.h>

typedef  ap_fixed<18, 4> fs; // float for sinh(x), 18 bits in total, 3 + 1 signed bit for the integer part (-8, 8) and 14 bits for the decimal (1/2^14) = 0.00006 for the decimal place
typedef  ap_fixed<29,15> fZ; // float for Z1 and Z2, 29 bits total, 14+1 for integer (+/-16384), 14 bit decimal

#define MaxN 5

extern "C" {
void PPZCalcStream(hls::stream<float>& InputStreamEtaL1, hls::stream<float>& InputStreamEtaL2, hls::stream<float>& OutStream);
float PPZ_calc(float eta1, float eta2);
}

#endif // PPZCALC_H
