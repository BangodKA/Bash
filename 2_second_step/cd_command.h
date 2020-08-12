#pragma once

#include "../0_general/structure.h"

int DetectExit(char *commands);

void GoHome();

int ChangeDir(char **command);

int FindStartPoint(int home_dir_length, char *dirName, char *homeDir);

void ChangeDirAndAppearance(CInf last_command_inf, char **dirName, int *start_point, const char **invitation, int home_dir_length, char *homeDir);