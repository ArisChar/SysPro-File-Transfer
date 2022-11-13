#include "server_utils.hpp"
#include "worker_thread.hpp"
#include "comm_thread.hpp"

// ./dataServer -p 12500 -s 1 -q 2 -b 512

int main(int argc, char** argv){
    int port, threadp_sz, queue_size, block_sz;

    // read the input arguments needed for the server
    read_args(argc, argv, port, threadp_sz, queue_size, block_sz);

    cout<<"Server’s parameters are:"
        <<"\nport: "            <<port
        <<"\nthread_pool_size: "<<threadp_sz
        <<"\nqueue_size: "      <<queue_size
        <<"\nBlock_size: "      <<block_sz<<endl;

    // initialize a mutex for used for handling the queue
    pthread_mutex_init(&mtx, 0);

    // we have 2 conditional variables per consumer producer example
    // one for when the queue is empty and one for when it is full
    pthread_cond_init(&cond_queue_full, 0);
    pthread_cond_init(&cond_queue_empty, 0);

    // a mutex for when we want to print things
    pthread_mutex_init(&print, 0);

    int sock, newsock;
    struct sockaddr_in server, client;
    socklen_t clientlen = sizeof(client);
    struct sockaddr *serverptr = (struct sockaddr *)&server;
    struct sockaddr *clientptr = (struct sockaddr *)&client;
    struct hostent *rem;

    /* Create socket */
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror_exit("socket");
    }
    cout<<"Server was successfully initialized..."<<endl; 

    server.sin_family       = AF_INET;          /* Internet domain */
    server.sin_addr.s_addr  = htonl(INADDR_ANY);
    server.sin_port         = htons(port);      /* The given port */

    /* Bind socket to address */
    if (bind(sock, serverptr, sizeof(server)) < 0){
        perror_exit("bind");
    }
    
    /* Listen for connections */
    if (listen(sock, 100) < 0){
        perror_exit("listen"); 
    } 
    cout<<"Listening for connections to port "<<port<<endl;  

    /* Create the worker thread(s) */

    pthread_t *worker_thr = new pthread_t[threadp_sz];
    
    // argument for the worker thread
    work_args *w_arg = (work_args *)malloc(sizeof(work_args));
    w_arg->blck_sz = block_sz;
    w_arg->queue_sz = queue_size;

    for (int i = 0; i < threadp_sz; i++){
        pthread_create(&worker_thr[i], NULL, worker_func, w_arg);
    }

    /* Create the communication threads on for each client */

    // the server runs 24/7
    while(true){
        /* Accept connection */
        if ((newsock = accept(sock, clientptr, &clientlen)) < 0){
            perror_exit("accept");
        }
        cout<<"\nAccepted connection from localhost"<<endl;

        pthread_t comm_thr;
        // argument for the comm thread
        comm_args *parg = (comm_args *)malloc(sizeof(comm_args));
        parg->socket = newsock;
        parg->blck_sz = block_sz;
        parg->queue_sz = queue_size;
        
        pthread_create(&comm_thr, NULL, comm_func, parg);  

    }
    delete[] worker_thr;
    
    return 0;
}