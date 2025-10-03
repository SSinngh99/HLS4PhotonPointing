#include <iostream>
#include <cstdint>
#include <cstdlib>
#include "PPZCalc.h"
#include "Sort.h"





int main(){

// std::cout << 1;

hls::stream<fEta> InputStreamEtaL1;
hls::stream<fEta> InputStreamEtaL2;
hls::stream<fZ> OutputStream_Test;

std::vector<float> InputVectorEtaL1 = {-2.099481, -1.181972, -999, 0.221235, -2.082226};
std::vector<float> InputVectorEtaL2 = {-2.098641, -1.191262, 0.112489, 0.202920, -2.060124};

// std::vector<float> InputVectorEtaL1 = {-1.181972, -999, 0.221235, -2.082226};
// std::vector<float> InputVectorEtaL2 = {-1.191262, 0.112489, 0.202920, -2.060124};

// // std::vector<float> InputVectorEtaL1 = {-2.0995, -1.1820, -999, 0.2212, -2.0822};
// // std::vector<float> InputVectorEtaL2 = {-2.0986, -1.1820, 0.1125, 0.2029, -2.0601};

// //answers: -112.0424, 516.07116, -nan, 375.0011. -1685.579224


for (int i = 0; i < MaxN; ++i) {
    fEta e1 = (fEta)InputVectorEtaL1[i];
    fEta e2 = (fEta)InputVectorEtaL2[i];
    InputStreamEtaL1.write(e1);
    InputStreamEtaL2.write(e2);
    std::cout << "e1 = " << e1 << ", e2 = " << e2 << std::endl;
  }


PPZCalcStream(InputStreamEtaL1, InputStreamEtaL2, OutputStream_Test);

for (int i = 0; i < MaxN; i++){
    fZ out = OutputStream_Test.read();   // DRAIN the output
    std::cout << std::setprecision(8)
              << "i=" << i
              << "  Eta1=" << InputVectorEtaL1[i]
              << "  Eta2=" << InputVectorEtaL2[i]
              << "  PPZ="  << out << "\n";
  }
}


