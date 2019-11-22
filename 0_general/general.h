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

int WaitBackgroundZombies(int *amp_amount);

void FreeHeap(CBack full_command);