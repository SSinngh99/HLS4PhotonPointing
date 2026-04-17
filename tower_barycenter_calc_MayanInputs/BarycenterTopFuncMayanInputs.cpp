#include "BarycenterMayanInputs.h"

const fAlpha EtaDiffMapL2[16] = {
    -0.025, 0, 0.025, 0.05,
    -0.025, 0, 0.025, 0.05,
    -0.025, 0, 0.025, 0.05,
    -0.025, 0, 0.025, 0.05,
};

SingleBitInt init = 0;
fET ETVec[NCells];
static State state;
FiveBitInt countCells;
FiveBitInt ETIdx;
fAlpha Alpha;
fBeta Beta;
fEta eta;



void BCCalcStreamMayanInputs(
    hls::stream<fET>&  InputStreamEt,
    hls::stream<fEta>&  InputStreamEta,
    hls::stream<bool>& InputStreamBitMask,
    hls::stream<fAlpha>&  OutStreamNum,
    hls::stream<fBeta>&  OutStreamDen
){
    #pragma HLS INTERFACE axis port=InputStreamEt
    #pragma HLS INTERFACE axis port=InputStreamEta
    #pragma HLS INTERFACE axis port=InputStreamBitMask
    #pragma HLS INTERFACE axis port=OutStreamNum
    #pragma HLS INTERFACE axis port=OutStreamDen
    #pragma HLS PIPELINE II=1
    // #pragma HLS latency min=12 // We will be limited by the max cell in L1 stuff so we can add a min latency if needed
    // #pragma HLS bind_storage variable=EtaDiffMapL2 type=ram_2p impl=URAM
    

    if (init == 0){
        init = ~init;
        state = COLLECT;
        countCells = 0;
        Alpha = 0;
        Beta = 0;
        ETIdx = 0;
        eta = 0;
    }
    switch (state) {
        case COLLECT: {
            ReadLogic:
            fET ET = InputStreamEt.read();
            fEta LocalEta = InputStreamEta.read();
            bool BitMask = InputStreamBitMask.read();

            MathLogic:
            ETVec[countCells] = ET;

            Alpha += ETVec[ETIdx] * BitMask * EtaDiffMapL2[countCells];
            Beta += ETVec[ETIdx] * BitMask;

            // if (BitMask){
            //     Alpha += ETVec[ETIdx] * EtaDiffMapL2[countCells];
            //     Beta += ETVec[ETIdx];
            //     ETIdx += 1;
            // }

            eta += LocalEta * !countCells; // Assume valid eta is first in a set of Ncells stream for a given tower

            #ifndef __SYNTHESIS__
            std:: cout << "countCells = " << countCells << ", Beta = " << Beta << ", ETIdx = " << ETIdx << ", ET = " << ETVec[ETIdx] << ", global eta = " << eta << std::endl;
            std::cout << "Alpha += " << ETVec[ETIdx] << " * " << BitMask << " * " << EtaDiffMapL2[countCells] << " = " << Alpha << std::endl << std::endl;
            std::cout << "LocalEta * !countCells = " << LocalEta << " * " << !countCells << " = " << LocalEta * !countCells << std::endl;
            #endif

            ETIdx += 1 * BitMask;
            countCells += 1;

            ControlLogic:
            state = (countCells == NCells) ? EMIT : state;
            break;
            
        }

        case EMIT: {
            // Alpha += eta * Beta;
            fAlpha Alpha_out = Alpha + eta * Beta;
            OutStreamNum.write(Alpha_out);
            OutStreamDen.write(Beta);

            #ifndef __SYNTHESIS__
            std::cout << "EMIT:" << std::endl;
            std:: cout << "countCells = " << countCells << ", Alpha = " << Alpha << ", Beta = " << Beta << ", eta = " << eta << std::endl;
            #endif

            countCells = 0;
            Alpha = 0;
            Beta = 0;
            ETIdx = 0;
            eta = 0;
            state = COLLECT;

            
        }
    }
}