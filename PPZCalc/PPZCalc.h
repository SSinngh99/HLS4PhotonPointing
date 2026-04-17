#ifndef PPZCALC_H
#define PPZCALC_H

#include <stdint.h>
#include <hls_stream.h>
#include <hls_math.h>
#include <ap_fixed.h>
#include <map>
#include "TypeDefs.h"

extern "C" {
void PPZCalcStream(hls::stream<int>& InputStreamEtaL1, hls::stream<int>& InputStreamEtaL2, hls::stream<fZ>& OutStream);
// fZ z1_eval(fEtaPPZ aeta, fEtaPPZ eta, fEtaPPZ s1);
// fZ z2_eval(fEtaPPZ aeta, fEtaPPZ eta, fEtaPPZ s2);
void SortPPZArr(const fZ In[NMaxTOBs], fZ Out[NMaxTOBs]);
}

fZ PPZ_calc(int eta1Idx, int eta2Idx);
fZ CalcSecondOrder(fEtaPPZ aeta, fZ c0, fZ c1, fZ c2);


#endif // PPZCALC_H
