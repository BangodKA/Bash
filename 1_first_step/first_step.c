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

int GetNewSymbol(int *back_sl, int *quote, int *double_quote, int *vert_slash) {
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

    if (c == '|') {
        /*if (j == 0) {
            while(getchar() != '\n'); // cчитываем лишний ввод
            return 1;
        }*/
        *vert_slash = 1;
    }

    if (c == '<') {
        return 0;
    }

    if (c == '>') {
        return 2;
    }
    return c;
}

int GetNewCommandWord(CBack *full_command, int *length) { // Добавляет в массив команды новое слово, возвращая последний символ
    int c;
    CInf *pipe_comms = (*full_command).background_pipes[(*full_command).length].pipe_comms;
    int len = (*full_command).background_pipes[(*full_command).length].length;
    char **command = pipe_comms[len].command;
    int *arg_length = pipe_comms[len].arg_length;
    int *pipeline = &pipe_comms[len].vert_slash;
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

        c = GetNewSymbol(&back_sl, &qoute, &double_quote, pipeline);

        

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
        if (c == 0) {
            while (((c = getchar()) == ' ') || c == '\t');
            int temp_size = 16;
            int i = 0;
            if ((*full_command).back_arrow.length == (*full_command).back_arrow.size) {
                GiveMoreTwoDimSpace(&(*full_command).back_arrow.file_name, &(*full_command).back_arrow.size);
            }
            (*full_command).back_arrow.file_name[(*full_command).back_arrow.length] = (char *)malloc(temp_size * sizeof(char *));
            while ((c != ' ' && c != '\n' && c != '\t' && c != '\r' && c != '&' && c != '|')) {
                (*full_command).back_arrow.file_name[(*full_command).back_arrow.length][i] = c;
                i++;
                if (i == temp_size) {
                    GiveMoreSpace(&(*full_command).back_arrow.file_name[(*full_command).back_arrow.length], &temp_size);
                }
                c = getchar();
            }
            (*full_command).back_arrow.file_name[(*full_command).back_arrow.length][i] = '\0';
            (*full_command).back_arrow.length += 1;
            printf("%d\n", (*full_command).back_arrow.length);
        }

        if (c == 2) {
            c = getchar();
            if (c == '>') {
                while (((c = getchar()) == ' ') || c == '\t');
                int temp_size = 16;
                int i = 0;
                if ((*full_command).double_arrow.length == (*full_command).double_arrow.size) {
                    GiveMoreTwoDimSpace(&(*full_command).double_arrow.file_name, &(*full_command).double_arrow.size);
                }
                (*full_command).double_arrow.file_name[(*full_command).double_arrow.length] = (char *)malloc(temp_size * sizeof(char *));
                while ((c != ' ' && c != '\n' && c != '\t' && c != '\r' && c != '&' && c != '|')) {
                    (*full_command).double_arrow.file_name[(*full_command).double_arrow.length][i] = c;
                    i++;
                    if (i == temp_size) {
                        GiveMoreSpace(&(*full_command).double_arrow.file_name[(*full_command).double_arrow.length], &temp_size);
                    }
                    c = getchar();
                }
                (*full_command).double_arrow.file_name[(*full_command).double_arrow.length][i] = '\0';
                (*full_command).double_arrow.length += 1;
            }
            else {
                while ((c == ' ') || (c == '\t')) {
                    c = getchar();
                }
                int temp_size = 16;
                int i = 0;
                if ((*full_command).arrow.length == (*full_command).arrow.size) {
                    GiveMoreTwoDimSpace(&(*full_command).arrow.file_name, &(*full_command).arrow.size);
                }
                (*full_command).arrow.file_name[(*full_command).arrow.length] = (char *)malloc(temp_size * sizeof(char *));
                while ((c != ' ' && c != '\n' && c != '\t' && c != '\r' && c != '&' && c != '|')) {
                    (*full_command).arrow.file_name[(*full_command).arrow.length][i] = c;
                    i++;
                    if (i == temp_size) {
                        GiveMoreSpace(&(*full_command).arrow.file_name[(*full_command).arrow.length], &temp_size);
                    }
                    c = getchar();
                }
                (*full_command).arrow.file_name[(*full_command).arrow.length][i] = '\0';
                (*full_command).arrow.length += 1;
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
            /*if ((c == '\n') && (*last_amp == 1)) {
                c = -1;
            }
            else {
                (*last_amp) = 0;
            }*/
            break;
        }
    } while(1);

    if (j != 0) { // Окончание строки, появилось новое слово длины j
        (*full_command).not_blank = 1;
        command[*length][j] = '\0';
        arg_length[*length] = j;
        (*length)++;
    }

    return c;
}

void GetNewCommand(CBack *full_command, int *exit_symbol) {
    int comm_size = 16;
    CInf *pipe_comms = (*full_command).background_pipes[(*full_command).length].pipe_comms;
    int len = (*full_command).background_pipes[(*full_command).length].length;
    pipe_comms[len].command = (char **)malloc(comm_size * sizeof(char *));
    pipe_comms[len].arg_length = (int *)malloc(comm_size * sizeof(int));
    pipe_comms[len].vert_slash = 0;
    int comm_length = 0;
    while(1) { // Последний прочитанный мог быть пробелом, тогда получаем еще слова
        *exit_symbol = GetNewCommandWord(full_command, &comm_length);
        if ((*exit_symbol == '\n') || (*exit_symbol == '&') || (*exit_symbol == 1) || (*exit_symbol == -1) || (*exit_symbol == '|')) {
            break;
        }
        
        if (comm_length == comm_size) {
            GiveMoreTwoDimSpace(&pipe_comms[len].command, &comm_size);
        }
    }

    
    if (comm_length == comm_size) {
        GiveMoreTwoDimSpace(&pipe_comms[len].command, &comm_size);
    }

    pipe_comms[len].command[comm_length] = (char*)malloc(10 * sizeof(char)); // Маркер завершения команды
    pipe_comms[len].command[comm_length] = NULL;                                    // для execvp

    pipe_comms[len].comm_length = comm_length;
}

void GetNewString(CBack *full_command, int *exit_symbol, int *last_amp) {
    int size = 16;
    CPipe *background_pipes = (*full_command).background_pipes;
    int background_length = (*full_command).length;
    background_pipes[background_length].pipe_comms = (CInf *)malloc(size * sizeof(CInf));
    background_pipes[background_length].length = 0;
    int conv_end = 0;
    while(1) {

        if (((*exit_symbol == '\n') && (!conv_end)) || (*exit_symbol == '&')) {
            break;
        }

        if (background_pipes[background_length].length == size) {
            GiveMore3DimSpace(&background_pipes[background_length].pipe_comms, &size);
        }

        int len = background_pipes[background_length].length;

        GetNewCommand(full_command, exit_symbol);


        if (background_pipes[background_length].pipe_comms[len].comm_length != 0) {
            conv_end = 0;
            *last_amp = 0;
        }

        /*if (exit_symbol == 1) {
            object.length = -1;
            break;
        }*/

        /*if (*exit_symbol == -1) {
            break;
        }*/
        
        background_pipes[background_length].length++;

        if (*exit_symbol == '|') {
            conv_end = 1;
            if (!(*full_command).last_is_pipeline) {
                (*full_command).last_is_pipeline = 1;
            }
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
    full_command.double_arrow.size = 16;
    full_command.back_arrow.size = 16;
    full_command.arrow.size = 16;
    full_command.double_arrow.length = 0;
    full_command.back_arrow.length = 0;
    full_command.arrow.length = 0;
    full_command.double_arrow.file_name = (char **)malloc(full_command.double_arrow.size * sizeof(char *));
    full_command.arrow.file_name = (char **)malloc(full_command.arrow.size * sizeof(char *));
    full_command.back_arrow.file_name = (char **)malloc(full_command.back_arrow.size * sizeof(char *));
    int exit_symbol = '\0';
    int last_amp = 0;
    while(exit_symbol != '\n') {
        if (full_command.length == size) {
            GiveMore4DimSpace(&full_command.background_pipes, &size);
        }
        exit_symbol = '\0';
        GetNewString(&full_command, &exit_symbol, &last_amp);

        if (exit_symbol == -1) {
            full_command.length++;
            break;
        }

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