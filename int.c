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

#include "general/general.h"
#include "first_step/first_step.h"
#include "second_third_step/second_third_step.h"


int main ()
{    
    char *dirName = NULL; // Указатель на область, где будет храниться путь к текущей директории
    int dir_length = 0; // Длина до рабочей директории
    char *homeDir = NULL; // Указатель на область, где будет храниться домашняя директория
    int home_dir_length = 0;
    char userName[LOGIN_NAME_MAX + 1]; // Указатель на область, где будет храниться имя пользователя
    char hostName[HOST_NAME_MAX + 1]; // Массив, где будет храниться имя хоста
    //object.command[0] = NULL;

    Comms object;

    CreateCommandPrompt(&dirName, userName, hostName, &dir_length);

    home_dir_length = GetHomeDirLen(&homeDir);

    int start_point = home_dir_length;

    const char * const twiddle = "~";
    const char * const nothing = "";

    const char * invitation = twiddle;


    int amp_amount = 0;

    while(1) {

        printf ("%s%s@%s%s:%s%s%s%s$ ", PINK, userName, hostName, RESET, YELLOW, invitation, &dirName[start_point], RESET);

        int child;
        child = waitpid(-1, NULL, WNOHANG);
        if(child > 0) {
            printf("\n[%d]+ Завершён %d        ", amp_amount, child);
            amp_amount--;
            /*for (int j = 0; j < object.commands[k].comm_length; j++) {
                printf("%s ", object.commands[k].command[j]);
            }*/
        }

        
        object = GetNewString();

        int len = object.length;

        if (len == -1) {
            printf("bash: синтаксическая ошибка рядом с неожиданным маркером «&»\n");
            continue;
        }

        if(DetectExit(object.commands, object.length)) {
            FreeHeap(object.commands, object.length);
            break;
        }

        if ((object.length >= 1) && (object.commands[0].arg_length[0] >= 1)) {
            for (int k = 0; k < object.length; k++) {
                char **temp_command = object.commands[k].command;
                
                if (!strcmp(temp_command[0], "cd")) {
                    if (object.length > 2) {
                        printf("bash: cd: слишком много аргументов\n");
                    }
                    else if(ChangeDir(temp_command)) {
                        dir_length = GetDir(&dirName);

                        start_point = FindStartPoint(home_dir_length, dir_length, dirName, homeDir);

                        invitation = NeedTwiddle(start_point, nothing, twiddle);
                    }
                }
                else {
                    ProcessCommand(temp_command, object.commands[k].ampersand, &amp_amount);
                }
            }
        }

        /*for(int k = 0; k <= len; k++) {
            for(int i = 0; i < object.commands[k].comm_length; i++) {
                for(int j = 0; j < object.commands[k].arg_length[i]; j++) {
                    printf("%c", object.commands[k].command[i][j]);
                }
            }
            printf(" ");
        }*/

        FreeHeap(object.commands, object.length); // Освобождаем место для новой команды
    }

    return 0;
}