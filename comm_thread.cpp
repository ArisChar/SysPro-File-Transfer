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

#include "comm_thread.hpp"

void insert_queue(string path, queue<entry> &oura, int sock, int queue_sz){
    
    pthread_mutex_lock(&print);
    cout<<"[Thread: "<<pthread_self()<<"]: About to scan directory "<<path<<endl;
    pthread_mutex_unlock(&print);

    // read the folder and push the files in the queue
    for (auto const& dir_entry : fs::recursive_directory_iterator(path)) {
        if(is_directory(dir_entry)){
            // cout<<"[Folder]: "<< dir_entry.path().string() << '\n';
        }else{
            pthread_mutex_lock(&mtx);

            while(oura.size() == queue_sz){
                pthread_cond_wait(&cond_queue_full, &mtx);
            }
            
            string temp = dir_entry.path().string();

            pthread_mutex_lock(&print);
            cout<< "[Thread: "<<pthread_self()<< "]: Adding file "<<temp<<" to the queue..."<<endl;
            pthread_mutex_unlock(&print);

            oura.push(entry(sock,temp));

            pthread_cond_signal(&cond_queue_empty);
            pthread_mutex_unlock(&mtx);
        }
    } 
}

void* comm_func(void* ptr_args){
    
    comm_args *args = (comm_args*)ptr_args;
    int newsock = args->socket;
    int block_sz = args->blck_sz;
    int queue_sz = args->queue_sz;
    free(args);

    pthread_mutex_t comm_mtx;
    pthread_mutex_init(&comm_mtx, 0);
    mutex_map.insert({newsock, comm_mtx});

    /* Read from client size of directory */
    int temp = 0;
    if (read(newsock, &temp, sizeof(temp)) < 0){
        perror_exit("Read size of directory!");
    } 

    int dir_len = ntohl(temp);
    // cout<<"[Recieved] size of directory "<<dir_len<<endl;

    /* Read from client name of directory! */
    char dir_buff[dir_len];
    if (read(newsock, dir_buff, dir_len) < 0){
        perror_exit("Read name of directory!");
    }
    // cout<<"[Recieved] path is: "<<dir_buff<<endl<<endl;
   
    /*----------------------------------------------*/ 
    int file_num = scan_dir(string(dir_buff));

    // Send number of file(s) to be made to the client
    int tmp_fnum = htonl(file_num);
    if (write(newsock, &tmp_fnum, sizeof(tmp_fnum)) < 0){
        perror_exit("Write size of files client will make!");
    }
    // cout<<"Send number of file(s) to be made: "<<file_num<<endl;

    // Send the block size to the client
    int tmp_block_sz = htonl(block_sz);
    if (write(newsock, &tmp_block_sz, sizeof(tmp_block_sz)) < 0){
        perror_exit("Write size of block!");
    }
    // cout<<"Sent block size: "<<block_sz<<endl;      

    /*------------INSERT IN QUEUE-------------------*/
    insert_queue(string(dir_buff), oura, newsock, queue_sz);

    // cout<< "Size of queue: "<<oura.size()<<endl;
    
    return NULL;
}
