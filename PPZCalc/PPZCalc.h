#ifndef KRNL1_SQUARE_H
#define KRNL1_SQUARE_H

#include <stdint.h>
#include <hls_stream.h>

#define MaxN 3

extern "C" {
void Kernel1_Square(hls::stream<int>& InputK1, hls::stream<int>& Out);
}

#endif // KRNL1_SQUARE_H