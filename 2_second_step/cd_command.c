#include <stdio.h> 
#include <stdlib.h> // Для определения домашней директории
#include <string.h>
#include <unistd.h>

#include "../0_general/structure.h"
#include "../1_first_step/command_prompt.h"

void GoHome() {
    char *tempHome = NULL;
    tempHome = getenv("HOME");
    chdir(tempHome);
}

int ChangeDir(char **command) {
    if (command[1] != NULL) {
        if (!strncmp(command[1], "~", 1)) {
            GoHome();
            if (command[1][1] == '\0') {
                return 1;
            }
            strcpy(command[1], (&command[1][2]));
        }
        if (chdir(command[1]) == -1) {
            perror(command[1]);
            return 0;
        }
        return 1;
    }

    return 0;
}

int FindStartPoint(int home_dir_length, char *dirName, char *homeDir) {    
    if (!strncmp(dirName, homeDir, home_dir_length)) {
        return home_dir_length;
    }
    return 0;
}

void ChangeDirAndAppearance(CInf last_command_inf, char **dirName, int *start_point, const char **invitation, int home_dir_length, char *homeDir) {
    if (!strcmp(last_command_inf.command[0], "cd")) {
        if (last_command_inf.length > 2) {
            printf("bash: cd: слишком много аргументов\n");
        }
        else if(ChangeDir(last_command_inf.command)) {
            GetDir(dirName);

            *start_point = FindStartPoint(home_dir_length, *dirName, homeDir);

            *invitation = (*start_point == 0) ? ("") : ("~");
        }
    }
}
