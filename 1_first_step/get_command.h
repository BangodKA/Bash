#pragma once

int DetectScreen(int c, int *back_sl, int *quote, int *double_quote);

int ProcessScreen(int *c, int qoute);

int GetNextSymbol(int j, int *back_sl, int *qoute, int *double_quote, int arrow);

int ProcessSymbol(int c, int *back_sl, int qoute, int double_quote, char *command_arg, int *j);

void GetAllNeededSymbols(char **file_name, int *back_sl, int *qoute, int *double_quote, int *c, int *temp_size, int *j, int arrow);

void CollectWholeFileName(Arrs *arrows, int *c, int *back_sl, int *qoute, int *double_quote, int arrow);

int GetFileName(int c, CBack *full_command, int *back_sl, int *quote, int *double_quote);

int GetNewCommandWord(CBack *full_command, int *length);

void GetNewCommand(CBack *full_command, int *exit_symbol);

int DetectPipeEnd(int exit_symbol, int conv_end, int *ampersand);

void GetNewString(CBack *full_command, int *exit_symbol, int *last_amp);

CBack GetNewCommPipe();