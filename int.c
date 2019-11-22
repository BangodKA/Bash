#define RESET   "\033[0m"
#define DIR   "\033[32;22;1m"
#define HOST  "\033[34;22;1m"
#define PINK  "\033[35;1m"
#define YELLOW "\033[33;1m"


#define _GNU_SOURCE // Для использования констант максимальных значений 
#include <limits.h> // длины пути, хоста и имени пользователя
#include <unistd.h> // Для использования команд bash
#include <stdio.h> 
#include <stdlib.h> // Для определения домашней директории
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "0_general/general.h"
#include "1_first_step/first_step.h"
#include "2_second_step/second_step.h"
#include "4_fourth_step/fourth_step.h"
//#include "5_fifth_step/fifth_step.h"


int main ()
{    
    char *dirName = NULL; // Указатель на область, где будет храниться путь к текущей директории
    int dir_length = 0; // Длина до рабочей директории
    char *homeDir = NULL; // Указатель на область, где будет храниться домашняя директория
    int home_dir_length = 0;
    char userName[LOGIN_NAME_MAX + 1]; // Указатель на область, где будет храниться имя пользователя
    char hostName[HOST_NAME_MAX + 1]; // Массив, где будет храниться имя хоста
    //full_command.command[0] = NULL;

    CBack full_command;

    CreateCommandPrompt(&dirName, userName, hostName, &dir_length);

    home_dir_length = GetHomeDirLen(&homeDir);

    int start_point = home_dir_length;

    const char * const twiddle = "~";
    const char * const nothing = "";

    const char * invitation = twiddle;


    int amp_amount = 0;
    //printf("%d", getpid());

    while(1) {
        
        int child;
        child = waitpid(-1, NULL, WNOHANG);
        int i = 0;
        while (child > 0) {
            printf("[%d]+ Завершён %d        \n", amp_amount, child);
            /*for (int k = 0; k < full_command.background_pipes[i].length; k++) {
                for (int s = 0; s < full_command.background_pipes[i].pipe_comms[k].comm_length; s++) {
                    printf("%s ", full_command.background_pipes[i].pipe_comms[k].command[s]);
                }
                printf("| ");
            }
            printf("\n");    */
            amp_amount--;
            child = waitpid(-1, NULL, WNOHANG);
            /*for (int j = 0; j < full_command.commands[k].comm_length; j++) {
                printf("%s ", full_command.commands[k].command[j]);
            }*/
            i = 1;
        }
        if (i == 1) {
            continue;
        }
        printf ("%s%s@%s%s:%s%s%s%s$ ", PINK, userName, hostName, RESET, YELLOW, invitation, &dirName[start_point], RESET);

        full_command = GetNewCommPipe();

        //printf("%d\n", full_command.back_arrow.length);

        /*for (int j = 0; j < full_command.length; j++) {
            for (int k = 0; k < full_command.background_pipes[j].length; k++) {
                for(int i = 0; i < full_command.background_pipes[j].pipe_comms[k].comm_length; i++) {
                    printf("word_len: %d, ", full_command.background_pipes[j].pipe_comms[k].arg_length[i]);
                }
                printf("comm_length: %d, ", full_command.background_pipes[j].pipe_comms[k].comm_length);
            }
            printf("conv_len: %d\n", full_command.background_pipes[j].length);
        }
        printf("%d\n", full_command.length);*/

        //printf("%s\n", full_command.background_pipes[full_command.length - 1].commands[0].command[0]);
        //printf("%s", full_command.commands[0].command[0]);

        /*int len = full_command.background_pipes[0].length;

        if (len == -1) {
            printf("bash: синтаксическая ошибка рядом с неожиданным маркером «&»\n");
            continue;
        }*/

        if (full_command.not_blank) {

            CInf last_command_inf = full_command.background_pipes[full_command.length - 1].pipe_comms[full_command.background_pipes[0].length - 1];

            //printf("%d, %d; \n", full_command.last_is_background, full_command.last_is_pipeline);

            if (!Read(full_command.arrows[0])) {
                break;
            }
            TruncWrite(full_command.arrows[1]);
            AppendWrite(full_command.arrows[2]);

            if ((!full_command.last_is_pipeline) && (!full_command.last_is_background)) {
                if (!strcmp(last_command_inf.command[0], "exit")) {
                    FreeHeap(full_command.background_pipes, full_command.length);
                    /*if(fork() == 0) {
                        char *cmd = "free";
                        char *argv[2];
                        argv[0] = "free";
                        argv[1] = NULL;
                        execvp(cmd, argv);
                    }
                    wait(NULL);*/
                    break;
                }
                if (!strcmp(last_command_inf.command[0], "cd")) {
                    if (last_command_inf.comm_length > 2) {
                        printf("bash: cd: слишком много аргументов\n");
                    }
                    else if(ChangeDir(last_command_inf.command)) {
                        dir_length = GetDir(&dirName);

                        start_point = FindStartPoint(home_dir_length, dir_length, dirName, homeDir);

                        invitation = NeedTwiddle(start_point, nothing, twiddle);
                    }
                }
            }

            /*if (full_command.is_pipeline) {
                int save[2];
                pipe(save);
                dup2(1, save[1]);
                dup2(0, save[0]);
                ProcessCommsPipe(full_command, (full_command.length - 1));
                FreeHeap(full_command.background_pipes, full_command.length);
                dup2(save[1], 1);
                dup2(save[0], 0);
                close(save[1]);
                close(save[0]);
                continue;
            }*/
            for (int i = 0; i < full_command.length; i++) {
                //int fd_out = open(full_command.arrow.file_name[full_command.arrow.length - 1], O_WRONLY | O_TRUNC | O_CREAT, S_IRWXU);
                pid_t eldest_daughter = fork();
                if (eldest_daughter == 0) {
                    int pipeline[full_command.background_pipes[i].length][2];
                    pipe(pipeline[0]);
                    pipeline[0][0] = dup(0);
                    //close(pipeline[0][1]);
                    int last_command_in_pipeline = 0;
                    for (int k = 0; k < full_command.background_pipes[i].length; k++) {
                        CInf temp_command_inf = full_command.background_pipes[i].pipe_comms[k];
                        pipe(pipeline[k + 1]);
                        if (k == full_command.background_pipes[i].length - 1) {
                            //printf("sfgdf %s\n", full_command.arrow.file_name[full_command.arrow.length - 1]);
                            pipeline[k + 1][1] = dup(1);
                            close(pipeline[k + 1][0]);
                            last_command_in_pipeline = 1;
                        }
                            /*int save_out = dup(1);
                            int save_inp = dup(0);

                            int fd_out = open(full_command.arrow.file_name[full_command.arrow.length - 1], O_WRONLY | O_TRUNC | O_CREAT, S_IRWXU);
                            dup2(fd_out, 1); 
                            close(fd_out);

                            int fd_inp = open(full_command.back_arrow.file_name[full_command.back_arrow.length - 1], O_RDONLY, S_IRWXU);
                            if (fd_inp == -1) {
                                //printf("bash: %s: Нет такого файла или каталога", full_command.back_arrow.file_name[full_command.back_arrow.length - 1]);
                                break;
                            }
                            dup2(fd_inp, 0);
                            close(fd_inp);*/

                            ProcessCommand(temp_command_inf.command, pipeline[k + 1][1], pipeline[k][0], last_command_in_pipeline);
                            close(pipeline[k][0]);
                            close(pipeline[k + 1][1]);
                            
                            /*dup2(save_out, 1);
                            dup2(save_inp, 0);
                            close(save_out);*/
                    }
                    /*for (int k = 0; k <= full_command.background_pipes[i].length; k++) {
                        close(pipeline[k][0]);
                        close(pipeline[k][1]);
                    }*/
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

        /*for(int k = 0; k <= len; k++) {
            for(int i = 0; i < full_command.commands[k].comm_length; i++) {
                for(int j = 0; j < full_command.commands[k].arg_length[i]; j++) {
                    printf("%c", full_command.commands[k].command[i][j]);
                }
            }
            printf(" ");
        }*/

        FreeHeap(full_command.background_pipes, full_command.length); // Освобождаем место для новой команды
    }

    return 0;
}