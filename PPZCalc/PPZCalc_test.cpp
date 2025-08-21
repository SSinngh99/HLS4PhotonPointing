#include <iostream>
#include <cstdint>
#include <cstdlib>
#include "PPZCalc.h"


int main(){

hls::stream<float> InputStreamEtaL1;
hls::stream<float> InputStreamEtaL2;
hls::stream<float> OutputStream_Test;

//std::vector<float> InputVectorEtaL1 = {-2.099481, -1.181972, -999, 0.221235, -2.082226};
//std::vector<float> InputVectorEtaL2 = {-2.098641, -1.191262, 0.112489, 0.202920, -2.060124};

std::vector<float> InputVectorEtaL1 = {-2.0995, -1.1820, -999, 0.2212, -2.0822};
std::vector<float> InputVectorEtaL2 = {-2.0986, -1.1820, 0.1125, 0.2029, -2.0601};
//answers: -112.0424, 516.07116, -nan, 375.0011. -1685.579224

for (int i=0; i < MaxN; i++){
    InputStreamEtaL1.write(InputVectorEtaL1[i]);
    InputStreamEtaL2.write(InputVectorEtaL2[i]);
}
PPZCalcStream(InputStreamEtaL1, InputStreamEtaL2, OutputStream_Test);

for (int i = 0; i < MaxN; i++){
std::cout << std::setprecision(8) << "input # = " << i << ", Eta 1: " << InputVectorEtaL1[i] << ", Eta 2: " << InputVectorEtaL2[i] << ", Output stream = " << OutputStream_Test.read() << std::endl;
}

}
