#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H

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

using namespace std;
namespace fs = std::experimental::filesystem;

extern map<int, pthread_mutex_t> mutex_map;

extern pthread_mutex_t mtx;
extern pthread_cond_t cond_queue_full;
extern pthread_cond_t cond_queue_empty;
extern pthread_mutex_t print; 

typedef struct entry{
    int sock;
    string file_path;

    entry(int s, string fp): sock(s), file_path(fp) { }
}entry;

extern queue<entry> oura;

void read_args(int argc, char* argv[], int& port, int& threadp_sz, int& queue_size, int& block_sz);
void perror_exit(string message);
int scan_dir(string path);

#endif