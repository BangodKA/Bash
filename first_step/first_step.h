#pragma once

int GetHomeDirLen(char **homeDir);

int GetDir(char **dirName);

void CreateCommandPrompt(char **dirName, char *userName, char *hostName, int *dir_length);

int GetNewCommandWord(char **command, int *length, int *arg_length, int *ampersand, int *last_amp);

void GetNewCommand(CInf *commands, int len, int *exit_symbol, int *last_amp);

void GetNewString(Comms *comms_pipe, int pipe_length, int *exit_symbol);

CPipe GetNewCommPipe();