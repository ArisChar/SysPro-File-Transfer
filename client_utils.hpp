#ifndef CLIENT_UTILS_H
#define CLIENT_UTILS_H

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
#include <experimental/filesystem>
#include <fstream>

using namespace std;
namespace fs = std::experimental::filesystem;

void read_args(int argc, char* argv[], char* &server_ip, int& server_port, char*& directory);
void perror_exit(string message);

#endif