#ifndef WORKER_THREAD_H
#define WORKER_THREAD_H

#include "server_utils.hpp"

typedef struct {
    int blck_sz;
    int queue_sz;
}work_args; 

void sent_files(entry tmp, int block_sz);
void* worker_func(void* ptr_args);

#endif