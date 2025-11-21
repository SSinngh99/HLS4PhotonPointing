#include "PPZCalc.h"
#include "Sort_SSPPZ.h"

void PPZCalcStream(hls::stream<int>& InputStreamEtaL1, hls::stream<int>& InputStreamEtaL2, hls::stream<fZ>& OutStream){
    #pragma HLS INTERFACE axis port = InputStreamEtaL1
    #pragma HLS INTERFACE axis port = InputStreamEtaL2 
    #pragma HLS INTERFACE axis port = OutStream
     
    fZ UnsortedArr[NMaxClusters] = {};
    fZ SortedArr[NMaxClusters] = {};
    
    PPZCalc_Loop:
    for (int i = 0; i < NMaxClusters; ++i){
        int Eta1Idx = InputStreamEtaL1.read();
        int Eta2Idx = InputStreamEtaL2.read();
        // #pragma HLS UNROLL factor=MaxPipeLine // Will have MaxPipeLine number of loops = MaxPipeLine number of PPZ?
        
        fZ PPZ = PPZ_calc(Eta1Idx, Eta2Idx);
        UnsortedArr[i] = PPZ;
    }

    SortPPZArr(UnsortedArr, SortedArr);
    fZ MinDeltaPPZ = DeltaPPZ(SortedArr);
    OutStream.write(MinDeltaPPZ);
    // if (MinDeltaPPZ <= DeltaPPZCut){OutStream.write(true);}
    // else {OutStream.write(true)};

}
    
