#include "hls_stream.h"
#include "hls_math.h"
#include "PPZCalc.h"



void PPZCalcStream(hls::stream<fEta>& InputStreamEtaL1, hls::stream<fEta>& InputStreamEtaL2, hls::stream<fZ>& OutStream){
    #pragma HLS INTERFACE axis port = InputStreamEtaL1 depth=MaxN
    #pragma HLS INTERFACE axis port = InputStreamEtaL2 depth=MaxN
    #pragma HLS INTERFACE axis port = OutStream depth=MaxN

    #pragma HLS DATAFLOW
    //#pragma HLS PIPELINE II=1

    float EtaL1, EtaL2, PPZ;
    if ((!InputStreamEtaL1.empty()) && (!InputStreamEtaL2.empty())){
        EtaL1 = InputStreamEtaL1.read();
        EtaL2 = InputStreamEtaL2.read();
        PPZ = PPZ_calc(EtaL1, EtaL2);
        OutStream.write(PPZ);
    }
    
}

static fZ z1_eval(fEta aeta, fEta eta, fEta s1) {
  #pragma HLS INLINE region

  // Horner form, reuse s1 precomputed
  fZ z;
  if (aeta < 0.8) {
    // (1558.859292 - 4.990838*a - 21.144279*a^2) * s1
    z = CalcSecondOrder(aeta, C0_L1E08, C1_L1E08, C2_L1E08);
    z *= s1;
  } else if (aeta < 1.5) {
    // (1522.775373 + 27.970192*a - 21.104108*a^2) * s1
    z = CalcSecondOrder(aeta, C0_L1E15, C1_L1E15, C2_L1E15);
    z *= s1;
  } else {
    z = CalcSecondOrder(aeta, C0_L1E25, CZero, CZero);
    if (eta < 0) {z *= -1;}
  }
  return z;
}

static fZ z2_eval(fEta aeta, fEta eta, fEta s2) {
  #pragma HLS INLINE region

  fZ z;
  if (aeta < 1.425) {
    // (1698.990944 - 49.431767*a - 24.504976*a^2) * s2
    float c = -24.504976;
    z = CalcSecondOrder(aeta, C0_L2E1425, C1_L2E1425, C2_L2E1425);
    z *= s2;
  } else if (aeta < 1.5) {
    // (8027.574119 - 2717.653528*a) * s2
    z = CalcSecondOrder(aeta, C0_L2E15, C1_L2E15, CZero);
    z *= s2;
  } else {
    z = CalcSecondOrder(aeta, C0_L2E25, C1_L2E25, C2_L2E25);
    if (eta < 0) z = -z;
  }
  return z;
}


fZ CalcSecondOrder(fEta aeta, fZ c0, fZ c1, fZ c2){
    return ((c2 * aeta) + c1) * aeta + c0
}

fZ PPZ_calc(fEta eta1, fEta eta2){

   // #pragma HLS latency min=120
   //#pragma HLS PIPELINE

    fEta aeta1 = abs(eta1);
    fEta aeta2 = abs(eta2);

    fEta s1 = sinh(aeta1);
    fEta s2 = sinh(aeta2);

    if (eta1 < 0){s1 *= -1}
    if (eta2 < 0){s2 *= -1}

    // #pragma HLS bind_op variable=aeta1 op=hmul
    // #pragma HLS bind_op variable=aeta2 op=hmul

    fZ Z1 = z1_eval(aeta1, eta1, s1);
    fZ Z2 = z2_eval(aeta2, eta2, s2);

    // Single reciprocal instead of division
    fZ num = (Z1 * Z2) * (s1 - s2);
    fZ den = (Z2 * s1) - (Z1 * s2);

    return (num / den);
}
