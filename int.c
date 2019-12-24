#define _GNU_SOURCE
#include <limits.h>
#include <stdlib.h>

#include "0_general/functions.h"

int main()
{    
    char *dirName = NULL;
    char *homeDir = NULL; 
    int home_dir_length = 0;
    char userName[_POSIX_LOGIN_NAME_MAX + 1];
    char hostName[_POSIX_HOST_NAME_MAX + 1];

    MakeCommandPrompt(&dirName, userName, hostName, &homeDir, &home_dir_length);

    ProcessShell(home_dir_length, userName, hostName, dirName, homeDir);
    return 0;
}