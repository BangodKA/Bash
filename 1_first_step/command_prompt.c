#define _GNU_SOURCE
#include <limits.h> // длины пути, хоста и имени пользователя
#include <stdio.h> 
#include <stdlib.h> // Для определения домашней директории
#include <unistd.h>


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