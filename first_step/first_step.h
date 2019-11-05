#pragma once

int GetHomeDirLen(char **homeDir);

int GetDir(char **dirName);

void CreateCommandPrompt(char **dirName, char *userName, char *hostName, int *dir_length);

int GetNewCommandWord(char **command, int *length, int *arg_length, int *ampersand, int *last_amp);

char **GetNewCommand(CInf *commands, int len, int *exit_symbol, int *last_amp);

Comms GetNewString();

CPipe GetNewCommPipe();