#pragma once

typedef struct CommInf{
    char **command;
    int size;
    int comm_length;
    int *arg_length;
    int ampersand;
}CInf;

typedef struct Commands{
    CInf *background_comms;
    int length;
}Comms;

typedef struct DoubleArrow {
    char **file_name;
    int size;
    int length;
}DArr;

typedef struct Arrow {
    char **file_name;
    int size;
    int length;
}Arr;

typedef struct BackArrow {
    char **file_name;
    int size;
    int length;
}BArr;

typedef struct CommandPipe{
    Comms *comm_pipes;
    int length;
    int is_pipeline;
    DArr double_arrow;
    Arr arrow;
    BArr back_arrow;
}CPipe;

void Copy(const char *small, char *big, int data_volume);

void GiveMoreSpace (char **small, int *data_volume);

void CopyTwoDim(char **small, char **big, int data_volume);

void GiveMoreTwoDimSpace (char ***small, int *data_volume);

void Copy3Dim(CInf *small, CInf *big, int data_volume);

void GiveMore3DimSpace (CInf **small, int *data_volume);

void Copy4Dim(Comms *small, Comms *big, int data_volume);

void GiveMore4DimSpace (Comms **small, int *data_volume);