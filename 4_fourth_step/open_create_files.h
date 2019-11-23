#pragma once

int Read(Arrs back_arrow);

int ProcessWriteFiles (Arrs arrow, int arrows_types);

int SpecifyThreads(Arrs *arrows, int *fd_out, int *fd_in, int which_arrow_last);