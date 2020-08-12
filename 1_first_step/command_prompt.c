#define _GNU_SOURCE
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>

#include "command_prompt.h"


void GetHomeDirLen(char **homeDir, int *home_dir_length) {
    *homeDir = getenv("HOME");
    while ((*homeDir)[*home_dir_length] != '\0') { 
        (*home_dir_length)++;
    }
}

void GetDir(char **dirName) {
    free(*dirName);
    *dirName = (char *)malloc((PATH_MAX + 1) * sizeof(char));
    getcwd(*dirName, PATH_MAX);
}

void CreateCommandPrompt(char **dirName, char *userName, char *hostName) {
    GetDir(dirName);

    getlogin_r(userName, _POSIX_LOGIN_NAME_MAX); 

    gethostname(hostName, _POSIX_HOST_NAME_MAX); 
} 