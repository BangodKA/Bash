#include <stdio.h> 
#include <stdlib.h> // Для определения домашней директории
#include <string.h>
#include <unistd.h>

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
            printf("bash: cd: %s: Нет такого файла или каталога\n", command[1]);
            return 0;
        }
        return 1;
    }

    return 0;
}

int FindStartPoint(int home_dir_length, int dir_length, char *dirName, char *homeDir) {
    if (home_dir_length > dir_length) {
        return 0;
    }
    
    if (!strncmp(dirName, homeDir, 5)) {
        return home_dir_length;
    }
    return 0;
}

const char * const NeedTwiddle(int start_point, const char * const nothing, const char * const twiddle) {
    if (start_point == 0) {
        return nothing;
    }
    else {
        return twiddle;
    }
}
