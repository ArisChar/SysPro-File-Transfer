# SysPro-File-Transfer

The goal of this assignment was to familiarize us with thread creation and network communication

The program's purpose is to be able to transfer the requested file (copy all the contents of a
directory recursively) from the server to the local file system of the client.

The server should be able to handle requests from different clients at all times (can only be terminated with CTRL-C or kill)
moment and process each request in parallel, breaking it into independent operations
(multithreading). Accordingly, the client should process the data being sent from the 
server and create a local copy of the directory requested. Many clients can connect to 
a server at the same time. Communication between them is done by sockets.
The file transfer protocol used is TCP/IP.

The server has two types of threads, communication and worker.  
The server will create a new communication thread for each new connection it accepts 
and the available worker threads will be responsible for the file transfer. 
Each request is first placed in a queue which is shared for all the worker threads.
The files will be read and sent to the client per block (the size of which will be given via command line).

## Compilation:  
&nbsp; type make all

## To run the server:   
&nbsp; ./dataServer -p <port> -s <thread_pool_size> -q <queue_size> -b <block_size>  
 
 port: The port on which the server will listen for external connections.  
 thread pool size: The number of worker threads in the thread pool  
 queue size: The number of positions in the execution queue.  
 block size: The size of the file blocks in bytes that the worker threads will send.  
 
## To run the client:  
&nbsp; ./remoteClient -i <server_ip> -p <server_port> -d <directory>  

 server ip: The IP address used by the server.  
 server port: The port on which the server listens for external connections.  
 directory: The directory to copy (a relative path).  
 
