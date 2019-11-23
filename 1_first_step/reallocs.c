#include <stdlib.h>
#include "../0_general/structure.h"

void Copy(const char *small, char *big, int data_volume) {
    for(int i = 0; i < data_volume; i++) {
        big[i] = small[i];
    }
}

void GiveMoreSpace (char **small, int *data_volume) {
    int temp = *data_volume;
    *data_volume *= 2;
    char* big = (char *)malloc(*data_volume * sizeof(char));
    Copy(*small, big, temp);
    free(*small);
    *small = big;
}

void CopyTwoDim(char **small, char **big, int data_volume) {
    for(int i = 0; i < data_volume; i++) {
        big[i] = small[i];
    }
}

void GiveMoreTwoDimSpace (char ***small, int *data_volume) {
    int temp = *data_volume;
    *data_volume *= 2;
    char **big = (char **)malloc(*data_volume * sizeof(char *));
    CopyTwoDim(*small, big, temp);
    free(*small);
    *small = big;
}

void Copy3Dim(CInf *small, CInf *big, int data_volume) {
    for(int i = 0; i < data_volume; i++) {
        big[i] = small[i];
    }
}

void GiveMore3DimSpace (CInf **small, int *data_volume) { 
    int temp = *data_volume;
    *data_volume *= 2;
    CInf *big = (CInf *)malloc(*data_volume * sizeof(CInf));
    Copy3Dim(*small, big, temp);
    free(*small);
    *small = big;
}

void Copy4Dim(CPipe *small, CPipe *big, int data_volume) { 
    for(int i = 0; i < data_volume; i++) {
        big[i] = small[i];
    }
}

void GiveMore4DimSpace (CPipe **small, int *data_volume) {
    int temp = *data_volume;
    *data_volume *= 2;
    CPipe *big = (CPipe *)malloc(*data_volume * sizeof(CPipe));
    Copy4Dim(*small, big, temp);
    free(*small);
    *small = big;
}