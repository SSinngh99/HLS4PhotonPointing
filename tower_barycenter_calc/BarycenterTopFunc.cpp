#include "BarycenterCalc.h"

static void reset_top5(Candidate top5[TopK]) {
    #pragma HLS INLINE
    for (int i = 0; i < TopK; i++) {
        #pragma HLS PIPELINE II=1
        top5[i].et     = 0;
        top5[i].etaIdx = 0;
        top5[i].valid  = false;
    }
}

static void insert_top5SS(Candidate x, Candidate top5[TopK]) {
    #pragma HLS INLINE

    if (!x.valid) return;

    // If buffer not yet full, this version assumes invalid entries are at the end.
    // Works cleanly if reset_top5() sets valid=false and et=0.
    int insertPos = TopK;

    // If any invalid slot exists, or x is better than current smallest, insert it.
    if (!top5[TopK - 1].valid || x.et > top5[TopK - 1].et) {
        for (int i = 0; i < TopK; ++i) {
        #pragma HLS PIPELINE II=1
            if (!top5[i].valid || x.et > top5[i].et) {
                insertPos = i;
                break;
            }
        }

        if (insertPos < TopK) {
            for (int j = TopK - 1; j > insertPos; --j) {
            #pragma HLS PIPELINE II=1
                top5[j] = top5[j - 1];
            }
            top5[insertPos] = x;
            top5[insertPos].valid = true;
        }
    }
}

void BCCalcStream(
    hls::stream<fET>&  InputStreamEt,
    hls::stream<int>&  InputStreamEtaIdx,
    hls::stream<bool>& InputStreamBitMask,
    hls::stream<fBC>&  OutStream
) {
    #pragma HLS INTERFACE axis port=InputStreamEt
    #pragma HLS INTERFACE axis port=InputStreamEtaIdx
    #pragma HLS INTERFACE axis port=InputStreamBitMask
    #pragma HLS INTERFACE axis port=OutStream
    #pragma HLS PIPELINE II=1

    static State state = COLLECT;
    static Candidate top5[TopK];
    static bool initialized = false;
    static int countCells = 0;

    #pragma HLS ARRAY_PARTITION variable=top5 complete

    if (!initialized) {
        reset_top5(top5);
        initialized = true;
    }

    switch (state) {
        case COLLECT: {
            if (!InputStreamEt.empty() &&
                !InputStreamEtaIdx.empty() &&
                !InputStreamBitMask.empty()) {

                Candidate x;
                x.et     = InputStreamEt.read();
                x.etaIdx = InputStreamEtaIdx.read();
                x.valid  = InputStreamBitMask.read();

                if (x.valid) {
                    insert_top5SS(x, top5);
                }

                countCells++;

                #ifndef __SYNTHESIS__
                std::cout << "Read cell " << countCells << "/" << NCells << std::endl;
                for (int i = 0; i < TopK; ++i) {
                    std::cout << "  top5[" << i << "] ET=" << top5[i].et
                              << " eta=" << top5[i].etaIdx
                              << " valid=" << top5[i].valid << std::endl;
                }
                #endif

                if (countCells == NCells) {
                    state = EMIT;
                }
            }
            break;
        }

        case EMIT: {
            fBC BCCalc = BCCalcFunc_top5(top5);

            #ifndef __SYNTHESIS__
            std::cout << "EMIT state reached" << std::endl;
            for (int i = 0; i < TopK; ++i) {
                std::cout << "  final top5[" << i << "] ET=" << top5[i].et
                          << " eta=" << top5[i].etaIdx
                          << " valid=" << top5[i].valid << std::endl;
            }
            std::cout << "BCCalc = " << BCCalc << std::endl;
            #endif

            OutStream.write(BCCalc);

            // Reset for next event
            reset_top5(top5);
            countCells = 0;
            state = COLLECT;
            break;
        }

        default: {
            state = COLLECT;
            countCells = 0;
            reset_top5(top5);
            break;
        }
    }
}