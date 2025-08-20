#include "hls_stream.h"
#include "Kernel1Square.h"
#include "cmath.h"

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
    
    float EtaL1, EtaL2, PPZ;
    do {
    EtaL1 = InputStreamEtaL1.read();
    EtaL2 = InputStreamEtaL2.read();
    PPZ = PPZ_calc(EtaL1, EtaL2);
    OutStream.write(PPZ);
    }while ((!InputStreamEtaL1.empty()) && (!InputStreamEtaL2.empty()));
}


float PPZ_calc(float eta1, float eta2){

    aeta1 = std::abs(eta1);
    aeta2 = std::abs(eta2);
    
    // For eta layer 1
    if (aeta1 < 0.8){
        float Z1 =  (1558.859292 - 4.990838 * aeta1 - 21.144279 * aeta1 * aeta1) * std::sinh(eta1);
    } else if (aeta1 < 1.5){
        float Z1 = (1522.775373 + 27.970192 * aeta1 - 21.104108 * aeta1 * aeta1) * std::sinh(eta1);
    }
    else{
        float Z1 = 3790.671754;
    }

    // For eta layer 2
    if (aeta2 < 1.425) {  // Barrel
        float Z2 = (1698.990944 - 49.431767 * aeta2 - 24.504976 * aeta2 * aeta2) * std::sinh(eta2);
    } else if (aeta2 < 1.5) {  // EME2 in tool
        float Z2 = (8027.574119 - 2717.653528 * aeta2) * std::sinh(eta2);
    } else {
        // endcap so Z
        float Z2 = (3473.473909 + 453.941515 * aeta2 - 119.101945 * aeta2 * aeta2);
    }
    if (eta2 < 0.) {
        // negative endcap
        Z2 = -Z2;
    }

    return (((Z1 * Z2) / (Z2 * std::sinh(eta1) - Z1 * (std::sinh(eta2))))*(std::sinh(eta1) -  std::sinh(eta2)));
}
