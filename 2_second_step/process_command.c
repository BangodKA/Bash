#include <stdio.h> 
#include <stdlib.h> // Для определения домашней директории
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void ProcessCommand(char **command, int where, int from, int wait_last) { // Выполнение команды API
    pid_t child;
    child = fork();
    if (child  < 0) {
        perror("");
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
            waitpid(child, NULL, WUNTRACED);
        }
    }
}