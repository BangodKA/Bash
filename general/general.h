#pragma once

typedef struct CommInf{
    char **command;
    int size;
    int comm_length;
    int *arg_length;
    int ampersand;
}CInf;

typedef struct Commands{
    CInf *commands;
    int length;
}Comms;

void FreeHeap(CInf *commands, int length);