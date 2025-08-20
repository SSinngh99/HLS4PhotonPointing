#include "hls_stream.h"
#include "Kernel1Square.h"

void SquareStream(hls::stream<int>& InputStream, hls::stream<int>& OutStream){
    int inVal, inputSq;
    do {
    inVal = InputStream.read();
    inputSq = inVal * inVal;
    OutStream.write(inputSq);
    }while (!InputStream.empty());
}

void Kernel1_Square(hls::stream<int>& InputK1, hls::stream<int>& Out) {
#pragma HLS INTERFACE axis port = InputK1 depth=MaxN

SquareStream(InputK1, Out);
}

