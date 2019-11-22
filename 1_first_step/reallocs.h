#pragma once

typedef struct CommInf{
    char **command;
    int length;
}CInf;

typedef struct CommandPipe{
    CInf *pipe_comms;
    int length;
    int ampersand;
}CPipe;

typedef struct Arrows {
    char **file_name;
    int size;
    int length;
}Arrs;

typedef struct BackgroundComm{
    CPipe *background_pipes;
    int length;
    int not_blank;
    int last_is_pipeline;
    int last_is_background;
    Arrs arrows[3];
    int which_arrow_last;
    /*Arrs double_arrow;
    Arrs arrow;
    Arrs back_arrow;*/
}CBack;

void Copy(const char *small, char *big, int data_volume);

void GiveMoreSpace (char **small, int *data_volume);

void CopyTwoDim(char **small, char **big, int data_volume);

void GiveMoreTwoDimSpace (char ***small, int *data_volume);

void Copy3Dim(CInf *small, CInf *big, int data_volume);

void GiveMore3DimSpace (CInf **small, int *data_volume);

void Copy4Dim(CPipe *small, CPipe *big, int data_volume);

void GiveMore4DimSpace (CPipe **small, int *data_volume);