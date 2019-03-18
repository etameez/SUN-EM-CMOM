#ifndef CBLACS_CONFIG_H
#define CBLACS_CONFIG_H

struct CBLACSConfig
{
    int context;
    int total_proc_rows;
    int total_proc_cols;
    int block_size;
    int proc_row;
    int proc_col;
};

#endif
