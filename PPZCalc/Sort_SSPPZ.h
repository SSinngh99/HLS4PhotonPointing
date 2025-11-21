#ifndef SORT_SSPPZ_H
#define SORT_SSPPZ_H

#include "PPZCalc.h"

void SortPPZArr(const fZ In[NMaxClusters], fZ Out[NMaxClusters]);
fZ DeltaPPZ(const fZ In[NMaxClusters]);
fZ tournament_min(const fZ diffs[NMaxClusters-1]);
#endif // SORT_SSPPZ_H