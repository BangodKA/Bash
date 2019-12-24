#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include "structure.h"
#include "../1_first_step/reallocs.h"

void PrintBackgroundZombie(CPipe process_pipe, pid_t child) {

    for (int k = 0; k < process_pipe.length; k++) {
        for (int l = 0; l < process_pipe.pipe_comms[k].length; l++) {
            printf("%s ", process_pipe.pipe_comms[k].command[l]);
        }
        if (k != process_pipe.length - 1) {
            printf("| ");
        }
    }
    printf("\n");
}

void FreeBackgroundZombie(CPipe *process_pipe, int length, int index) {
    for (int k = 0; k < length; k++) {
        for(int i = 0; i < (*process_pipe).pipe_comms[k].length; i++) {
            free((*process_pipe).pipe_comms[k].command[i]);
        }
        free((*process_pipe).pipe_comms[k].command);
    }
    free((*process_pipe).pipe_comms);
}

void WaitBackgroundZombies(int *amp_amount, BZombies *process_manager) {
    pid_t child;
    child = waitpid(-1, NULL, WNOHANG);
    while (child > 0) {
        int index = child % 1000;
        printf("[%d]+  Завершён       ", process_manager[index].process_number);
        PrintBackgroundZombie(process_manager[index].background_pipe, child);
        FreeBackgroundZombie(&process_manager[index].background_pipe, process_manager[index].background_pipe.length, index);
        (*amp_amount)--;
        child = waitpid(-1, NULL, WNOHANG);
    }
}

void RememberBackgroundZombie(CPipe *process_pipe, CPipe background_pipe) {
    (*process_pipe).pipe_comms = (CInf *)malloc(background_pipe.length * sizeof(CInf));
    (*process_pipe).length = background_pipe.length;
    for (int s = 0; s < background_pipe.length; s++) {
        (*process_pipe).pipe_comms[s].command = (char **)malloc(background_pipe.pipe_comms[s].length * sizeof(char *));
        (*process_pipe).pipe_comms[s].length = background_pipe.pipe_comms[s].length;
        for (int l = 0; l < background_pipe.pipe_comms[s].length; l++) {
            int word_size = 16;
            (*process_pipe).pipe_comms[s].command[l] = (char *)malloc(word_size * sizeof(char));
            int j = 0;
            while (background_pipe.pipe_comms[s].command[l][j] != '\0') {
                if (j == word_size) {
                    GiveMoreSpace(&background_pipe.pipe_comms[s].command[l], &word_size);
                }
                (*process_pipe).pipe_comms[s].command[l][j] = background_pipe.pipe_comms[s].command[l][j];
                j++;
            }
            (*process_pipe).pipe_comms[s].command[l][j] = background_pipe.pipe_comms[s].command[l][j];
        }
    }
}