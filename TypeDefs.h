#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <ap_fixed.h>

////////////////////////////////////////////////
///////////////// PPZ CALC /////////////////////
////////////////////////////////////////////////

// This NEEDS TO BE OPTIMIZED AS OF APRIL 2026

#define EtaIdxOverFlow -9999
#define PPZOverFlow 5000.0
#define NMaxTOBs 15
#define DeltaPPZCut 100 // To be changed

#define DecimalPrecisionPPZ 20 // Remember to change the sinh map when changing this otherwise there is no effect!!
#define fZWord 42
#define fZIntBits fZWord - DecimalPrecisionPPZ
#define fEtaPPZWord DecimalPrecisionPPZ + 4 
#define fEtaPPZIntBits fEtaPPZWord - DecimalPrecisionPPZ



typedef  ap_fixed<fEtaPPZWord, fEtaPPZIntBits> fEtaPPZ; // float for sinh(x), 18 bits in total, 3 + 1 signed bit for the integer part (-8, 8) and 8 bits for the decimal (1/2^8)
typedef  ap_fixed<fZWord, fZWord - DecimalPrecisionPPZ> fZ; // float for Z1 and Z2, 29 bits total, 14+1 for integer (+/-16384), 8 bit decimal

////////////////////////////////////////////////
///////////////// TOWER BC CALC ////////////////
////////////////////////////////////////////////

#define NCells 16 // Number of cells --> 16 in L2
#define fETWord 12 // Size of word for single ET in cell
#define fETDecimalBits 4 // Decimal precision for ET in cell
#define DecimalPrecisionBits 16 // Decimal precision we need/want for barycenter calc

#define fETIntBits fETWord - fETDecimalBits

#define fBetaWord fETWord + 4 // Max(fBeta) = 16 * ET_cell --> 4 more integer bits than fETWord, use same decimal precision as fETDecimalBits
#define fBetaIntBits fBetaWord - fETDecimalBits

#define fAlphaIntBits fBetaIntBits + 2
#define fAlphaWord fAlphaIntBits + DecimalPrecisionBits // max(alpha) = 2.5 * beta --> 2 integer bits more than beta, but w/ full precision in decimal bits


typedef  ap_fixed<fETWord, fETIntBits, AP_TRN, AP_SAT> fET; // float for ET, can be changed on the fly.
typedef  ap_fixed<DecimalPrecisionBits + 2, 2> fEta; // Decimal precision needed for 0.025
typedef  ap_uint<1> SingleBitInt; // Single bit for state
typedef  ap_uint<5> FiveBitInt; // Five bit int for CellIdx and NETIdx --> easier to test since it has a 16th idx --> EMIT signal


typedef  ap_fixed<fAlphaWord, fAlphaIntBits> fAlpha; // Should have 2 more integer bits than fBeta, and DecimalPrecisionBits number of fractional bits --> Total word = fETWord - fETDecimalPrec + 2 + DecimalPrecision
typedef  ap_fixed<fBetaWord, fBetaIntBits> fBeta; // Can change to be something else, preferably the scheme for full tower ET

////////////////////////////////////////////////
///////////////// MAX CELL L1 //////////////////
////////////////////////////////////////////////

#define fEtaIdxL1Word 11 // 11 bits total needed to represent L1 from -2.5 to 2.5
#define MaxNCellsL1 32 // Number of max N cells --> technically this is different for different areas in eta.... but max is 32 --> keep counting till 32 before forcefully sending EMIT
#define OverflowL1IdxEta 2000 // There should never be idx 2000 in L1 so it is an overflow idx
typedef ap_uint<fEtaIdxL1Word> fEtaIdxL1; 
typedef  ap_uint<6> SixBitInt; // Six bit int for CellIdx for L1 --> easier to test since it has a 32nd idx --> EMIT signal

////////////////////////////////////////////////
///////////////// PER TOB CALC /////////////////
////////////////////////////////////////////////

typedef ap_uint<3> ThreeBitInt; // Max towers/TOB = 4, count to 2+1 bits
#define MaxNTowersPerTOB 4

#endif // TYPEDEFS_H
