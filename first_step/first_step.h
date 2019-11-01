#pragma once

int GetHomeDirLen(char **homeDir);

int GetDir(char **dirName);

void CreateCommandPrompt(char **dirName, char *userName, char *hostName, int *dir_length);

void Copy(const char *small, char *big, int data_volume);

void GiveMoreSpace (char **small, int *data_volume);

void CopyTwoDim(char **small, char **big, int data_volume);

void GiveMoreTwoDimSpace (char ***small, int *data_volume);

int GetNewCommandWord(char **command, int *length, int *arg_length, int *ampersand, int *last_amp);

char **GetNewCommand(CInf *commands, int len, int *exit_symbol, int *last_amp);

Comms GetNewString();