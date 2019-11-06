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

typedef struct CommandPipe{
    Comms *comm_pipes;
    int length;
    int is_pipeline;
}CPipe;

void FreeHeap(Comms *commands, int length);