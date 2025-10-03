#ifndef PPZCALC_H
#define PPZCALC_H

#include <stdint.h>
#include <hls_stream.h>
#include <hls_math.h>
#include <ap_fixed.h>
#include <map>

typedef  ap_fixed<12, 4> fEta; // float for sinh(x), 18 bits in total, 3 + 1 signed bit for the integer part (-8, 8) and 8 bits for the decimal (1/2^8)
typedef  ap_fixed<32,24> fZ; // float for Z1 and Z2, 29 bits total, 14+1 for integer (+/-16384), 8 bit decimal

#define MaxN 5
#define EtaOverFlow -7
#define PPZOverFlow 5000.0


extern "C" {
void PPZCalcStream(hls::stream<fEta>& InputStreamEtaL1, hls::stream<fEta>& InputStreamEtaL2, hls::stream<fZ>& OutStream);
// fZ z1_eval(fEta aeta, fEta eta, fEta s1);
// fZ z2_eval(fEta aeta, fEta eta, fEta s2);
}
fZ PPZ_calc(fEta eta1, fEta eta2);
fZ CalcSecondOrder(fEta aeta, fZ c0, fZ c1, fZ c2);


#endif // PPZCALC_H
