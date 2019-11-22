#pragma once

int GetHomeDirLen(char **homeDir);

int GetDir(char **dirName);

void CreateCommandPrompt(char **dirName, char *userName, char *hostName, int *dir_length);