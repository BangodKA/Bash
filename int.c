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
#include "2_3_second_third_step/second_third_step.h"
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

        //printf("%s\n", full_command.background_pipes[full_command.length - 1].commands[0].command[0]);
        //printf("%s", full_command.commands[0].command[0]);

        /*int len = full_command.background_pipes[0].length;

        if (len == -1) {
            printf("bash: синтаксическая ошибка рядом с неожиданным маркером «&»\n");
            continue;
        }*/

        CInf last_command_inf = full_command.background_pipes[0].pipe_comms[full_command.background_pipes[0].length - 1];

        if ((!full_command.last_is_pipeline) && (!full_command.last_is_background)) {
            if (DetectExit(last_command_inf.command[0])) {
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
        }

        /*if (!Read(full_command.back_arrow)) {
            break;
        }
        TruncWrite(full_command.arrow);
        AppendWrite(full_command.double_arrow);*/

        
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

        if (full_command.not_blank) {
            for (int k = 0; k < full_command.background_pipes[full_command.length - 1].length; k++) {
                CInf temp_command_inf = full_command.background_pipes[full_command.length - 1].pipe_comms[k];
                
                if ((!strcmp(temp_command_inf.command[0], "cd")) && (!temp_command_inf.vert_slash)) {
                    if (full_command.background_pipes[full_command.length - 1].length > 2) {
                        printf("bash: cd: слишком много аргументов\n");
                    }
                    else if(ChangeDir(temp_command_inf.command)) {
                        dir_length = GetDir(&dirName);

                        start_point = FindStartPoint(home_dir_length, dir_length, dirName, homeDir);

                        invitation = NeedTwiddle(start_point, nothing, twiddle);
                    }
                }
                else {
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

                    ProcessCommand(temp_command_inf.command, temp_command_inf.vert_slash, &amp_amount);
                    
                    /*dup2(save_out, 1);
                    dup2(save_inp, 0);
                    close(save_out);*/
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