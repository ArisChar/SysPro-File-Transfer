#include "worker_thread.hpp"

void sent_files(entry tmp, int block_sz){

    // cout<<"\nIn worker thread: "<<pthread_self()<<endl;

    pthread_mutex_lock(&print);
    cout<< "[Thread: "<<pthread_self()<< "]: Received task: <"<<tmp.file_path<<", "<<tmp.sock<<">"<<endl;
    pthread_mutex_unlock(&print);
    
    /*-------------------------------*/
    // Send filename size and the the file name to the client
    int file_len = tmp.file_path.length()+1;
    int path_len = htonl(file_len);

    if (write(tmp.sock, &path_len, sizeof(path_len)) < 0){
        perror_exit("Write size of filename!");
    }

    const char* file_path = tmp.file_path.c_str();
    if (write(tmp.sock, file_path, file_len) < 0){
        perror_exit("Write name of file!");
    }
    // cout<<"[File_name] sent: "<<file_path<<endl;

    /*-------------------------------*/
    // Send file size to the client
    int file_sz = fs::file_size(file_path);
    int sz_2sent = htonl(file_sz);

    if (write(tmp.sock, &sz_2sent, sizeof(sz_2sent)) < 0){
        perror_exit("Write size of file");
    }
    // cout<<"[File_sz] sent: "<<file_sz<<endl; 

    /*-------------------------------*/
    pthread_mutex_lock(&print);
    cout<<"[Thread: "<<pthread_self()<< "]: About to read file "<<tmp.file_path<<endl;
    pthread_mutex_unlock(&print);

    // Open file and send data to the client
    char* buff_fl = new char[file_sz];
    int val_sent = 0;
    int offset = 0;
    int remain = file_sz;

    ifstream server_file;
    server_file.open(file_path);
    server_file.read(buff_fl, file_sz);
    server_file.close();

    // while we have remaining data to send
    while(remain > 0){
            
        if(remain >= block_sz){
            val_sent = block_sz;
        }else{
            val_sent = remain;
        }
            
        if ((val_sent = write(tmp.sock, buff_fl+offset, val_sent)) < 0){
            perror_exit("Transfer file");
        }

        offset += val_sent;
        remain -= val_sent;
    }

    delete [] buff_fl;
}

void* worker_func(void* ptr_args){

    work_args *args = (work_args*)ptr_args;
    int block_sz = args->blck_sz;
    int queue_sz = args->queue_sz;
    free(args);
    
    while(true){

        pthread_mutex_lock(&mtx);

        while(oura.size() == 0){
            // cout<<"I'm waiting"<<endl;
            pthread_cond_wait(&cond_queue_empty, &mtx);
        }

        entry tmp = oura.front();
        oura.pop();

        pthread_cond_signal(&cond_queue_full);   
        pthread_mutex_unlock(&mtx); 

        pthread_mutex_t temp_comm_mtx = mutex_map.at(tmp.sock);
        pthread_mutex_lock(&temp_comm_mtx);   

        sent_files(tmp, block_sz);
        
        pthread_mutex_unlock(&temp_comm_mtx);

    }
}