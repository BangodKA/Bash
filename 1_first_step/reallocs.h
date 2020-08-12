#pragma once

#include "../0_general/structure.h"

void Copy(const char *small, char *big, int data_volume);

void GiveMoreSpace (char **small, int *data_volume);

void CopyTwoDim(char **small, char **big, int data_volume);

void GiveMoreTwoDimSpace (char ***small, int *data_volume);

void Copy3Dim(CInf *small, CInf *big, int data_volume);

void GiveMore3DimSpace (CInf **small, int *data_volume);

void Copy4Dim(CPipe *small, CPipe *big, int data_volume);

void GiveMore4DimSpace (CPipe **small, int *data_volume);

void CopyArrs(Arrs **small, Arrs **big, int data_volume);

void GiveMoreArrsSpace (Arrs ***small, int data_volume);