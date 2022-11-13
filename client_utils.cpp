#include "client_utils.hpp"

void read_args(int argc, char* argv[], char* &server_ip, int& server_port, char*& directory){
    /* read the arguments in any order given */
    
    if(argc != 7){
        cout<<"Not enough input aruments!\n"
            <<"Input example: \n"
            <<"\t./remoteClient -i <server_ip> -p <server_port> -d <directory>"<<endl;
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < argc; i += 2){
        
        if(strcmp(argv[i], "-i") == 0){
            server_ip = argv[i+1];
            // cout<<argv[i]<<": " << argv[i+1]<<endl;
            
        }else if(strcmp(argv[i], "-p") == 0){
            server_port = atoi(argv[i+1]);
            // cout<<argv[i]<<": " << argv[i+1]<<endl;

        }else if(strcmp(argv[i], "-d") == 0){
            directory = argv[i+1];
            // cout<<argv[i]<<": " << argv[i+1]<<endl;

        }else{
            cout<<"Wrong input arguments!\n"
                <<"Input example: \n"
                <<"\t./remoteClient -i <server_ip> -p <server_port> -d <directory>"<<endl;
            exit(EXIT_FAILURE);
        }
    }
}

void perror_exit(string message) {
    const char* msg = message.c_str();
    perror(msg);
    exit(EXIT_FAILURE);
}