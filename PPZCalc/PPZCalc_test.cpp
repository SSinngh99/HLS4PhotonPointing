#include <iostream>
#include <cstdint>
#include <cstdlib>
#include "PPZCalc.h"

int main(){

hls::stream<float> InputStreamEtaL1;
hls::stream<float> InputStreamEtaL2;
hls::stream<float> OutputStream_Test;

std::vector<float> InputVectorEtaL1 = {0.1, 0.1, 1.6, 2.2};
std::vector<float> InputVectorEtaL2 = {0.2, 0.2, 1.5, 2.1};

for (int i=0; i < MaxN; i++){
    InputStreamEtaL1.write(InputVectorEtaL1[i]);
    InputStreamEtaL2.write(InputVectorEtaL2[i]);
}
PPZCalcStream(InputStreamEtaL1, InputStreamEtaL2, OutputStream_Test);

for (int i = 0; i < MaxN; i++){
std::cout << "input # = " << i << ", Eta 1: " << InputVectorEtaL1[i] << ", Eta 2: " << InputVectorEtaL2[i] << ", Output stream = " << OutputStream_Test.read() << std::endl;
}

}