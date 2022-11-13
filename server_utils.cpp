#include "server_utils.hpp"

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

map<int, pthread_mutex_t> mutex_map;

pthread_mutex_t mtx;
pthread_cond_t cond_queue_full;
pthread_cond_t cond_queue_empty;
pthread_mutex_t print; 

queue<entry> oura;

void read_args(int argc, char* argv[], int& port, int& threadp_sz, int& queue_size, int& block_sz){

    if(argc != 9){
        cout<<"Not enough input aruments!\n"
            <<"Input example: \n"
            <<"\t./dataServer -p <port> -s <thread_pool_size> -q <queue_size> -b <block_size>"<<endl;
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < argc; i += 2){
        
        if(strcmp(argv[i], "-p") == 0){
            port = atoi(argv[i+1]);
            // cout<<argv[i]<<": " << argv[i+1]<<endl;
            
        }else if(strcmp(argv[i], "-s") == 0){
            threadp_sz = atoi(argv[i+1]);
            // cout<<argv[i]<<": " << argv[i+1]<<endl;

        }else if(strcmp(argv[i], "-q") == 0){
            queue_size = atoi(argv[i+1]);
            // cout<<argv[i]<<": " << argv[i+1]<<endl;

        }else if(strcmp(argv[i], "-b") == 0){
            block_sz = atoi(argv[i+1]);
            // cout<<argv[i]<<": " << argv[i+1]<<endl;

        }else{
            cout<<"Wrong input arguments!\n"
                <<"Input example: \n"
                <<"\t./dataServer -p <port> -s <thread_pool_size> -q <queue_size> -b <block_size>"<<endl;
            exit(EXIT_FAILURE);
        }
    }
}

void perror_exit(string message) {
    const char* msg = message.c_str();
    perror(msg);
    exit(EXIT_FAILURE);
}

/* scan directory to find how many files are in it */
int scan_dir(string path){
    int counter = 0;

    for (auto const& dir_entry : fs::recursive_directory_iterator(path)) {
        if(is_directory(dir_entry)){
            continue;
        }else{
            counter++;
        }
    }  
    return counter;  
}