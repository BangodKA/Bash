#define _GNU_SOURCE
#include <limits.h> // длины пути, хоста и имени пользователя
#include <stdio.h> 
#include <stdlib.h> // Для определения домашней директории
#include <string.h>
#include <unistd.h>
#include "reallocs.h"


int GetHomeDirLen(char **homeDir) {
    *homeDir = getenv("HOME"); // Определяем домашнюю директорию

    int i = 0;

    while ((*homeDir)[i] != '\0') { 
        i++;
    }

    return i;
}

int GetDir(char **dirName) { // Задает путь для приглашения ко вводу, возвращает длину домашней директории
    free(*dirName);
    *dirName = (char *)malloc((PATH_MAX + 1) * sizeof(char));
    getcwd(*dirName, PATH_MAX); // Определяем путь к текущей директории 

    int dir_length = 0;
    while((*dirName)[dir_length] != '\0') { // Ищем конец для ChangeDir
        dir_length++; 
    }
    return dir_length;
}

void CreateCommandPrompt(char **dirName, char *userName, char *hostName, int *dir_length) { // Приглашение ко вводу
    GetDir(dirName);

    while((*dirName)[*dir_length] != '\0') { 
        (*dir_length)++; 
    }

    getlogin_r(userName, LOGIN_NAME_MAX); 

    gethostname(hostName, HOST_NAME_MAX); 
} 

int GetNewSymbol(int *back_sl, int *quote, int *double_quote, int *ampersand) {
    int c = getchar();
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

    if (c == '&') {
        /*if (j == 0) {
            while(getchar() != '\n'); // cчитываем лишний ввод
            return 1;
        }*/
        *ampersand = 1;
    }
    return c;
}

int GetNewCommandWord(char **command, int *length, int *arg_length, int *ampersand, int *last_amp) { // Добавляет в массив команды новое слово, возвращая последний символ
    int c;
    int word_size = 16;
    command[*length] = (char*)malloc(word_size * sizeof(char));
    arg_length[*length] = 0;
    int j = 0;
    int back_sl = 0;
    int double_quote = 0;
    int qoute = 0;
    do {
        if (j == word_size) {
            GiveMoreSpace(&command[*length], &word_size);
        }

        c = GetNewSymbol(&back_sl, &qoute, &double_quote, ampersand);

        if (c == -1) {
            continue;
        }

        if (c == -2) {
            if (!qoute) {
                continue;
            }
            else
            {
                c = '\\';
            }
            
        }

        /*if (c == 1) {
            return 1;
        }*/

        if ((c != ' ' && c != '\n' && c != '\t' && c != '\r' && c != '&' && c != '|') || (double_quote == 1) || (qoute == 1)) { // Новый символ текущего слова команды
            if ((c == '\n') && (back_sl == 1) && (double_quote == 1)) {
                back_sl = 0;
                continue;
            }
            command[*length][j] = c;
            j++;
            back_sl = 0;
        }
        else {
            if ((c == '\n') && (back_sl == 1)) {
                back_sl = 0;
                continue;
            }
            if ((c == '\n') && (*last_amp == 1)) {
                c = -1;
            }
            else {
                (*last_amp) = 0;
            }
            break;
        }
    } while(1);

    if (j != 0) { // Окончание строки, появилось новое слово длины j
        command[*length][j] = '\0';
        arg_length[*length] = j;
        (*length)++;
    }

    return c;
}

void GetNewCommand(CInf *background_comms, int len, int *exit_symbol, int *last_amp) {
    int comm_size = 16;
    background_comms[len].command = (char **)malloc(comm_size * sizeof(char *));
    background_comms[len].arg_length = (int *)malloc(comm_size * sizeof(int));
    background_comms[len].ampersand = 0;
    int comm_length = 0;
    while(1) { // Последний прочитанный мог быть пробелом, тогда получаем еще слова
        *exit_symbol = GetNewCommandWord(background_comms[len].command, &comm_length, background_comms[len].arg_length, &background_comms[len].ampersand, last_amp);
        if ((*exit_symbol == '\n') || (*exit_symbol == '&') || (*exit_symbol == 1) || (*exit_symbol == -1) || (*exit_symbol == '|')) {
            break;
        }
        
        if (comm_length == comm_size) {
            GiveMoreTwoDimSpace(&background_comms[len].command, &comm_size);
        }
    }

    
    if (comm_length == comm_size) {
        GiveMoreTwoDimSpace(&background_comms[len].command, &comm_size);
    }

    background_comms[len].command[comm_length] = (char*)malloc(10 * sizeof(char)); // Маркер завершения команды
    background_comms[len].command[comm_length] = NULL;                                    // для execvp

    background_comms[len].comm_length = comm_length;
}

void GetNewString(Comms *comms_pipe, int pipe_length, int *exit_symbol) {
    int size = 16;
    comms_pipe[pipe_length].background_comms = (CInf *)malloc(size * sizeof(CInf));
    comms_pipe[pipe_length].length = 0;
    int last_amp = 0;
    while((*exit_symbol != '\n') && (*exit_symbol != '|')) {

        if (comms_pipe[pipe_length].length == size) {
            GiveMore3DimSpace(&comms_pipe[pipe_length].background_comms, &size);
        }

        GetNewCommand(comms_pipe[pipe_length].background_comms, comms_pipe[pipe_length].length, exit_symbol, &last_amp);

        /*if (exit_symbol == 1) {
            object.length = -1;
            break;
        }*/

        if (*exit_symbol == -1) {
            break;
        }
        
        comms_pipe[pipe_length].length++;

        if (*exit_symbol == '&') {
            last_amp = 1;
        }
    }
}

CPipe GetNewCommPipe() {
    CPipe full_command;
    int size = 16;
    full_command.comm_pipes = (Comms *)malloc(size * sizeof(Comms));
    full_command.length = 0;
    full_command.is_pipeline = 0;
    int exit_symbol = '\0';
    while(exit_symbol != '\n') {

        if (full_command.length == size) {
            GiveMore4DimSpace(&full_command.comm_pipes, &size);
        }
        exit_symbol = '\0';
        GetNewString(full_command.comm_pipes, full_command.length, &exit_symbol);

        if (exit_symbol == -1) {
            full_command.length++;
            break;
        }

        if ((!full_command.is_pipeline) && (exit_symbol == '|')) {
            full_command.is_pipeline = 1;
        }
        
        full_command.length++;
    }

    return full_command;
}