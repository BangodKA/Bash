#include <stdio.h>
#include <stdlib.h> // Для определения домашней директории
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "../0_general/general.h"
#include "../2_3_second_third_step/second_third_step.h"

void ProcessCommsPipe (CPipe full_command, int part_number) {
    int amp_amount;
    for (int s = 0; s < full_command.length; s++) {
        for (int k = 0; k < full_command.comm_pipes[s].length; k++) {
            char **temp_command = full_command.comm_pipes[s].background_comms[k].command;
            if (((k == 0) && (s != 0)) || ((k == full_command.comm_pipes[s].length - 1) && (s != full_command.length - 1))) {
                int fd[2];
                pipe(fd);
                int daughter = fork();
                if (daughter == 0) {
                    //printf("%s  @,  %d  \n", temp_command[0], full_command.comm_pipes[s].background_comms[k].ampersand);
                    if ((k == full_command.comm_pipes[s].length - 1) && (s != full_command.length - 1))
                    {
                        dup2(fd[1], 1);
                    }
                    close(fd[1]);
                    close(fd[0]);
                    
                    ProcessCommand(temp_command, full_command.comm_pipes[full_command.length - 1].background_comms[k].ampersand, &amp_amount);
                    exit(EXIT_SUCCESS);
                }
                else {
                    waitpid(daughter, NULL, WUNTRACED);
                    //printf("%s  #,  %d  \n", temp_command[0], full_command.comm_pipes[s].background_comms[k].ampersand);
                    dup2(fd[0], 0);
                    close(fd[1]);
                    close(fd[0]);
                }
                
            }
            else {
                ProcessCommand(temp_command, full_command.comm_pipes[s].background_comms[k].ampersand, &amp_amount);
            }
            
        }
    }
    //waitpid(child, NULL, WNOHANG);
}