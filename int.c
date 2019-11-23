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
#include <errno.h>

#include "0_general/structure.h"
#include "0_general/functions.h"
#include "1_first_step/command_prompt.h"
#include "1_first_step/get_command.h"
#include "2_second_step/cd_command.h"
#include "2_second_step/process_command.h"
#include "4_fourth_step/open_create_files.h"
#include "5_fifth_step/pipeline.h"


int main()
{    
    char *dirName = NULL;
    char *homeDir = NULL; 
    int home_dir_length = 0;
    char userName[LOGIN_NAME_MAX + 1];
    char hostName[HOST_NAME_MAX + 1];
    
    CreateCommandPrompt(&dirName, userName, hostName);
    GetHomeDirLen(&homeDir, &home_dir_length);

    CBack full_command;
    int start_point = home_dir_length;
    const char * invitation = "~";
    int amp_amount = 0;

    BZombies process_manager[1000];

    while(1) {

        WaitBackgroundZombies(&amp_amount, process_manager);

        printf ("%s%s@%s%s:%s%s%s%s$ ", PINK, userName, hostName, RESET, YELLOW, invitation, &dirName[start_point], RESET);

        full_command = GetNewCommPipe();

        if (full_command.not_blank) {
            
            int fd_in = -1;
            int fd_out = 0;

            if (SpecifyThreads(full_command.arrows, &fd_out, &fd_in, full_command.which_arrow_last) == -1) {
                break;
            }

            if (CheckExitAndCd(full_command, &dirName, &start_point, &invitation, home_dir_length, homeDir) == -1) {
                break;
            }

            ProcessFullCommand(full_command, fd_in, fd_out, &amp_amount, process_manager);
        }
        FreeHeap(full_command);
    }

    free(dirName);

    return 0;
}