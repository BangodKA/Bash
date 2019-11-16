#define _GNU_SOURCE // Для использования констант максимальных значений 
#include <limits.h> // длины пути, хоста и имени пользователя
#include <unistd.h> // Для использования команд bash
#include <stdio.h> 
#include <stdlib.h> // Для определения домашней директории
#include <errno.h>
#include <sys/types.h>
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
    Arrs double_arrow;
    Arrs arrow;
    Arrs back_arrow;
}CBack;

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