#include <stdio.h> 
#include <stdlib.h> // Для определения домашней директории
#include <sys/wait.h>
#include <string.h>

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
        /*for (int k = 0; k < full_command.background_pipes[i].length; k++) {
            for (int s = 0; s < full_command.background_pipes[i].pipe_comms[k].comm_length; s++) {
                printf("%s ", full_command.background_pipes[i].pipe_comms[k].command[s]);
            }
            printf("| ");
        }
        printf("\n");    */
        (*amp_amount)--;
        child = waitpid(-1, NULL, WNOHANG);
        /*for (int j = 0; j < full_command.commands[k].comm_length; j++) {
            printf("%s ", full_command.commands[k].command[j]);
        }*/
    }
}

void FreeHeap(CPipe *background_pipes, int length) { // Освобождает от команды
    for (int j = 0; j < length; j++) {
        for (int k = 0; k < background_pipes[j].length; k++) {
            for(int i = 0; i <= background_pipes[j].pipe_comms[k].comm_length; i++) {
                free(background_pipes[j].pipe_comms[k].command[i]);
            }
            free(background_pipes[j].pipe_comms[k].command);
        }
        free(background_pipes[j].pipe_comms);
    }
    free(background_pipes);
}