#pragma once

int DetectExit(CInf *commands, int length);

void GoHome();

int ChangeDir(char **command);

int FindStartPoint(int home_dir_length, int dir_length, char *dirName, char *homeDir);

const char * const NeedTwiddle(int start_point, const char * const nothing, const char * const twiddle);

void ProcessCommand(char **command, int ampersand, int *amp_amount);