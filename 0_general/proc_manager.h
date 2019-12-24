#pragma once

void PrintBackgroundZombie(CPipe process_pipe, pid_t child);

void FreeBackgroundZombie(CPipe *process_pipe, int length, int index);

void WaitBackgroundZombies(int *amp_amount, BZombies *process_manager);

void RememberBackgroundZombie(CPipe *process_pipe, CPipe background_pipe);