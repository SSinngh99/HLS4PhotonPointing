#include <iostream>
#include <cstdint>
#include <cstdlib>
#include "PPZCalc.h"
#include "PPZCalc_test.h"


int GetL1EtaIdx(float eta){
  int max_eta_ind;
  if (eta == -999){return -999;}
  if(eta < -2.4){
    max_eta_ind = (int)((eta + 2.5 - 0.1/8.) / (0.1/4.));
}else if(eta < -2.0){
    max_eta_ind = (int)((eta + 2.4 - 0.1/32.) / (0.1/16.)) + 4;
}else if(eta < -1.8){
    max_eta_ind = (int)((eta + 2.0 - 0.1/48.) / (0.1/24.)) + 68;
}else if(eta < -1.5){
    max_eta_ind = (int)((eta + 1.8 - 0.1/64.) / (0.1/32.)) + 116;
}else if(eta < -1.4){
    max_eta_ind = (int)((eta + 1.5 - 0.1/8.) / (0.1/4.)) + 212;
}else if(eta < 1.4){
    max_eta_ind = (int)((eta + 1.4 - 0.1/64.) / (0.1/32.)) + 216;
}else if(eta < 1.5){
    max_eta_ind = (int)((eta - 1.4 - 0.1/8.) / (0.1/4.)) + 1112;
}else if(eta < 1.8){
    max_eta_ind = (int)((eta - 1.5 - 0.1/64.) / (0.1/32.)) + 1116;
}else if(eta < 2.0){
    max_eta_ind = (int)((eta - 1.8 - 0.1/48.) / (0.1/24.)) + 1212;
}else if(eta < 2.4){
    max_eta_ind = (int)((eta - 2.0 - 0.1/32.) / (0.1/16.)) + 1260;
}else{
    max_eta_ind = (int)((eta - 2.4 - 0.1/8.) / (0.1/4.)) + 1324;
}

  return max_eta_ind;
}

int GetL2EtaIdx(float eta){
  if (eta == -999){return -999;}
  return int((eta + 2.5 - 0.1/8.) / (0.1/4.));
}


int main(){

// std::cout << 1;

hls::stream<int> InputStreamEtaL1;
hls::stream<int> InputStreamEtaL2;
hls::stream<fZ> OutputStream_Test;

std::vector<float> InputVectorEtaL1 = {-2.099481, -1.181972, -999, 0.221235, -2.082226};
std::vector<float> InputVectorEtaL2 = {-2.098641, -1.191262, 0.112489, 0.202920, -2.060124};

std::vector<int> InputEtaL1Idx, InputEtaL2Idx;

for (int i = 0; i < InputVectorEtaL1.size(); ++i){
  int Eta1Idx = GetL1EtaIdx(InputVectorEtaL1.at(i));
  int Eta2Idx = GetL2EtaIdx(InputVectorEtaL2.at(i));
  InputEtaL1Idx.push_back(Eta1Idx);
  InputEtaL2Idx.push_back(Eta2Idx);
  std::cout << "Eta1 = " << InputVectorEtaL1.at(i) << "-->" << Eta1Idx << std::endl;
  std::cout << "Eta2 = " << InputVectorEtaL2.at(i) << "-->" << Eta2Idx << "\n" << std::endl;
}



for (int i = InputVectorEtaL1.size(); i < NMaxTOBs; ++i){
  InputEtaL1Idx.push_back(-9999);
  InputEtaL2Idx.push_back(-9999);
  std::cout << i << std::endl;
}



// std::vector<float> InputVectorEtaL1 = {-1.181972, -999, 0.221235, -2.082226};
// std::vector<float> InputVectorEtaL2 = {-1.191262, 0.112489, 0.202920, -2.060124};

// // std::vector<float> InputVectorEtaL1 = {-2.0995, -1.1820, -999, 0.2212, -2.0822};
// // std::vector<float> InputVectorEtaL2 = {-2.0986, -1.1820, 0.1125, 0.2029, -2.0601};

// //answers: -112.0424, 516.07116, -nan, 375.0011. -1685.579224


for (int i = 0; i < NMaxTOBs; ++i) {
    std::cout << "eta1Idx[" << i << "] = " << InputEtaL1Idx[i] << std::endl;
    InputStreamEtaL1.write(InputEtaL1Idx[i]);
    InputStreamEtaL2.write(InputEtaL2Idx[i]);
  }


PPZCalcStream(InputStreamEtaL1, InputStreamEtaL2, OutputStream_Test);
fZ OutMinDeltaPPZ = OutputStream_Test.read();
}


