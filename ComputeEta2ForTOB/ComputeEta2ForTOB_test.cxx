#include <iostream>
#include <cstdint>
#include <cstdlib>
#include "ComputeEta2ForTOB.h"
#include "TypeDefs.h"

int main(){

hls::stream<fAlpha> InputStreamAlpha;
hls::stream<fBeta> InputStreamBeta;
hls::stream<bool> InputStreamBitMask;
hls::stream<fEtaPPZ> OutputStreamEta2_Test;

// Bitmask should be a vector of len(MaxNTowersPerTOB). Assume each TOB has MaxNTowersPerTOB = 4 and the bitmask tells us which towers are valid. If this is untrue, NEED TO DECODE THIS INFO


std::vector<float> InputStreamAlpha_Vec =     {120.453, 32.232, 532.3, 899.84}; //  < NCells for now, and TestV1
std::vector<float> InputStreamBeta_Vec = {200, 39, 600, 1000}; // Single value
std::vector<bool> InputStreamBitMask_Vec = {1, 1, 1, 0, }; // TestV1, answer should be (120.453 + 32.232 + 532.3)/839 = 684.985/839 = .816430

for (int i = 0; i < MaxNTowersPerTOB; ++i) {
    InputStreamAlpha.write(static_cast<fAlpha>(InputStreamAlpha_Vec[i]));
    InputStreamBeta.write(static_cast<fBeta>(InputStreamBeta_Vec[i]));
    InputStreamBitMask.write(InputStreamBitMask_Vec[i]);
    std::cout << "Index: " << i << ", Input Alpha: " << static_cast<fAlpha>(InputStreamAlpha_Vec[i]) << ", Beta: " << static_cast<fBeta>(InputStreamBeta_Vec[i]) << ", BitMask: " << InputStreamBitMask_Vec[i] << std::endl;
  }

for (int i = 0; i < MaxNTowersPerTOB+1; ++i) {
    std::cout << "Running BCCalcStreamMayanInputs, idx = " << i << std::endl;
    ComputeEta2ForTOB(InputStreamAlpha, InputStreamBeta, InputStreamBitMask, OutputStreamEta2_Test);
}

if (!OutputStreamEta2_Test.empty()) {
    fEtaPPZ OutEta2 = OutputStreamEta2_Test.read();
    std::cout << "Output Eta2 = " << OutEta2 << std::endl;
} else {
    std::cout << "ERROR: Output stream is empty." << std::endl;
    return 1;
}

}


