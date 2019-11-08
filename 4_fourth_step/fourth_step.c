#include <stdio.h> 
#include <stdlib.h> // Для определения домашней директории
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "../0_general/general.h"
#include "../2_3_second_third_step/second_third_step.h"

void ProcessCommsPipe (CPipe full_command, int part_number, int *fd, int *save) {
    int amp_amount = 0;
    /*int child = fork();
    if (child == 0) {
        dup2(fd[1], 1);
        if (part_number != 0) {
            dup2(fd[0], 0);
        }
        close(fd[1]);
        close(fd[0]);
        if (part_number != 0) {
            ProcessCommsPipe (full_command, part_number - 1, fd);
        }
        exit(EXIT_SUCCESS);
    }
    else {*/
        /*if ((part_number != 0)) {
            dup2(fd[0], 0);
        }*/
        //close(fd[0]);
        /*if ((part_number != full_command.length - )) {
            
        }*/
        //close(fd[1]);
        //dup2(fd[0], 0);
        if (part_number != 0) {
            ProcessCommsPipe (full_command, part_number - 1, fd, save);
        }
        for (int k = 0; k < full_command.comm_pipes[part_number].length; k++) {
            char **temp_command = full_command.comm_pipes[part_number].background_comms[k].command;
            if (((k == full_command.comm_pipes[part_number].length - 1) && (part_number != full_command.length - 1)) || ((k == 0) && (part_number != 0))) {
                int child = fork();
                if (child == 0) {
                    if ((k == full_command.comm_pipes[part_number].length - 1) && (part_number != full_command.length - 1)) {
                        printf("%s 0\n", temp_command[0]);
                        dup2(fd[1], 1);
                        close(fd[1]);
                        close(fd[0]);
                        ProcessCommand(temp_command, full_command.comm_pipes[part_number].background_comms[k].ampersand, &amp_amount);
                    }
                    exit(EXIT_SUCCESS);
                }
                else
                {
                    if ((k == 0) && (part_number != 0)) {
                        printf("%s 1\n", temp_command[0]);
                        dup2(fd[0], 0);
                        close(fd[1]);
                        close(fd[0]);
                        ProcessCommand(temp_command, full_command.comm_pipes[part_number].background_comms[k].ampersand, &amp_amount);
                    }
                }
                wait(NULL);
                continue;
            }
            
            close(fd[1]);
            close(fd[0]);
            //printf("%s,  %d  \n", temp_command[0], full_command.comm_pipes[part_number].background_comms[k].ampersand);
            ProcessCommand(temp_command, full_command.comm_pipes[part_number].background_comms[k].ampersand, &amp_amount);
        }
    //}
}