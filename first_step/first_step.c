#define _GNU_SOURCE
#include <limits.h> // длины пути, хоста и имени пользователя
#include <stdio.h> 
#include <stdlib.h> // Для определения домашней директории
#include <string.h>
#include <unistd.h>



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

void Copy(const char *small, char *big, int data_volume) { // Копирует из одной области память в другую
    for(int i = 0; i < data_volume; i++) {
        big[i] = small[i];
    }
}

void GiveMoreSpace (char **small, int *data_volume) { // Выделяет в два раза больше места под массив
    int temp = *data_volume;
    *data_volume *= 2;
    char* big = (char *)malloc(*data_volume * sizeof(char));
    Copy(*small, big, temp);
    free(*small);
    *small = big;
}

void CopyTwoDim(char **small, char **big, int data_volume) { // Копирует двумерный массив из одной области память в другую двумерный массив
    for(int i = 0; i < data_volume; i++) {
        big[i] = small[i];
    }
}

void GiveMoreTwoDimSpace (char ***small, int *data_volume) { // Выделяет в два раза больше места под массив строк
    int temp = *data_volume;
    *data_volume *= 2;
    char **big = (char **)malloc(*data_volume * sizeof(char *));
    CopyTwoDim(*small, big, temp);
    free(*small);
    *small = big;
}

void Copy3Dim(CInf *small, CInf *big, int data_volume) { // Копирует двумерный массив из одной области память в другую двумерный массив
    for(int i = 0; i < data_volume; i++) {
        big[i] = small[i];
    }
}

void GiveMore3DimSpace (CInf **small, int *data_volume) { // Выделяет в два раза больше места под массив строк
    int temp = *data_volume;
    *data_volume *= 2;
    CInf *big = (CInf *)malloc(*data_volume * sizeof(CInf));
    Copy3Dim(*small, big, temp);
    free(*small);
    *small = big;
}

int GetNewCommandWord(char **command, int *length, int *arg_length, int *ampersand, int *last_amp) { // Добавляет в массив команды новое слово, возвращая последний символ
    int c;
    int word_size = 16;
    command[*length] = (char*)malloc(word_size * sizeof(char));
    arg_length[*length] = 0;
    int j = 0;
    int back_sl = 0;
    do {
        if (j == word_size) {
            GiveMoreSpace(&command[*length], &word_size);
        }

        c = getchar();

        if (c == '\\') {
            back_sl = 1;
            continue;
        }

        if (c == '&') {
            if ((*length == 0) && (j == 0)) {
                while(getchar() != '\n'); // cчитываем лишний ввод
                return 1;
            }
            *ampersand = 1;
            break;
        }

        if (c != ' ' && c != '\n' && c != '\t' && c != '\r') { // Новый символ текущего слова команды
            command[*length][j] = c;
            j++;
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

char **GetNewCommand(CInf *commands, int len, int *exit_symbol, int *last_amp) {
    int comm_size = 16;
    commands[len].command = (char **)malloc(comm_size * sizeof(char *));
    commands[len].arg_length = (int *)malloc(comm_size * sizeof(int));
    commands[len].ampersand = 0;
    int comm_length = 0;
    while(1) { // Последний прочитанный мог быть пробелом, тогда получаем еще слова
        *exit_symbol = GetNewCommandWord(commands[len].command, &comm_length, commands[len].arg_length, &commands[len].ampersand, last_amp);
        if ((*exit_symbol == '\n') || (*exit_symbol == '&') || (*exit_symbol == 1) || (*exit_symbol == -1)) {
            break;
        }
        
        if (comm_length == comm_size) {
            GiveMoreTwoDimSpace(&commands[len].command, &comm_size);
        }
    }

    
    if (comm_length == comm_size) {
        GiveMoreTwoDimSpace(&commands[len].command, &comm_size);
    }

    commands[len].command[comm_length] = (char*)malloc(10 * sizeof(char)); // Маркер завершения команды
    commands[len].command[comm_length] = NULL;                                    // для execvp

    commands[len].comm_length = comm_length;

    return commands[len].command;
}

Comms GetNewString() {
    Comms object;
    int size = 16;
    object.commands = (CInf *)malloc(size * sizeof(CInf));
    object.length = 0;
    int exit_symbol = '\0';
    int last_amp = 0;
    while(exit_symbol != '\n') {

        if (object.length == size) {
            GiveMore3DimSpace(&object.commands, &size);
        }

        GetNewCommand(object.commands, object.length, &exit_symbol, &last_amp);

        if (exit_symbol == 1) {
            object.length = -1;
            break;
        }

        if (exit_symbol == -1) {
            break;
        }
        
        object.length++;

        if (exit_symbol == '&') {
            last_amp = 1;
        }
    }

    return object;

}