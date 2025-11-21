#include <ostream>
#include <stdint.h>
#include <ap_int.h>
#include <hls_math.h>
#include "Sort_SSPPZ.h"
#include "PPZCalc.h"

static inline fZ abs_fixed(fZ x) {
  return (x >= (fZ)0) ? x : (fZ)(-x);
}

void SortPPZArr(const fZ In[NMaxClusters], fZ Out[NMaxClusters]){
    bool cmp[NMaxClusters-1];
    #pragma HLS ARRAY_PARTITION variable=In  complete
    #pragma HLS ARRAY_PARTITION variable=Out complete
    #pragma HLS ARRAY_PARTITION variable=cmp complete
    
    
    for (int i = 0; i < NMaxClusters; ++i) {
        // #pragma HLS UNROLL
        for (int j = 0; j < NMaxClusters; ++j) {
            #pragma HLS UNROLL
            if (j < i)      cmp[j]   = (In[i] >= In[j]);
            else if (j > i) cmp[j-1] = (In[i] >  In[j]);
        }

        int r = 0;
        for (int k = 0; k < NMaxClusters-1; ++k) {
            #pragma HLS UNROLL
            r += cmp[k];
        }
        Out[r] = In[i]; 
    }

    #ifndef __SYNTHESIS__
    for (int i = 0; i < NMaxClusters; ++i){
        std::cout << "Out[" << i << "]: " << Out[i] << std::endl;
    }
    #endif
}



fZ DeltaPPZ(const fZ In[NMaxClusters]){
    fZ min_delta = (fZ)PPZOverFlow;
    fZ Diffs[NMaxClusters-1];
    for (int i = 0; i < NMaxClusters - 1; ++i){
        // #pragma HLS UNROLL
        if (In[i] == 0 || In[i + 1] == 0 || In[i] == PPZOverFlow || In[i + 1] == PPZOverFlow){continue;}
        fZ diff = abs_fixed(In[i+1] - In[i]);
        if (diff < min_delta) min_delta = diff;
    }

    return min_delta;
}