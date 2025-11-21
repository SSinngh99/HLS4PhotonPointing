#include <iostream>
#include <cstdint>
#include <cstdlib>
#include "PPZCalc.h"





int main(){

// std::cout << 1;

hls::stream<fEta> InputStreamEtaL1;
hls::stream<fEta> InputStreamEtaL2;
hls::stream<fZ> OutputStream_Test;

std::vector<float> InputVectorEtaL1 = {-2.099481, -1.181972, -999, 0.221235, -2.082226};
std::vector<float> InputVectorEtaL2 = {-2.098641, -1.191262, 0.112489, 0.202920, -2.060124};

for (int i = InputVectorEtaL1.size(); i < NMaxClusters; ++i){
  InputVectorEtaL1.push_back(-999);
  InputVectorEtaL2.push_back(-999);
  std::cout << i << std::endl;
}



// std::vector<float> InputVectorEtaL1 = {-1.181972, -999, 0.221235, -2.082226};
// std::vector<float> InputVectorEtaL2 = {-1.191262, 0.112489, 0.202920, -2.060124};

// // std::vector<float> InputVectorEtaL1 = {-2.0995, -1.1820, -999, 0.2212, -2.0822};
// // std::vector<float> InputVectorEtaL2 = {-2.0986, -1.1820, 0.1125, 0.2029, -2.0601};

// //answers: -112.0424, 516.07116, -nan, 375.0011. -1685.579224


for (int i = 0; i < NMaxClusters; ++i) {
    std::cout << "eta1[" << i << "] = " << InputVectorEtaL1[i] << std::endl;
    fEta e1 = (fEta)InputVectorEtaL1[i];
    fEta e2 = (fEta)InputVectorEtaL2[i];
    InputStreamEtaL1.write(e1);
    InputStreamEtaL2.write(e2);
  }


PPZCalcStream(InputStreamEtaL1, InputStreamEtaL2, OutputStream_Test);
fZ OutMinDeltaPPZ = OutputStream_Test.read();
}


