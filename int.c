#define RESET   "\033[0m"
#define DIR   "\033[32;22;1m"
#define HOST  "\033[34;22;1m"

#define PINK  "\033[35;1m"
#define YELLOW "\033[33;1m"


#define _GNU_SOURCE // Для использования констант максимальных значений 
#include <limits.h> // длины пути, хоста и имени пользователя
#include <unistd.h> // Для использования команд bash
#include <stdio.h> 
#include <stdlib.h> // Для определения домашней директории
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <string.h>

// Команда, память для нее, кол-во слов в команде, массив кол-ва символов в каждом слове команды
typedef struct CommInf{
    char **command;
    int size;
    int length;
    int *arg_length;
    int *ampersand;
}CInf;


// ПЕРВЫЙ ЭТАП


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

int GetNewCommandWord(char **command, int *length, int word_size, int *arg_length, int *ampersand) { // Добавляет в массив команды новое слово, возвращая последний символ
    int c;
    command[*length] = (char*)malloc(word_size * sizeof(char));
    arg_length[*length] = 0;
    ampersand[*length] = 0;
    int j = 0;
    do {
        if (j == word_size) {
            GiveMoreSpace(&command[*length], &word_size);
        }

        c = getchar();

        if (c == '&') {
            if ((*length == 0) && (j == 0)) {
                while(getchar() != '\n'); // cчитываем лишний ввод
                return c;
            }

            break;
        }

        if (c != ' ' && c != '\n' && c != '\t' && c != '\r') { // Новый символ текущего слова команды
            command[*length][j] = c;
            j++;
        }
        else {
            break;
        }
    } while(1);

    if (j != 0) { // Окончание строки, появилось новое слово длины j
        ampersand[*length] = 1;
        command[*length][j] = '\0';
        arg_length[*length] = j;
        (*length)++;
    }

    else {
        ampersand[*length - 1] = 1;
    }

    return c;
}

char** GetNewCommand(int *size, int *length, int **arg_length, int *ampersand) {
    char **command = (char **)malloc(*size * sizeof(char *));
    *ampersand = (int *)malloc(*size * sizeof(int));
    *arg_length = (int *)malloc(*size * sizeof(int));
    *length = 0;
    int word_size = 10;
    int temp;
    while(1) { // Последний прочитанный мог быть пробелом, тогда получаем еще слова
        temp = GetNewCommandWord(command, length, word_size, *arg_length, *ampersand);
        if (temp == '\n') {
            break;
        }

        if (temp == '&') {
            return NULL;
        }
        
        if (*length == *size) {
            GiveMoreTwoDimSpace(&command, size);
        }
    }

    
    if (*length == *size) {
        GiveMoreTwoDimSpace(&command, size);
    }

    command[*length] = (char*)malloc(word_size * sizeof( char)); // Маркер завершения команды
    command[*length] = NULL;                                    // для execvp

    return command;

}


// ВТОРОЙ ЭТАП

int DetectExit(char **command, int length) { // Обнаруживает exit
    if (length > 0) {
        if (strcmp(command[0], "exit") == 0) {
            return 1;
        }
    }
    return 0;
}

void GoHome() {
    char *tempHome = NULL;
    tempHome = getenv("HOME");
    chdir(tempHome);
}

int ChangeDir(char **command) {
    if (command[1] != NULL) {
        if (!strncmp(command[1], "~", 1)) {
            GoHome();
            if (command[1][1] == '\0') {
                return 1;
            }
            strcpy(command[1], (&command[1][2]));
        }
        if (chdir(command[1]) == -1) {
            printf("bash: cd: %s: Нет такого файла или каталога\n", command[1]);
            return 0;
        }
        return 1;
    }

    return 0;
}

int FindStartPoint(int home_dir_length, int dir_length, char *dirName, char *homeDir) {
    if (home_dir_length > dir_length) {
        return 0;
    }
    
    if (!strncmp(dirName, homeDir, 5)) {
        return home_dir_length;
    }
    return 0;
}

const char * const NeedTwiddle(int start_point, const char * const nothing, const char * const twiddle) {
    if (start_point == 0) {
        return nothing;
    }
    else {
        return twiddle;
    }
}

void ProcessCommand(char **command) { // Выполнение команды API
    pid_t child;
    child = fork();
    if (child  < 0) {
        printf("%s\n", "Something went wrong");
    }
    else if (child == 0) {
        execvp(command[0], command); // Исполнение команды bash                                                                                                                                                                
        printf("%s: команда не найдена\n", command[0]);
        exit(EXIT_FAILURE);
    }
    else {
        wait(NULL);
    }
}

void FreeHeap(char **command, int length) { // Освобождает от команды
    for(int i = 0; i <= length; i++) {
        free(command[i]);
    }
    free(command);
}

int main ()
{    
    char *dirName = NULL; // Указатель на область, где будет храниться путь к текущей директории
    int dir_length = 0; // Длина до рабочей директории
    char *homeDir = NULL; // Указатель на область, где будет храниться домашняя директория
    int home_dir_length = 0;
    char userName[LOGIN_NAME_MAX + 1]; // Указатель на область, где будет храниться имя пользователя
    char hostName[HOST_NAME_MAX + 1]; // Массив, где будет храниться имя хоста
    CInf  object;
    object.size = 200;
    object.length = 0;
    //object.command[0] = NULL;

    CreateCommandPrompt(&dirName, userName, hostName, &dir_length);

    home_dir_length = GetHomeDirLen(&homeDir);

    int start_point = home_dir_length;

    const char * const twiddle = "~";
    const char * const nothing = "";

    const char * invitation = twiddle;

    while(1) {
        printf ("%s%s@%s%s:%s%s%s%s$ ", PINK, userName, hostName, RESET, YELLOW, invitation, &dirName[start_point], RESET);

        object.command = GetNewCommand(&object.size, &object.length, &object.arg_length, object.ampersand);

        if (object.command == NULL) {
            printf("bash: синтаксическая ошибка рядом с неожиданным маркером «&»\n");
            continue;
        }

        if(DetectExit(object.command, object.length)) {
            break;
        }

        if (object.length >= 1) {
            if (!strcmp(object.command[0], "cd")) {

                if (object.length > 2) {
                    printf("bash: cd: слишком много аргументов\n");
                }
                else if(ChangeDir(object.command)) {
                    dir_length = GetDir(&dirName);

                    start_point = FindStartPoint(home_dir_length, dir_length, dirName, homeDir);

                    invitation = NeedTwiddle(start_point, nothing, twiddle);
                }
            }
            else {
                ProcessCommand(object.command);
            }
        }

        /*for(int i = 0; i < object.length; i++) {
            for(int j = 0; j < object.arg_length[i]; j++) {
                printf("%c", object.command[i][j]);
            }
        }*/

        FreeHeap(object.command, object.length); // Освобождаем место для новой команды
    }


    FreeHeap(object.command, object.length); // Освобождаем место для новой команды


    return 0;
}