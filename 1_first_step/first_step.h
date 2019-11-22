#pragma once

int GetHomeDirLen(char **homeDir);

int GetDir(char **dirName);

void CreateCommandPrompt(char **dirName, char *userName, char *hostName, int *dir_length);

int GetNewSymbol(int *back_sl, int *quote, int *double_quote, int *vert_slash);

int GetFileName(int c, CBack *full_command);

int GetNewCommandWord(CBack *full_command, int *length);

void GetNewCommand(CBack *full_command, int *exit_symbol);

void GetNewString(CBack *full_command, int *exit_symbol, int *last_amp);

CBack GetNewCommPipe();