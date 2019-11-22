#include <stdio.h> 
#include <stdlib.h> // Для определения домашней директории
#include <sys/wait.h>
#include <string.h>

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
    /*Arrs* double_arrow;
    Arrs arrow;
    Arrs back_arrow;*/
    Arrs arrows[3];
    int which_arrow_last;
}CBack;

void WaitBackgroundZombies(int *amp_amount) {
    int child;
    child = waitpid(-1, NULL, WNOHANG);
    while (child > 0) {
        printf("[%d]+ Завершён %d        \n", *amp_amount, child);
        (*amp_amount)--;
        child = waitpid(-1, NULL, WNOHANG);
    }
}

void FreeCommand(CPipe *background_pipes, int length) {
    for (int j = 0; j < length; j++) {
        for (int k = 0; k < background_pipes[j].length; k++) {
            for(int i = 0; i <= background_pipes[j].pipe_comms[k].length; i++) {
                free(background_pipes[j].pipe_comms[k].command[i]);
            }
            free(background_pipes[j].pipe_comms[k].command);
        }
        free(background_pipes[j].pipe_comms);
    }
    free(background_pipes);
}

void FreeFiles(Arrs *arrows) {
    for (int k = 0; k < 3; k++) {
        free(arrows[k].file_name);
    }
}

void FreeHeap(CBack full_command) { // Освобождает от команды
    FreeCommand(full_command.background_pipes, full_command.length);
    FreeFiles(full_command.arrows);   
}