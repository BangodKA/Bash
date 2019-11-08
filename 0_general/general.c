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

void FreeHeap(Comms *comm_pipes, int length) { // Освобождает от команды
    for (int j = 0; j < length; j++) {
        for (int k = 0; k < comm_pipes[j].length; k++) {
            for(int i = 0; i <= comm_pipes[j].background_comms[k].comm_length; i++) {
                free(comm_pipes[j].background_comms[k].command[i]);
            }
            free(comm_pipes[j].background_comms[k].command);
        }
        free(comm_pipes[j].background_comms);
    }
    free(comm_pipes);
}