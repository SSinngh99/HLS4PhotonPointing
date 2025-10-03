#ifndef SORT_H
#define SORT_H

#include <stdint.h>
#define AP_INT_MAX_W 2048
#include "ap_int.h"
#include <hls_math.h>

#define N 64
#define DATA_WIDTH  16
typedef ap_uint<N * DATA_WIDTH> inputtype;
typedef ap_uint<N * DATA_WIDTH> outputtype;
typedef ap_uint<16> et_type;

void sort_v1(inputtype data_in, inputtype &data_out);


#endif
