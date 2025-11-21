//*******************Sorting ******************************//
/*Version 2.1
Company: University of Pittsburgh
Designer: Yuvaraj Elangovan, Tae Min Hong
Date: 2024/05/16
*/

//***************Configurations*****************//
/*
Clock: 240 MHz
Sort length N = 8 to 64
Datawidth : 16bits
inputtype or outputype : N * Datawidth
*/

#include <stdint.h>
#define AP_INT_MAX_W 2048
#include <ap_int.h>
#include <hls_math.h>
#include "Sort.h"

void sort_v1(inputtype data_in, outputtype &data_out)
{
et_type sorted[N];
et_type arr[N];
bool compares[N-1];

#pragma HLS ARRAY_PARTITION variable=arr complete
#pragma HLS ARRAY_PARTITION variable=compares complete
#pragma HLS ARRAY_PARTITION variable=sorted complete

	//Initialize
	for (int i = 0; i < N; i++)
	{
#pragma HLS UNROLL
		arr[i] = 0;
	}

	for (int j = 0; j < N; j++)
	{
#pragma HLS UNROLL
		arr[j] = data_in(j*16+15,j*16);
	}


	for(int i = 0; i < N; i++)
	{
#pragma HLS UNROLL

		for(int j = 0; j < N; j++)
		{
#pragma HLS UNROLL
			if(j<i)
			{
				if (arr[i] >= arr[j])
				{
					compares[j]=1;
				}
				else
				{
					compares[j]=0;
				}
			}
			else if(j>i)
			{
				if (arr[i] > arr[j])
				{
					compares[j-1]=1;
				}
				else
				{
					compares[j-1] = 0;
				}
			}

		}
		int sum=0;
		for(int i = 0; i < N-1; i++)
		{
#pragma HLS UNROLL
			sum += compares[i];
		}
		sorted[sum] = arr[i];

	}

		for (int j = 0; j < N; j++)
		{
	#pragma HLS UNROLL
			data_out(j*16+15,j*16) = sorted[j];
		}
}


//*************************************************************************************************//



//Simple sorting
//void sort_v1(inputtype arr[N], inputtype sorted[N])
//{
//
//bool compares[N-1];
//#pragma HLS ARRAY_PARTITION variable=arr complete
////#pragma HLS INTERFACE ap_none port=arr
//#pragma HLS ARRAY_PARTITION variable=compares complete
//#pragma HLS ARRAY_PARTITION variable=sorted complete
//
//	for(int i = 0; i < N; i++)
//	{
//#pragma HLS UNROLL
//
//		for(int j = 0; j < N; j++)
//		{
//#pragma HLS UNROLL
//			if(j<i)
//			{
//				if (arr[i] >= arr[j])
//				{
//					compares[j]=1;
//				}
//				else
//				{
//					compares[j]=0;
//				}
//			}
//			else if(j>i)
//			{
//				if (arr[i] > arr[j])
//				{
//					compares[j-1]=1;
//				}
//				else
//				{
//					compares[j-1] = 0;
//				}
//			}
//
//		}
//		int sum=0;
//		for(int i = 0; i < N-1; i++)
//		{
//#pragma HLS UNROLL
//			sum += compares[i];
//		}
//		sorted[sum] = arr[i];
//
//	}
//
//	return;
//}


//
//
//void sort_v0(inputtype data_in[GRID_LEN], outputtype data_out[GRID_LEN])
//{
////#pragma HLS INTERFACE axis port=data bundle=control
////#pragma HLS INTERFACE axis port=Median_ET bundle=control
////#pragma HLS interface ap_ctrl_none port=return
////#pragma HLS interface ap_memory port=lookup_table
//#pragma HLS ARRAY_PARTITION variable=data_in
////unsigned char tmp;
////inputtype med_val;
//bool noofones[GRID_LEN];
//#pragma HLS ARRAY_PARTITION variable=noofones complete dim=0
//
//
// for(int i = 0; i < GRID_LEN; i++)
// {
// #pragma HLS UNROLL
//	 noofones[i] = 0;
// 	//inner1
//     for(int j = 0; j < GRID_LEN; j++)
//     {
//	#pragma HLS UNROLL
//
//     	if(i<j)
//     	{
//     		if(data_in[i] >= data_in[j])
//     		{
//     			noofones[i] = 1;
//     		}
//     		else
//     		{
//     			noofones[j] = 0;
//     		}
//     	}
//
//     	if(i>j)
//     	{
//     		if(data_in[i] > data_in[j])
//     		{
//     			noofones[i] = 1;
//     		}
//     		else
//     		{
//     			noofones[j] = 0;
//     		}
//     	}
//     }
// }
////	if (noofones[i]==GRID_LEN/2)
////	{
////		Median_ET = data[i];
////	}
//// }
//    tmp=0;
//	for (int x = 0; x < GRID_LEN; x++)
//	{
//	#pragma HLS UNROLL
//		tmp += noofones[x];
//	}
////tmp += noofones[x];
////	if (tmp==GRID_LEN/2)
////	{
////		Median_ET = data[i];
////	}
//// }
////Median_ET = med_val;
//
//	for(int i = 0; i < GRID_LEN; i++)
//	{
//	#pragma HLS UNROLL
//		if(noofones[i] == (GRID_LEN/2))
//		{
//			Median_ET = data[i];
//		}
//	}
//
//}

