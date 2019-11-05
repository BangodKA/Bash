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
    CInf *commands;
    int length;
}Comms;

typedef struct CommandPipe{
    Comms *comm_pipes;
    int length;
}CPipe;

void FreeHeap(CInf *commands, int length) { // Освобождает от команды
    for (int k = 0; k < length; k++) {
        for(int i = 0; i <= commands[k].comm_length; i++) {
            free(commands[k].command[i]);
        }
        free(commands[k].command);
    }
    free(commands);
}