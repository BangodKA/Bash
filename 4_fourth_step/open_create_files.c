#include <unistd.h> // Для использования команд bash
#include <stdio.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "open_create_files.h"
#include "../0_general/structure.h"

#define WriteFlags(first, second, third) \
    int open_res = 0; \
    for (int v = 0; v < arrow.length - 1; v++) { \
        open_res = open(arrow.file_name[v], first | second | third, S_IRWXU); \
        if (open_res != -1) { \
            close(open_res); \
        } \
    }\
    return open(arrow.file_name[arrow.length - 1], first | second | third, S_IRWXU); 

int ProcessWriteFiles (Arrs arrow, int arrows_types) {
    if (arrows_types == 1) {
        WriteFlags(O_WRONLY, O_TRUNC, O_CREAT)
    }
    else {
        WriteFlags(O_WRONLY, O_APPEND, O_CREAT)
    }
    
}

int Read(Arrs back_arrow) {
    int open_res = 0;
    for (int v = 0; v < back_arrow.length - 1; v++) {
        open_res = open(back_arrow.file_name[v], O_RDONLY, S_IRWXU);
        if (open_res != -1) {
            close(open_res);
        }
        else {
            perror(back_arrow.file_name[v]);
            return -2;
        }
    }
    if (back_arrow.length == 0) {
        return 0;
    }
    else {
        return open(back_arrow.file_name[back_arrow.length - 1], O_RDONLY, S_IRWXU);
    }
}

int SpecifyThreads(Arrs *arrows, int *fd_out, int *fd_in, int which_arrow_last) {
    if ((*fd_out = Read(arrows[0])) < 0) {
        if (*fd_out == -1) {
            perror(arrows[0].file_name[arrows[0].length - 1]);
        }
        return -1;
    }
    if (which_arrow_last == 1) {
        *fd_in = ProcessWriteFiles(arrows[1], 1);
        close(ProcessWriteFiles(arrows[2], 2));
    }
    else if(which_arrow_last == 2) {
        *fd_in = ProcessWriteFiles(arrows[2], 2);
        close(ProcessWriteFiles(arrows[1], 1));
    }

    if (*fd_in == 0) {
        *fd_in = dup(1);
    }

    if (*fd_out < 0) {
        *fd_out = dup(0);
    }
    return 0;
}