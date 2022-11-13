#include "client_utils.hpp"

// ./remoteClient -i 127.0.0.1 -p 12500 -d Server
// ./remoteClient -i linux05.di.uoa.gr -p 12500 -d Server
  
int main(int argc, char** argv){
    int server_port;
    char *server_ip, *directory;

    read_args(argc, argv, server_ip, server_port, directory);

    cout<<"Client’s parameters are:\n"
        <<"serverIP: "              <<server_ip<<endl 
        <<"port: "                  <<server_port<<endl
        <<"directory: "             <<directory<<endl;

    int port, sock, i;
    struct sockaddr_in server;
    struct sockaddr *serverptr = (struct sockaddr*)&server;
    struct hostent *rem;

    /* Create socket */
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror_exit("socket");
    }

    /* Find server address */
    if ((rem = gethostbyname(server_ip)) == NULL) {	
	    herror("gethostbyname"); 
        exit(1);
    }

    port = server_port;
    server.sin_family = AF_INET;            /* Internet domain */
    memcpy(&server.sin_addr, rem->h_addr, rem->h_length);
    server.sin_port = htons(port);          /* Server port */	

    /* Initiate connection */
    if (connect(sock, serverptr, sizeof(server)) < 0){
        perror_exit("connect");
    }
	cout<<"Connecting to "<<server_ip<<" on port "<<port<<endl<<endl; 

    /*-------------------------------*/

    int dir_len = strlen(directory)+1;
    int temp = htonl(dir_len);

    // Send the size of the directory name to the server
    if (write(sock, &temp, sizeof(temp)) < 0){
        perror_exit("write");
    }
    // cout<<"[Directory] size sent"<<endl;    

    // Send the directory name to the server
    if (write(sock, directory, strlen(directory)+1) < 0){
        perror_exit("write");
    }
    // cout<<"[Directory] sent:"<<directory<<endl;  

    // Get number of files you will make from the server
    int tmp_fnum = 0;
    if (read(sock, &tmp_fnum, sizeof(tmp_fnum)) < 0){
        perror_exit("read");
    } 

    int file_num = ntohl(tmp_fnum);
    // cout<<"Numbers of files I will make: "<<file_num<<endl;

    // Get the block size from the server
    int tmp_block_sz = 0;
    if (read(sock, &tmp_block_sz, sizeof(tmp_block_sz)) < 0){
        perror_exit("read");
    }

    int block_sz = ntohl(tmp_block_sz);
    // cout<<"Got block size: "<<block_sz<<endl<<endl;

    /*-------------------------------*/
    string my_dir = "./Client/";
    fs::create_directories(my_dir);

    // repeat until we get all the files from the server
    while (file_num > 0) {

        int file_len = 0;
        // get the size of the file name from the server
        if (read(sock, &file_len, sizeof(file_len)) < 0){
            perror_exit("read size of the file name");
        }
        int tfile_len = ntohl(file_len);  

        // get the file name from the server
        char fil_buff[tfile_len];
        if (read(sock, fil_buff, tfile_len) < 0){
            perror_exit("read file name");
        }

        /*------------MAKES THE FOLDER---------------*/
        // add ./Client to each incoming files path
        // cause we will store the there

        string newPath = my_dir.append((fil_buff));
        int pos = newPath.find_last_of('/');
        string file = newPath.substr(pos+1);
        string parent = newPath.substr(0, pos);

        cout<<"[Recieved]: "<<fil_buff<<endl;

        if (fs::exists(parent) == false){
            fs::create_directories(parent);
            // cout<<"Made Folder\n";
        }

        // recieve file size
        int tmp_fl_sz = 0;
        if (read(sock, &tmp_fl_sz, sizeof(tmp_fl_sz)) < 0){
            perror_exit("read");
        } 

        int file_sz = ntohl(tmp_fl_sz);
        // cout<<"File size is: "<<file_sz<<endl;

        // delete file if it already exists
        if(fs::remove(newPath)){
            // cout<<"File [does exist] and will proceed to deletion and then creation.\n\n";
        }else{
            // cout<<"File [does not exist] and will proceed to creation.\n\n";
        }
        // string s;
        // cin>> s;
        
        char* buff_fl = new char[file_sz];
        int val_read = 0;
        int val_exp = 0;
        int offset = 0;
        int remain = file_sz;

        while (remain > 0) {

            if(remain >= block_sz){
                val_exp = block_sz;
            }else{
                val_exp = remain;
            }

            if ((val_read = read(sock, &buff_fl[offset], val_exp)) < 0){
                perror_exit("Read file");
            }

            offset += val_read;
            remain -= val_read;
        }

        // open folder and tranfer the data
        ofstream client_file;
        client_file.open(newPath);
        client_file.write(buff_fl, file_sz);
        client_file.close();

        delete [] buff_fl;
        
        /*-------------------------------*/
        my_dir = "./Client/";
        file_num--;
    }
    
    /*-------------------------------*/

    printf("\nClosing Socket.\n");
    close(sock);
    return 0;
}