#ifndef SORT_SSPPZ_H
#define SORT_SSPPZ_H

#include "PPZCalc.h"
#include "TypeDefs.h"

void SortPPZArr(const fZ In[NMaxTOBs], fZ Out[NMaxTOBs]);
fZ DeltaPPZ(const fZ In[NMaxTOBs]);
fZ tournament_min(const fZ diffs[NMaxTOBs-1]);
#endif // SORT_SSPPZ_H