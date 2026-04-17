#include "ComputeEta2ForTOB.h"
#include "TypeDefs.h"

SingleBitInt init = 0;
fET ETVec[NCells];
static State state;
ThreeBitInt countTowers;
fAlpha AlphaSum;
fBeta BetaSum;




void ComputeEta2ForTOB(
    hls::stream<fAlpha>&  InputStreamAlpha,
    hls::stream<fBeta>&  InputStreamBeta,
    hls::stream<bool>& InputStreamBitMask,
    hls::stream<fEtaPPZ>&  OutStreamEta2
){
    #pragma HLS INTERFACE axis port=InputStreamAlpha
    #pragma HLS INTERFACE axis port=InputStreamBeta
    #pragma HLS INTERFACE axis port=InputStreamBitMask
    #pragma HLS INTERFACE axis port=OutStreamEta2
    #pragma HLS PIPELINE II=1
    

    if (init == 0){
        init = ~init;
        state = COLLECT;
        countTowers = 0;
        AlphaSum = 0;
        BetaSum = 0;
    }
    switch (state) {
        case COLLECT: {
            ReadLogic:
            fAlpha AlphaTower = InputStreamAlpha.read();
            fBeta BetaTower = InputStreamBeta.read();
            bool BitMask = InputStreamBitMask.read();

            MathLogic:

            if (BitMask){
                AlphaSum += AlphaTower;
                BetaSum += BetaTower;
            }


            #ifndef __SYNTHESIS__
            std::cout << "countTowers = " << countTowers << ", Alpha = " << AlphaTower << ", Beta = " << BetaTower << ", BitMask = " << BitMask << std::endl;
            std::cout << "AlphaSum = " << AlphaSum << ", BetaSum = " << BetaSum << std::endl;
            #endif

            countTowers += 1;

            ControlLogic:
            state = (countTowers == MaxNTowersPerTOB) ? EMIT : state;
            break;
            
        }

        case EMIT: {
            // Alpha += eta * Beta;
            fEtaPPZ Eta2Out = AlphaSum/BetaSum;
            OutStreamEta2.write(Eta2Out);

            #ifndef __SYNTHESIS__
            std::cout << "EMIT:" << std::endl;
            std:: cout << "countTowers = " << countTowers << ", AlphaSum = " << AlphaSum << ", BetaSum = " << BetaSum << ", eta2 = " << Eta2Out << std::endl;
            #endif

            countTowers = 0;
            AlphaSum = 0;
            BetaSum = 0;
            state = COLLECT;

            
        }
    }
}