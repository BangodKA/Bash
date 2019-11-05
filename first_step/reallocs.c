#define _GNU_SOURCE
#include <limits.h> // длины пути, хоста и имени пользователя
#include <stdio.h> 
#include <stdlib.h> // Для определения домашней директории
#include <string.h>
#include <unistd.h>
#include "../general/general.h"

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

void Copy4Dim(Comms *small, Comms *big, int data_volume) { // Копирует двумерный массив из одной области память в другую двумерный массив
    for(int i = 0; i < data_volume; i++) {
        big[i] = small[i];
    }
}

void GiveMore4DimSpace (Comms **small, int *data_volume) { // Выделяет в два раза больше места под массив строк
    int temp = *data_volume;
    *data_volume *= 2;
    Comms *big = (Comms *)malloc(*data_volume * sizeof(Comms));
    Copy4Dim(*small, big, temp);
    free(*small);
    *small = big;
}