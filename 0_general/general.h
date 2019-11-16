#pragma once

typedef struct CommInf{
    char **command;
    int size;
    int comm_length;
    int *arg_length;
    int vert_slash;
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
    Arrs double_arrow;
    Arrs arrow;
    Arrs back_arrow;
}CBack;

void FreeHeap(CPipe *background_pipes, int length);