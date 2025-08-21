#include "hls_stream.h"
#include "hls_math.h"
#include "PPZCalc.h"

void SquareStream(hls::stream<int>& InputStream, hls::stream<int>& OutStream){
    int inVal, inputSq;
    do {
    inVal = InputStream.read();
    inputSq = inVal * inVal;
    OutStream.write(inputSq);
    }while (!InputStream.empty());
}

void PPZCalcStream(hls::stream<float>& InputStreamEtaL1, hls::stream<float>& InputStreamEtaL2, hls::stream<float>& OutStream){
    #pragma HLS INTERFACE axis port = InputStreamEtaL1 depth=MaxN
    #pragma HLS INTERFACE axis port = InputStreamEtaL2 depth=MaxN
    #pragma HLS INTERFACE axis port = OutStream depth=MaxN

    #pragma HLS DATAFLOW
    //#pragma HLS PIPELINE II=1

    float EtaL1, EtaL2, PPZ;
    do {
    EtaL1 = InputStreamEtaL1.read();
    EtaL2 = InputStreamEtaL2.read();
    PPZ = PPZ_calc(EtaL1, EtaL2);
    OutStream.write(PPZ);
    }while ((!InputStreamEtaL1.empty()) && (!InputStreamEtaL2.empty()));
}

static float z1_eval(float aeta, float eta, float s1) {
  #pragma HLS INLINE region

  // Horner form, reuse s1 precomputed
  float z;
  if (aeta < 0.8f) {
    // (1558.859292 - 4.990838*a - 21.144279*a^2) * s1
    float c = -21.144279f;
    z = (c*aeta + (-4.990838f))*aeta + 1558.859292f;
    z *= s1;
  } else if (aeta < 1.5f) {
    // (1522.775373 + 27.970192*a - 21.104108*a^2) * s1
    float c = -21.104108f;
    z = (c*aeta + 27.970192f)*aeta + 1522.775373f;
    z *= s1;
  } else {
    z = 3790.671754f;
    if (eta < 0.f) z = -z;
  }
  return z;
}

static float z2_eval(float aeta, float eta, float s2) {
  #pragma HLS INLINE region

  float z;
  if (aeta < 1.425f) {
    // (1698.990944 - 49.431767*a - 24.504976*a^2) * s2
    float c = -24.504976f;
    z = (c*aeta + (-49.431767f))*aeta + 1698.990944f;
    z *= s2;
  } else if (aeta < 1.5f) {
    // (8027.574119 - 2717.653528*a) * s2
    z = ( -2717.653528f * aeta + 8027.574119f ) * s2;
  } else {
    z = (3473.473909f + 453.941515f*aeta - 119.101945f*aeta*aeta);
    if (eta < 0.f) z = -z;
  }
  return z;
}

float PPZ_calc(float eta1, float eta2){

   // #pragma HLS latency min=120
   //#pragma HLS PIPELINE

    float s1 = sinh(eta1);
    float s2 = sinh(eta2);

    float aeta1 = std::abs(eta1);
    float aeta2 = std::abs(eta2);
    
    // #pragma HLS bind_op variable=aeta1 op=hmul
    // #pragma HLS bind_op variable=aeta2 op=hmul

    float Z1 = z1_eval(aeta1, eta1, s1);
    float Z2 = z2_eval(aeta2, eta2, s2);

    // Single reciprocal instead of division
    float num = (Z1 * Z2) * (s1 - s2);
    float den = (Z2 * s1) - (Z1 * s2);
    // hls::recip is cheaper than / for many targets; NR can further tune
    float inv = hls::recip(den);

    return (num * inv);
}
