CC = g++
LIBS =  -lpthread -lstdc++fs

SRV_OBJ = dataServer.o  server_utils.o worker_thread.o comm_thread.o 
CLN_OBJ = remoteClient.o client_utils.o

all: dataServer remoteClient

dataServer: $(SRV_OBJ)
	$(CC) -o dataServer $(SRV_OBJ) $(LIBS) 

remoteClient: $(CLN_OBJ)
	$(CC) -o remoteClient $(CLN_OBJ) $(LIBS) 

dataServer.o: dataServer.cpp
	$(CC) -c dataServer.cpp 

remoteClient.o: remoteClient.cpp
	$(CC) -c remoteClient.cpp 

worker_thread.o: worker_thread.cpp
	$(CC) -c worker_thread.cpp $(LIBS) 

comm_thread.o: comm_thread.cpp
	$(CC) -c comm_thread.cpp $(LIBS) 

server_utils.o: server_utils.cpp
	$(CC) -c server_utils.cpp $(LIBS) 

client_utils.o: client_utils.cpp
	$(CC) -c client_utils.cpp $(LIBS) 

clean: 
	rm -rf *o dataServer remoteClient