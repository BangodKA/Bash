#include <unistd.h>

#include "pipeline.h"
#include "../0_general/structure.h"
#include "../2_second_step/process_command.h"

void ProcessPipeline(CPipe background_pipe, int fd_in, int fd_out) {
    int pipeline[background_pipe.length][2];
    pipe(pipeline[0]);
    pipeline[0][0] = dup(fd_out);
    int last_command_in_pipeline = 0;
    for (int k = 0; k < background_pipe.length; k++) {
        CInf temp_command_inf = background_pipe.pipe_comms[k];
        pipe(pipeline[k + 1]);
        if (k == background_pipe.length - 1) {
            pipeline[k + 1][1] = dup(fd_in);
            if (fd_in != 1) {
                close(fd_in);
            }
            close(pipeline[k + 1][0]);
            last_command_in_pipeline = 1;
        }
            ProcessCommand(temp_command_inf.command, pipeline[k + 1][1], pipeline[k][0], last_command_in_pipeline);
            close(pipeline[k][0]);
            close(pipeline[k + 1][1]);
    }
}