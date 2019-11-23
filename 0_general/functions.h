#include <stdio.h> 
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>

#include "structure.h"
#include "../1_first_step/reallocs.h"
#include "../2_second_step/cd_command.h"
#include "../5_fifth_step/pipeline.h"

void PrintBackroundZombie(CPipe process_pipe, pid_t child);

void FreeBackgroundZombie(CPipe process_pipe, int length, int index);

void WaitBackgroundZombies(int *amp_amount, BZombies *process_manager);

int CheckExitAndCd(CBack full_command, char **dirName, int *start_point, const char **invitation, int home_dir_length, char *homeDir);

void RememberBackgroundZombie(CPipe process_pipe, CPipe background_pipe);

void ProcessFullCommand(CBack full_command, int fd_in, int fd_out, int *amp_amount, BZombies *process_manager);

void FreeCommand(CPipe *background_pipes, int length);

void FreeFiles(Arrs *arrows);

void FreeHeap(CBack full_command);