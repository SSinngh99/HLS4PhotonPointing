#ifndef BCCALC_H
#define BCCALC_H

#include <stdint.h>
#include <hls_stream.h>
#include <hls_math.h>
#include <ap_fixed.h>
#include <map>

# define TopK 5 // Top N cells to use for eta barycenter
#define EtaOverFlow -7
#define EtaIdxOverFlow -9999
#define NMaxClusters 15
#define fETWord 42
#define DecimalPrecision 8
#define fBCWord DecimalPrecision + 4
#define NCells 64





typedef  ap_fixed<fETWord, fETWord - DecimalPrecision> fET; // float for ET, can be changed on the fly.
typedef  ap_fixed<fBCWord, fBCWord - DecimalPrecision> fBC; // float for BC eta calc, 29 bits total, 14+1 for integer (+/-16384), 8 bit decimal

// Candidate packet
struct Candidate {
    fET et;        // ET
    int etaIdx;       // payload
    bool   valid;     // whether this input is valid
    // bool   process;   // when true, emit current top-5 buffer
};

enum State {
    COLLECT = 0,
    EMIT    = 1
};



extern "C" {
void BCCalcStream(hls::stream<fET>& InputStreamEt, hls::stream<int>& InputStreamEtaIdx, hls::stream<bool>& InputStreamBitMask, hls::stream<fBC>& OutStream);
void CheckTopK(const fET ET, const int EtaIdx, fET TopKET_Buffer[TopK], int TopKEta_Buffer[TopK]);
}
fBC BCCalcFunc(fET TopKET_Buffer[TopK], int TopKEta_Buffer[TopK]);
fBC BCCalcFunc_top5(Candidate top5[5]);



#endif // BCCALC_H
