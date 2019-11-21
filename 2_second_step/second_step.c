#include <stdio.h> 
#include <stdlib.h> // Для определения домашней директории
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

/*int DetectExit(char *command) { // Обнаруживает exit
    if (command != NULL) {
        if (strcmp(command, "exit") == 0) {
            return 1;
        }
    }
    return 0;
}*/

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

void ProcessCommand(char **command, int where, int from, int wait_last) { // Выполнение команды API
    //printf("%s, %d", command[0], ampersand);
    pid_t child;
    child = fork();
    if (child  < 0) {
        printf("%s\n", "Something went wrong");
    }
    else if (child == 0) {
        dup2(from, 0);
        dup2(where, 1);
        close(from);
        close(where);
        if (!strcmp(command[0], "exit")) {
            exit(EXIT_SUCCESS);
        }
        if (!strcmp(command[0], "cd")) {
            exit(EXIT_SUCCESS);
        }
        execvp(command[0], command);
        printf("%s: команда не найдена\n", command[0]);
        exit(EXIT_FAILURE);
    }
    else {
        if (wait_last) {
            //printf("%s", command[0]);
            waitpid(child, NULL, WUNTRACED);
        }
    }
}