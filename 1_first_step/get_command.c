#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

#include "../0_general/structure.h"
#include "reallocs.h"

int DetectScreen(int c, int *back_sl, int *quote, int *double_quote) {
    if (c == '\\') {
        if (*back_sl == 1) {
            *back_sl = 0;
        }
        else {
            *back_sl = 1;
            return -2;
        }
    }

    if ((c == '"') && (!(*quote))) {
        *double_quote ^= 1;
        return -1;
    }

    if ((c == '\'') && (!(*double_quote))) {
        *quote ^= 1;
        return -1;
    }

    return c;
}

int ProcessScreen(int *c, int qoute) {
    if (*c == -1) {
        return 1;
    }
    if (*c == -2) {
        if (!qoute) {
            return 1;
        }
        else {
            *c = '\\';
        }
    }
    return 0;
}

int GetNextSymbol(int *c, int j, int *back_sl, int *qoute, int *double_quote, int arrow) {
    if (!arrow) {
        *c = getchar();
    }
    if (j == 0) {
        if ((*c == ' ') || (*c == '\t')) {
            *c = getchar();
        }
    }
    
    *c = DetectScreen(*c, back_sl, qoute, double_quote);

    if (ProcessScreen(c, *qoute)) {
        return -1;
    }
    return 0;
}

int ProcessSymbol(int c, int *back_sl, int qoute, int double_quote, char *command_arg, int *j) {
    if ((c != ' ' && c != '\n' && c != '\t' && c != '\r' && c != '&' && c != '|') || (double_quote == 1) || (qoute == 1)) {
        if ((c == '\n') && (*back_sl == 1) && (double_quote == 1)) {
            *back_sl = 0;
        }
        else {
            command_arg[*j] = c;
            (*j)++;
            *back_sl = 0;
        }
    }
    else {
        if ((c == '\n') && (*back_sl == 1)) {
            *back_sl = 0;
        }
        else {
            return -1;
        }
    }
    return 0;
}

void GetAllNeededSymbols(char **file_name, int *back_sl, int *qoute, int *double_quote, int *c, int *temp_size, int *j, int arrow) {
    while (1) {
        if (*j == *temp_size) {
            GiveMoreSpace(file_name, temp_size);
        }

        if((GetNextSymbol(c, *j, back_sl, qoute, double_quote, arrow)) == -1) {
            continue;
        }

        if ((ProcessSymbol(*c, back_sl, *qoute, *double_quote,  *file_name, j)) == -1) {
            break;
        }
        arrow = 0;
    }
}

void CollectWholeFileName(Arrs *arrows, int *c, int *back_sl, int *qoute, int *double_quote, int arrow) {
    int temp_size = 16;
    int j = 0;
    if ((*arrows).length == (*arrows).size) {
        GiveMoreTwoDimSpace(&(*arrows).file_name, &(*arrows).size);
    }
    (*arrows).file_name[(*arrows).length] = (char *)malloc(temp_size * sizeof(char *));

    GetAllNeededSymbols(&(*arrows).file_name[(*arrows).length], back_sl, qoute, double_quote, c, &temp_size, &j, arrow);
    
    (*arrows).file_name[(*arrows).length][j] = '\0';
    (*arrows).length += 1;
}

int GetFileName(int c, CBack *full_command, int *back_sl, int *quote, int *double_quote) {
    if (c == '<') {
        CollectWholeFileName(&(*full_command).arrows[0], &c, back_sl, quote, double_quote, 0);
    }
    if (c == '>') {
            c = getchar();
            if (c == '>') {
                (*full_command).which_arrow_last = 2;
                CollectWholeFileName(&(*full_command).arrows[2], &c, back_sl, quote, double_quote, 0);
            }
            else {
                (*full_command).which_arrow_last = 1;
                CollectWholeFileName(&(*full_command).arrows[1], &c, back_sl, quote, double_quote, 1);
            }
        }

    return c;
}

int GetNewCommandWord(CBack *full_command, int *length) {
    int c;
    CInf *pipe_comms = (*full_command).background_pipes[(*full_command).length].pipe_comms;
    int len = (*full_command).background_pipes[(*full_command).length].length;
    char **command = pipe_comms[len].command;
    int word_size = 16;
    command[*length] = (char*)malloc(word_size * sizeof(char));
    int j = 0;
    int back_sl = 0;
    int double_quote = 0;
    int qoute = 0;
    while(1) {
        if (j == word_size) {
            GiveMoreSpace(&command[*length], &word_size);
        }

        if((GetNextSymbol(&c, j, &back_sl, &qoute, &double_quote, 0)) == -1) {
            continue;
        }

        c = GetFileName(c, full_command, &back_sl, &qoute, &double_quote);

        if ((ProcessSymbol(c, &back_sl, qoute, double_quote, command[*length], &j)) == -1) {
            break;
        }
    }

    if (j != 0) {
        (*full_command).not_blank = 1;
        command[*length][j] = '\0';
        (*length)++;
    }

    return c;
}

void GetNewCommand(CBack *full_command, int *exit_symbol) {
    int comm_size = 16;
    CInf *pipe_comms = (*full_command).background_pipes[(*full_command).length].pipe_comms;
    int len = (*full_command).background_pipes[(*full_command).length].length;
    pipe_comms[len].command = (char **)malloc(comm_size * sizeof(char *));
    int comm_length = 0;
    while(1) {
        *exit_symbol = GetNewCommandWord(full_command, &comm_length);
        if ((*exit_symbol == '\n') || (*exit_symbol == '&') || (*exit_symbol == '|')) {
            break;
        }
        
        if (comm_length == comm_size) {
            GiveMoreTwoDimSpace(&pipe_comms[len].command, &comm_size);
        }
    }

    
    if (comm_length == comm_size) {
        GiveMoreTwoDimSpace(&pipe_comms[len].command, &comm_size);
    }

    pipe_comms[len].command[comm_length] = (char*)malloc(10 * sizeof(char));
    pipe_comms[len].command[comm_length] = NULL;                                    

    pipe_comms[len].length = comm_length;
}

int DetectPipeEnd(int exit_symbol, int conv_end, int *ampersand) {
    if (((exit_symbol == '\n') && (!conv_end)) || (exit_symbol == '&')) {
        if (exit_symbol == '&') {
            *ampersand = 1;
        }
        return -1;
    }
    return 0;
}

void GetNewString(CBack *full_command, int *exit_symbol, int *last_amp) {
    int size = 16;
    int background_length = (*full_command).length;
    CPipe *last_pipe_inf = &(*full_command).background_pipes[background_length];
    (*last_pipe_inf).pipe_comms = (CInf *)malloc(size * sizeof(CInf));
    (*last_pipe_inf).length = 0;
    (*last_pipe_inf).ampersand = 0;    
    int conv_end = 0;
    while(1) {

        if (DetectPipeEnd(*exit_symbol, conv_end, &(*last_pipe_inf).ampersand) == -1) {
            break;
        }        

        if ((*exit_symbol == '\n') && (conv_end)) {
            (*last_pipe_inf).length--;
        }

        if ((*last_pipe_inf).length == size) {
            GiveMore3DimSpace(&(*last_pipe_inf).pipe_comms, &size);
        }

        GetNewCommand(full_command, exit_symbol);
        
        if ((*last_pipe_inf).pipe_comms[(*last_pipe_inf).length].length != 0) {
            conv_end = 0;
            *last_amp = 0;
        }
        
        (*last_pipe_inf).length++;

        if (*exit_symbol == '|') {
            conv_end = 1;
            (*full_command).last_is_pipeline = 1;
        }
    }
}

CBack GetNewCommPipe() {
    CBack full_command;
    int size = 16;
    full_command.background_pipes = (CPipe *)malloc(size * sizeof(CPipe));
    full_command.not_blank = 0;
    full_command.length = 0;
    full_command.last_is_pipeline = 0;
    full_command.last_is_background = 0;
    full_command.which_arrow_last = -1;
    full_command.arrows[0].size = 16;
    full_command.arrows[1].size = 16;
    full_command.arrows[2].size = 16;
    full_command.arrows[0].length = 0;
    full_command.arrows[1].length = 0;
    full_command.arrows[2].length = 0;
    full_command.arrows[0].file_name = (char **)malloc(full_command.arrows[0].size * sizeof(char *));
    full_command.arrows[1].file_name = (char **)malloc(full_command.arrows[1].size * sizeof(char *));
    full_command.arrows[2].file_name = (char **)malloc(full_command.arrows[2].size * sizeof(char *));
    int exit_symbol = '\0';
    int last_amp = 0;
    while(exit_symbol != '\n') {
        
        if (full_command.length == size) {
            GiveMore4DimSpace(&full_command.background_pipes, &size);
        }
        exit_symbol = '\0';
        GetNewString(&full_command, &exit_symbol, &last_amp);

        if (exit_symbol == '&') {
            full_command.last_is_pipeline = 0;
            last_amp = 1;
        }

        if (!((exit_symbol == '\n') && (last_amp))) {
            full_command.length++;
        }

        else {
            full_command.last_is_background = 1;
        }
    }

    return full_command;
}