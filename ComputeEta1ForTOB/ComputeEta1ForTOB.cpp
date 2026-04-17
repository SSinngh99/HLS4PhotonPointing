#include "ComputeEta1ForTOB.h"

SingleBitInt init = 0;
fET MaxET;
static State state;
ThreeBitInt countCells;
fEtaIdxL1 OutputEta;

void ComputeMaxCellL1_TOB(
    hls::stream<fET>&  InputStreamEt,
    hls::stream<fEtaIdxL1>&  InputStreamEtaIdxL1,
    hls::stream<bool>& InputStreamBitMask,
    hls::stream<fEtaIdxL1>&  OutStreamEtaIdxL1
){
    #pragma HLS INTERFACE axis port=InputStreamEt
    #pragma HLS INTERFACE axis port=InputStreamEtaIdxL1
    #pragma HLS INTERFACE axis port=InputStreamBitMask
    #pragma HLS INTERFACE axis port=OutStreamEtaIdxL1
    #pragma HLS PIPELINE II=1
    

    if (~init){
        init = ~init;
        state = COLLECT;
        countCells = 0;
        MaxET = 0;
        OutputEta = OverflowL1IdxEta;
    }
    switch (state) {
        case COLLECT: {
            ReadLogic:
            fET ET = InputStreamEt.read();
            fEtaIdxL1 LocalEta = InputStreamEtaIdxL1.read();
            bool BitMask = InputStreamBitMask.read();

            MathLogic:
            if (BitMask){
                if (ET > MaxET){
                    MaxET = ET;
                    OutputEta = LocalEta;
                    }
            }

            #ifndef __SYNTHESIS__
            std::cout << "countCells = " << countCells << ", ET = " << ET << ", BitMask = " << BitMask << ", MaxET = " << MaxET << std::endl;
            std::cout << "LocalEta = " << LocalEta << ", OutputEta = " << OutputEta << std::endl;
            #endif

            countCells += 1;

            ControlLogic:
            state = (countCells == MaxNTowersPerTOB) ? EMIT : state;
            break;
            
        }

        case EMIT: {
            OutStreamEtaIdxL1.write(OutputEta);

            #ifndef __SYNTHESIS__
            std::cout << "EMIT:" << std::endl;
            std:: cout << "countCells = " << countCells << ", OutputEta = " << OutputEta << ", MaxET = " << MaxET << std::endl;
            #endif

            countCells = 0;
            MaxET = 0;
            OutputEta = OverflowL1IdxEta;
            state = COLLECT;

            
        }
    }
}