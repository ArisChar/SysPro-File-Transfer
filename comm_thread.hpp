#ifndef COMM_THREAD_H
#define COMM_THREAD_H

#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <sys/wait.h>	     /* sockets */
#include <sys/types.h>	     /* sockets */
#include <sys/socket.h>	     /* sockets */
#include <netinet/in.h>	     /* internet sockets */
#include <netdb.h>	         /* gethostbyaddr */
#include <unistd.h>	         /* fork */		
#include <stdlib.h>	         /* exit */
#include <ctype.h>	         /* toupper */
#include <signal.h>          /* signal */
#include <pthread.h>         /* threads */
#include <queue>
#include <dirent.h>
#include <experimental/filesystem>
#include <fstream>
#include <map>

#include "server_utils.hpp"

typedef struct {
    int socket;
    int blck_sz;
    int queue_sz;
}comm_args;

void insert_queue(string path, queue<entry> &oura, int sock, int queue_sz);
void* comm_func(void* ptr_args);

#endif