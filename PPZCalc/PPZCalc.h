#ifndef PPZCALC_H
#define PPZCALC_H

#include <stdint.h>
#include <hls_stream.h>
#include <hls_math.h>

#define MaxN 5

extern "C" {
void PPZCalcStream(hls::stream<float>& InputStreamEtaL1, hls::stream<float>& InputStreamEtaL2, hls::stream<float>& OutStream);
float PPZ_calc(float eta1, float eta2);
}

#endif // PPZCALC_H
