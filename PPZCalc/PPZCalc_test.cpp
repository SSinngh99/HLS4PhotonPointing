#include <iostream>
#include <cstdint>
#include <cstdlib>
#include "PPZCalc.h"





int main(){

// std::cout << 1;

hls::stream<int> InputStreamEtaL1;
hls::stream<int> InputStreamEtaL2;
hls::stream<fZ> OutputStream_Test;

std::vector<float> InputVectorEtaL1 = {-2.099481, -1.181972, -999, 0.221235, -2.082226};
std::vector<float> InputVectorEtaL2 = {-2.098641, -1.191262, 0.112489, 0.202920, -2.060124};

std::vector<int> InputEtaL1Idx = {0, 1, -9999, 2, 3};
std::vector<int> InputEtaL2Idx = {4, 5, 6, 7, 8};


for (int i = InputVectorEtaL1.size(); i < NMaxClusters; ++i){
  InputEtaL1Idx.push_back(-9999);
  InputEtaL2Idx.push_back(-9999);
  std::cout << i << std::endl;
}



// std::vector<float> InputVectorEtaL1 = {-1.181972, -999, 0.221235, -2.082226};
// std::vector<float> InputVectorEtaL2 = {-1.191262, 0.112489, 0.202920, -2.060124};

// // std::vector<float> InputVectorEtaL1 = {-2.0995, -1.1820, -999, 0.2212, -2.0822};
// // std::vector<float> InputVectorEtaL2 = {-2.0986, -1.1820, 0.1125, 0.2029, -2.0601};

// //answers: -112.0424, 516.07116, -nan, 375.0011. -1685.579224


for (int i = 0; i < NMaxClusters; ++i) {
    std::cout << "eta1[" << i << "] = " << InputEtaL1Idx[i] << std::endl;
    InputStreamEtaL1.write(InputEtaL1Idx[i]);
    InputStreamEtaL2.write(InputEtaL2Idx[i]);
  }


PPZCalcStream(InputStreamEtaL1, InputStreamEtaL2, OutputStream_Test);
fZ OutMinDeltaPPZ = OutputStream_Test.read();
}


