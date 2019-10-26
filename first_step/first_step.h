#pragma once

typedef struct CommInf{
    char **command;
    int size;
    int comm_length;
    int *arg_length;
    int ampersand;
}CInf;

typedef struct Commands{
    CInf *commands;
    int length;
}Comms;

int GetHomeDirLen(char **homeDir);

int GetDir(char **dirName);

void CreateCommandPrompt(char **dirName, char *userName, char *hostName, int *dir_length);

void Copy(const char *small, char *big, int data_volume);

void GiveMoreSpace (char **small, int *data_volume);

void CopyTwoDim(char **small, char **big, int data_volume);

void GiveMoreTwoDimSpace (char ***small, int *data_volume);

int GetNewCommandWord(char **command, int *length, int *arg_length, int *ampersand);

char **GetNewCommand(CInf *commands, int len, int *exit_symbol);

Comms GetNewString();