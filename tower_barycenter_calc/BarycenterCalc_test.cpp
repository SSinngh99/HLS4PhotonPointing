#include <iostream>
#include <cstdint>
#include <cstdlib>
#include "BarycenterCalc.h"


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

hls::stream<fET> InputStreamEt;
hls::stream<int> InputStreamEtaIdx;
hls::stream<bool> InputStreamBitMask;
hls::stream<fBC> OutputStream_Test;

std::vector<float> InputStreamEt_Vec =     {0.2, 10, 14, 28, 9, 5, 8};
std::vector<float> InputVectorEta = {-2.48828125, -2.46484375, -2.390625, -2.36328125, -2.33984375, -2.3125, -2.23828125}; // idx = (0, 1, 4, 5, 6, 7, 10)
std::vector<bool> InputStreamBitMask_Vec = {1,    0,  1,  0, 0, 0, 1};
std::vector<int> InputStreamEtaIdx_Vec;

// Fill up bitmask to 64 to not consider stuff beyond the things written above
for (int i = InputStreamBitMask_Vec.size(); i < NCells; ++i){
  InputStreamBitMask_Vec.push_back(0);
}

// Convert eta to etaIdx
for (int i = 0; i < InputVectorEta.size(); ++i){
  int EtaIdx2 = GetL2EtaIdx(InputVectorEta.at(i)); // Using L2 mapping
  InputStreamEtaIdx_Vec.push_back(EtaIdx2);
}

for (int i = 0; i < NCells; ++i) {
    InputStreamEt.write(static_cast<fET>(InputStreamEt_Vec[i]));
    InputStreamEtaIdx.write(InputStreamEtaIdx_Vec[i]);
    InputStreamBitMask.write(InputStreamBitMask_Vec[i]);
    std::cout << "Index: " << i << ", Input ET: " << InputStreamEt_Vec[i] << ", EtaIdx: " << InputStreamEtaIdx_Vec[i] << ", BitMask: " << InputStreamBitMask_Vec[i] << std::endl;
  }

for (int i = 0; i < NCells + 1; ++i) {
    BCCalcStream(InputStreamEt, InputStreamEtaIdx, InputStreamBitMask, OutputStream_Test);
}

if (!OutputStream_Test.empty()) {
    fBC OutMinDeltaPPZ = OutputStream_Test.read();
    std::cout << "Output BC = " << OutMinDeltaPPZ << std::endl;
} else {
    std::cout << "ERROR: Output stream is empty." << std::endl;
    return 1;
}

// BCCalcStream(InputStreamEt, InputStreamEtaIdx, InputStreamBitMask, OutputStream_Test);
// fBC OutMinDeltaPPZ = OutputStream_Test.read();




// std::vector<int> InputEtaL1Idx, InputEtaL2Idx;

// for (int i = 0; i < InputVectorEtaL1.size(); ++i){
//   int Eta1Idx = GetL1EtaIdx(InputVectorEtaL1.at(i));
//   int Eta2Idx = GetL2EtaIdx(InputVectorEtaL2.at(i));
//   InputEtaL1Idx.push_back(Eta1Idx);
//   InputEtaL2Idx.push_back(Eta2Idx);
//   std::cout << "Eta1 = " << InputVectorEtaL1.at(i) << "-->" << Eta1Idx << std::endl;
//   std::cout << "Eta2 = " << InputVectorEtaL2.at(i) << "-->" << Eta2Idx << "\n" << std::endl;
// }



// for (int i = InputVectorEtaL1.size(); i < NMaxClusters; ++i){
//   InputEtaL1Idx.push_back(-9999);
//   InputEtaL2Idx.push_back(-9999);
//   std::cout << i << std::endl;
// }



// std::vector<float> InputVectorEtaL1 = {-1.181972, -999, 0.221235, -2.082226};
// std::vector<float> InputVectorEtaL2 = {-1.191262, 0.112489, 0.202920, -2.060124};

// // std::vector<float> InputVectorEtaL1 = {-2.0995, -1.1820, -999, 0.2212, -2.0822};
// // std::vector<float> InputVectorEtaL2 = {-2.0986, -1.1820, 0.1125, 0.2029, -2.0601};

// //answers: -112.0424, 516.07116, -nan, 375.0011. -1685.579224


}


