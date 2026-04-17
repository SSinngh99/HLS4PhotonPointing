#include <iostream>
#include <cstdint>
#include <cstdlib>
#include "ComputeEta1ForTOB.h"
#include "TypeDefs.h"


int GetL1EtaIdx(float eta){
  int max_eta_ind;
  if (eta == -999){return OverflowL1IdxEta;}
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

hls::stream<fET> InputStreamEt;
hls::stream<fEtaIdxL1> InputStreamEtaIdxL1;
hls::stream<bool> InputStreamBitMask;
hls::stream<fEtaIdxL1>  OutStreamEtaIdxL1_Test;


// Bitmask should be a vector of len(32). Assume Pawel is giving us 32 cells in L1 regardless of what the size of a tower is - if this is untrue, NEED ANOTHER CONTROL FLAG

std::vector<float> InputStreamEt_Vec =     {-3, 10, 14, 3,}; //  < MaxNTowersPerTOB for now, and TestV1
std::vector<float> InputStreamEta_Vec = {1.0,      1.003125, 1.009375, 1.0025};
std::vector<bool> InputStreamBitMask_Vec = {0, 1, 1, 1}; // TestV1, 14 and 1.009375 for raw eta (note 20 is flagged as bad)



// Fill up ET to 16 to not consider stuff beyond the things written above
for (int i = InputStreamEt_Vec.size(); i < MaxNTowersPerTOB; ++i){
  InputStreamBitMask_Vec.push_back(0);
  InputStreamEta_Vec.push_back(-999);
}

for (int i = 0; i < MaxNTowersPerTOB; ++i) {
    InputStreamEt.write(static_cast<fET>(InputStreamEt_Vec[i]));
    InputStreamEtaIdxL1.write(static_cast<fEtaIdxL1>(GetL1EtaIdx(InputStreamEta_Vec[i])));
    InputStreamBitMask.write(InputStreamBitMask_Vec[i]);
    std::cout << "Index: " << i << ", Input ET: " << InputStreamEt_Vec[i] << ", Eta: " << InputStreamEta_Vec[i] << ", EtaIdx: " << GetL1EtaIdx(InputStreamEta_Vec[i]) << ", BitMask: " << InputStreamBitMask_Vec[i] << std::endl;
  }

for (int i = 0; i < MaxNTowersPerTOB+1; ++i) {
    std::cout << "Running FindMaxCellL1, idx = " << i << std::endl;
    ComputeMaxCellL1_TOB(InputStreamEt, InputStreamEtaIdxL1, InputStreamBitMask, OutStreamEtaIdxL1_Test);
}

if (!OutStreamEtaIdxL1_Test.empty()) {
    fEtaIdxL1 OutEtaIdx = OutStreamEtaIdxL1_Test.read();
    std::cout << "Output EtaIdx = " << OutEtaIdx << std::endl;
} else {
    std::cout << "ERROR: Output stream is empty." << std::endl;
    return 1;
}
}


