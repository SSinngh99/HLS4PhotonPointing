#include "BarycenterMayanInputs.h"

const fEta EtaDiffMapL2[16] = {
    -0.025, 0, 0.025, 0.05,
    -0.025, 0, 0.025, 0.05,
    -0.025, 0, 0.025, 0.05,
    -0.025, 0, 0.025, 0.05,
};

int init = 0;
fET ETVec[NCells];
static State state;
int countCells;
int ETIdx;
fET Num;
fET Den;
fBC eta;



void BCCalcStreamMayanInputs(
    hls::stream<fET>&  InputStreamEt,
    hls::stream<fBC>&  InputStreamEta,
    hls::stream<bool>& InputStreamBitMask,
    hls::stream<fBC>&  OutStream
){
    #pragma HLS INTERFACE axis port=InputStreamEt
    #pragma HLS INTERFACE axis port=InputStreamEta
    #pragma HLS INTERFACE axis port=InputStreamBitMask
    #pragma HLS INTERFACE axis port=OutStream
    #pragma HLS PIPELINE II=1
    #pragma HLS bind_storage variable=EtaDiffMapL2 type=ram_2p impl=URAM
    

    if (init == 0){
        init = 1;
        state = COLLECT;
        countCells = 0;
        Num = 0;
        Den = 0;
        ETIdx = 0;
        eta = 0;
    }
    switch (state) {
        case COLLECT: {
            ReadLogic:
            fET ET = InputStreamEt.read();
            fBC LocalEta = InputStreamEta.read();
            bool BitMask = InputStreamBitMask.read();

            MathLogic:
            ETVec[countCells] = ET;

            Num += ETVec[ETIdx] * BitMask * EtaDiffMapL2[countCells];
            Den += ETVec[ETIdx] * BitMask;

            eta += LocalEta * !countCells; // Assume valid eta is first in a set of Ncells stream for a given tower

            #ifndef __SYNTHESIS__
            std:: cout << "countCells = " << countCells << ", Den = " << Den << ", ETIdx = " << ETIdx << ", ET = " << ETVec[ETIdx] << ", global eta = " << eta << std::endl;
            std::cout << "Num += " << ETVec[ETIdx] << " * " << BitMask << " * " << EtaDiffMapL2[countCells] << " = " << Num << std::endl << std::endl;
            std::cout << "LocalEta * !countCells = " << LocalEta << " * " << !countCells << " = " << LocalEta * !countCells << std::endl;
            #endif

            ETIdx += 1 * BitMask;
            countCells += 1;

            ControlLogic:
            if (countCells == NCells){state = EMIT;}
            break;
            
        }

        case EMIT: {
            fBC BCLocal = Num/Den;
            OutStream.write(BCLocal + eta);
            #ifndef __SYNTHESIS__
            std::cout << "EMIT:" << std::endl;
            std:: cout << "countCells = " << countCells << ", Num = " << Num << ", Den = " << Den << ", BCLocal = " << BCLocal << ", eta = " << eta << ", eta + BCLocal = " << BCLocal + eta << std::endl;
            #endif

            countCells = 0;
            Num = 0;
            Den = 0;
            ETIdx = 0;
            eta = 0;
            state = COLLECT;
        }
    }
}