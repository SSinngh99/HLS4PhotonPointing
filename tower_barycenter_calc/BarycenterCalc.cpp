#include "hls_stream.h"
#include "hls_math.h"
#include "BarycenterCalc.h"


void SortArr(const fET In[NCells], bool Out[NCells]){
    bool cmp[NMaxClusters-1];
    #pragma HLS ARRAY_PARTITION variable=In  complete
    #pragma HLS ARRAY_PARTITION variable=Out complete
    #pragma HLS ARRAY_PARTITION variable=cmp complete
    fET TempArr[NCells] = {};
    
    for (int i = 0; i < NCells; ++i) {
        // #pragma HLS UNROLL
        for (int j = 0; j < NCells; ++j) {
            #pragma HLS UNROLL
            if (j < i)      cmp[j]   = (In[i] >= In[j]);
            else if (j > i) cmp[j-1] = (In[i] >  In[j]);
        }

        int r = 0;
        for (int k = 0; k < NCells-1; ++k) {
            #pragma HLS UNROLL
            r += cmp[k];
        }
        TempArr[r] = In[i]; 
    }

    #ifndef __SYNTHESIS__
    for (int i = 0; i < NMaxClusters; ++i){
        std::cout << "Out[" << i << "]: " << Out[i] << std::endl;
    }
    #endif

    // for (int MaxIdx = 0; MaxIdx < ){
    //     Out[]
    // }
}


void CheckTopK(const fET ET, const int EtaIdx, fET TopKET_Buffer[TopK], int TopKEta_Buffer[TopK]){
    // TopK is sorted so check if ET > TopKET_Buffer[-1]
    int InsertPos = TopK;
    if (ET > TopKET_Buffer[TopK - 1]){
        for (int i = 0; i < TopK; ++i){
            #pragma HLS pipeline II=1
            if (ET > TopKET_Buffer[i]){
                InsertPos = i;
                for (int j = TopK - 1; j > InsertPos; --j){
                    #pragma HLS pipeline II=1
                    TopKET_Buffer[j] = TopKET_Buffer[j - 1];
                    TopKEta_Buffer[j] = TopKEta_Buffer[j - 1];
                }
                if (InsertPos < TopK){
                    TopKET_Buffer[InsertPos] = ET;
                    TopKEta_Buffer[InsertPos] = EtaIdx;
                }
                break;
            }
        } 
    }
}

fBC BCCalcFunc(fET TopKET_Buffer[TopK], int TopKEta_Buffer[TopK]){
    fET SumET = 0;
    fET BCNum = 0;
    for (int i = 0; i < TopK; ++i){
        // #pragma HLS pipeline II=1
        #pragma HLS UNROLL
        SumET += TopKET_Buffer[i];
        BCNum += TopKET_Buffer[i] * TopKEta_Buffer[i];
        #ifndef __SYNTHESIS__
        std::cout << "Idx: " << i << ", SumET: " << SumET << ", BCNum: " << BCNum << std::endl;
        #endif
    }

    return BCNum/SumET;
}

fBC BCCalcFunc_top5(Candidate top5[5]){
    fET SumET = 0;
    fET BCNum = 0;
    for (int i = 0; i < TopK; ++i){
        // #pragma HLS pipeline II=1
        #pragma HLS UNROLL
        SumET += top5[i].et;
        BCNum += top5[i].et * top5[i].etaIdx;
        #ifndef __SYNTHESIS__
        std::cout << "Idx: " << i << ", SumET: " << SumET << ", BCNum: " << BCNum << std::endl;
        #endif
    }

    return BCNum/SumET;
}

