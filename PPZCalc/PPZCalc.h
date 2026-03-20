#ifndef PPZCALC_H
#define PPZCALC_H

#include <stdint.h>
#include <hls_stream.h>
#include <hls_math.h>
#include <ap_fixed.h>
#include <map>


#define EtaOverFlow -7
#define EtaIdxOverFlow -9999
#define PPZOverFlow 5000.0
#define MaxPipeLine 1
#define NMaxClusters 15
#define fZWord 42
#define DecimalPrecision 20 // Remember to change the sinh map when changing this otherwise there is no effect!!
#define fEtaWord DecimalPrecision + 4 //
#define DeltaPPZCut 100
#define TwoPowerNBitsDecimal 2**DecimalPrecision


typedef  ap_fixed<fEtaWord, fEtaWord - DecimalPrecision> fEta; // float for sinh(x), 18 bits in total, 3 + 1 signed bit for the integer part (-8, 8) and 8 bits for the decimal (1/2^8)
typedef  ap_fixed<fZWord, fZWord - DecimalPrecision> fZ; // float for Z1 and Z2, 29 bits total, 14+1 for integer (+/-16384), 8 bit decimal


extern "C" {
void PPZCalcStream(hls::stream<int>& InputStreamEtaL1, hls::stream<int>& InputStreamEtaL2, hls::stream<fZ>& OutStream);
// fZ z1_eval(fEta aeta, fEta eta, fEta s1);
// fZ z2_eval(fEta aeta, fEta eta, fEta s2);
void SortPPZArr(const fZ In[NMaxClusters], fZ Out[NMaxClusters]);
}

fZ PPZ_calc(int eta1Idx, int eta2Idx);
fZ CalcSecondOrder(fEta aeta, fZ c0, fZ c1, fZ c2);


#endif // PPZCALC_H
