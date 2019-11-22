#define RESET   "\033[0m"
#define DIR   "\033[32;22;1m"
#define HOST  "\033[34;22;1m"
#define PINK  "\033[35;1m"
#define YELLOW "\033[33;1m"


#define _GNU_SOURCE
#include <limits.h>
#include <unistd.h>
#include <stdio.h> 
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

#include "0_general/general.h"
#include "1_first_step/command_prompt.h"
#include "1_first_step/get_command.h"
#include "2_second_step/cd_command.h"
#include "2_second_step/process_command.h"
#include "4_fourth_step/open_create_files.h"


int main()
{    
    char *dirName = NULL;
    int dir_length = 0;
    char *homeDir = NULL; 
    int home_dir_length = 0;
    char userName[LOGIN_NAME_MAX + 1];
    char hostName[HOST_NAME_MAX + 1];

    CBack full_command;

    CreateCommandPrompt(&dirName, userName, hostName, &dir_length);

    home_dir_length = GetHomeDirLen(&homeDir);

    int start_point = home_dir_length;

    const char * const twiddle = "~";
    const char * const nothing = "";

    const char * invitation = twiddle;


    int amp_amount = 0;

    while(1) {
        WaitBackgroundZombies(&amp_amount);

        printf ("%s%s@%s%s:%s%s%s%s$ ", PINK, userName, hostName, RESET, YELLOW, invitation, &dirName[start_point], RESET);

        full_command = GetNewCommPipe();

        if (full_command.not_blank) {

            CInf last_command_inf = full_command.background_pipes[full_command.length - 1].pipe_comms[full_command.background_pipes[full_command.length - 1].length - 1];

            int fd_in = -1;
            int fd_out = -1;

            if ((fd_out = Read(full_command.arrows[0])) == -1) {
                printf("No such file\n"); 
                break;
            }
            if (full_command.which_arrow_last == 1) {
                fd_in = TruncWrite(full_command.arrows[1]);
                close(AppendWrite(full_command.arrows[2]));
            }
            else if(full_command.which_arrow_last == 2) {
                fd_in = AppendWrite(full_command.arrows[2]);
                close(TruncWrite(full_command.arrows[1]));
            }

            if (fd_in == -1) {
                fd_in = dup(1);
            }

            if (fd_out == 0) {
                fd_out = dup(0);
            }

            if ((!full_command.last_is_pipeline) && (!full_command.last_is_background)) {

                if (!strcmp(last_command_inf.command[0], "exit")) {
                    FreeHeap(full_command);
                    break;
                }

                if (!strcmp(last_command_inf.command[0], "cd")) {
                    if (last_command_inf.length > 2) {
                        printf("bash: cd: слишком много аргументов\n");
                    }
                    else if(ChangeDir(last_command_inf.command)) {
                        dir_length = GetDir(&dirName);

                        start_point = FindStartPoint(home_dir_length, dir_length, dirName, homeDir);

                        invitation = NeedTwiddle(start_point, nothing, twiddle);
                    }
                }
            }

            for (int i = 0; i < full_command.length; i++) {
                pid_t eldest_daughter = fork();
                if (eldest_daughter == 0) {
                    int pipeline[full_command.background_pipes[i].length][2];
                    pipe(pipeline[0]);
                    pipeline[0][0] = dup(fd_out);
                    //close(pipeline[0][1]);
                    int last_command_in_pipeline = 0;
                    for (int k = 0; k < full_command.background_pipes[i].length; k++) {
                        CInf temp_command_inf = full_command.background_pipes[i].pipe_comms[k];
                        pipe(pipeline[k + 1]);
                        if (k == full_command.background_pipes[i].length - 1) {
                            //printf("sfgdf %s\n", full_command.arrow.file_name[full_command.arrow.length - 1]);
                            pipeline[k + 1][1] = dup(fd_in);
                            if (fd_in != 1) {
                                close(fd_in);
                            }
                            close(pipeline[k + 1][0]);
                            last_command_in_pipeline = 1;
                        }
                            ProcessCommand(temp_command_inf.command, pipeline[k + 1][1], pipeline[k][0], last_command_in_pipeline);
                            close(pipeline[k][0]);
                            close(pipeline[k + 1][1]);
                    }
                    exit(EXIT_SUCCESS);
                }
                else {
                    if (!full_command.background_pipes[i].ampersand) {
                        waitpid(eldest_daughter, NULL, 0);
                    }
                    else {
                        amp_amount++;
                        printf("[%d] %d\n", amp_amount, eldest_daughter);
                        usleep(5000);
                    }
                }
            }
        }
        
        FreeHeap(full_command); // Освобождаем место для новой команды
    }

    return 0;
}