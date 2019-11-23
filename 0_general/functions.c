#define RESET   "\033[0m"
#define DIR   "\033[32;22;1m"
#define HOST  "\033[34;22;1m"
#define PINK  "\033[35;1m"
#define YELLOW "\033[33;1m"

#include <unistd.h>
#include <stdio.h> 
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

#include "structure.h"
#include "../1_first_step/reallocs.h"
#include "../1_first_step/command_prompt.h"
#include "../1_first_step/get_command.h"
#include "../2_second_step/cd_command.h"
#include "../4_fourth_step/open_create_files.h"
#include "../5_fifth_step/pipeline.h"

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
        for(int i = 0; i <= (*process_pipe).pipe_comms[k].length; i++) {
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

int CheckExitAndCd(CBack full_command, char **dirName, int *start_point, const char **invitation, int home_dir_length, char *homeDir) {
    if ((!full_command.last_is_pipeline) && (!full_command.last_is_background)) {
                
        CInf last_command_inf = full_command.background_pipes[full_command.length - 1].pipe_comms[full_command.background_pipes[full_command.length - 1].length - 1];

        if (!strcmp(last_command_inf.command[0], "exit")) {
            return -1;
        }
        
        ChangeDirAndAppearance(last_command_inf, dirName, start_point, invitation, home_dir_length, homeDir);
    }
    return 0;
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

int ProcessFullCommand(CBack full_command, int fd_in, int fd_out, int *amp_amount, BZombies *process_manager) {
    for (int i = 0; i < full_command.length; i++) {
        if (SpecifyThreads(full_command.arrows[i], &fd_out, &fd_in, full_command.which_arrow_last) == -1) {
            return -1;
        }
        pid_t eldest_daughter = fork();
        if (eldest_daughter == 0) {
            ProcessPipeline(full_command.background_pipes[i], fd_in, fd_out);
            exit(EXIT_SUCCESS);
        }
        else {
            if (!full_command.background_pipes[i].ampersand) {
                waitpid(eldest_daughter, NULL, 0);
            }
            else {
                (*amp_amount)++;
                printf("[%d] %d\n", *amp_amount, eldest_daughter);
                int index = eldest_daughter % 1000;
                process_manager[index].process_number = *amp_amount;
                RememberBackgroundZombie(&process_manager[index].background_pipe, full_command.background_pipes[i]);
                usleep(5000);
            }
        }
    }

    return 0;
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

void FreeFiles(Arrs **arrows, int length) {
    for (int i = 0; i < length; i++) {
        for (int k = 0; k < 3; k++) {
            for (int s = 0; s < arrows[i][k].length; s++) {
                free(arrows[i][k].file_name[s]);
            }
            free(arrows[i][k].file_name);
        }
        free(arrows[i]);
    }
    free(arrows);
}

void FreeHeap(CBack full_command) {
    FreeCommand(full_command.background_pipes, full_command.length);
    FreeFiles(full_command.arrows, full_command.length);   
}

void ProcessShell(int home_dir_length, char *userName, char *hostName, char *dirName, char *homeDir) {
    CBack full_command;
    int start_point = home_dir_length;
    const char * invitation = "~";
    int amp_amount = 0;

    BZombies process_manager[1000];

    while(1) {
        WaitBackgroundZombies(&amp_amount, process_manager);

        printf ("%s%s@%s%s:%s%s%s%s$ ", PINK, userName, hostName, RESET, YELLOW, invitation, &dirName[start_point], RESET);

        full_command = GetNewCommPipe();

        if (full_command.not_blank) {
            
            int fd_in = -1;
            int fd_out = 0;

            if (CheckExitAndCd(full_command, &dirName, &start_point, &invitation, home_dir_length, homeDir) == -1) {
                break;
            }

            if (ProcessFullCommand(full_command, fd_in, fd_out, &amp_amount, process_manager) == -1) {
                break;
            }
        }
        FreeHeap(full_command);
    }
}

void MakeCommandPrompt(char **dirName, char *userName, char *hostName, char **homeDir, int *home_dir_length) {
    CreateCommandPrompt(dirName, userName, hostName);
    GetHomeDirLen(homeDir, home_dir_length);
}