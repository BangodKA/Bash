#pragma once

int GetHomeDirLen(char **homeDir, int *home_dir_length);

int GetDir(char **dirName);

void CreateCommandPrompt(char **dirName, char *userName, char *hostName);