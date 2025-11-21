#include "PPZCalc.h"
#include "Sort_SSPPZ.h"

void PPZCalcStream(hls::stream<fEta>& InputStreamEtaL1, hls::stream<fEta>& InputStreamEtaL2, hls::stream<fZ>& OutStream){
    #pragma HLS INTERFACE axis port = InputStreamEtaL1
    #pragma HLS INTERFACE axis port = InputStreamEtaL2 
    #pragma HLS INTERFACE axis port = OutStream
     
    fZ UnsortedArr[NMaxClusters] = {};
    fZ SortedArr[NMaxClusters] = {};
    
    PPZCalc_Loop:
    for (int i = 0; i < NMaxClusters; ++i){
        // #pragma HLS UNROLL factor=MaxPipeLine // Will have MaxPipeLine number of loops = MaxPipeLine number of PPZ?
        fEta EtaL1 = InputStreamEtaL1.read();
        fEta EtaL2 = InputStreamEtaL2.read();
        fZ PPZ = PPZ_calc(EtaL1, EtaL2);
        UnsortedArr[i] = PPZ;
    }

    SortPPZArr(UnsortedArr, SortedArr);
    fZ MinDeltaPPZ = DeltaPPZ(SortedArr);
    OutStream.write(MinDeltaPPZ);
    // if (MinDeltaPPZ <= DeltaPPZCut){OutStream.write(true);}
    // else {OutStream.write(true)};

}
    
