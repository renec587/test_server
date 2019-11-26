//
//  server.c
//
#include "server.h"

int numServers = 1;

int main(int argc,char** argv) {
	int clientSock = 0,	numReady,numClients,full,serverSock;
	struct sockaddr_in client;
	socklen_t clientLen = sizeof(client); //To pass &clientLen
	struct in_addr statAddr;
	long bytesRead = 0,bytesWritten = 0;
	
	int fds[FD_SETSIZE]; //Set to 0 if fd closed
	
	for(int i = 0; i < FD_SETSIZE; i++ ) fds[i] = 0;
	numReady = numClients = full = 0;
	
	serverSock = openPort(SERVER_PORT);
	
	fd_set listenSet,activeSet;
	int highSocket = serverSock;
	FD_ZERO(&listenSet);
	FD_SET(serverSock,&listenSet);
	
	while(TRUE) {
		activeSet = listenSet;
		if((numReady = select(highSocket+1,&activeSet,NULL,NULL,NULL)) < 1) perror("Select");
		if(FD_ISSET(serverSock,&activeSet)) { //New connection
			numReady--;
			FD_CLR(serverSock,&activeSet); //So we don't trigger below
			if (numClients < POOL_SIZE && (clientSock = accept(serverSock,(struct sockaddr *)&client,&clientLen)) < 0) {
				perror("Can't accept client");
			} else if(numClients < POOL_SIZE && clientSock > 0) {
				if(highSocket < clientSock) highSocket = clientSock;
				FD_SET(clientSock,&listenSet);
				fds[clientSock] = client.sin_addr.s_addr;
				numClients++;
			}
		}
		for(int i = 0; i < FD_SETSIZE && numReady > 0;i++) {
			if(FD_ISSET(i,&activeSet)) {
				numReady--;
				bytesRead = bytesWritten = 0;
				if(handleRequest(i,&bytesRead,&bytesWritten) < 0) { // We must quit.
					FD_CLR(i,&listenSet);
					numClients--;
					fds[i] = 0;
				} else {
					statAddr.s_addr = fds[i];
				}
			}
		}
	} //end While
	return EXIT_SUCCESS;
}

/* Handles request made by FD. Returns -1 if we closed connection,
 and 1 otherwise */
int handleRequest(int fd,long* bytesRead,long* bytesWritten) {
	char buffer[BUFLEN];
	char* quitMessage = "BY";
	char* invalidMessage = "NO";
	long answer;
	int numChars = (int)recv(fd,buffer,BUFLEN-1,0);
	buffer[numChars] = 0; //Null terminate received string.
	if(numChars <= 0) { //Close connection
		printf("Client has closed connection\n");
		shutdown(fd,SHUT_RDWR);
		close(fd);
		return -1;
	} else if((answer = parseString(buffer,fd)) >= 0) {
		*bytesWritten += answer;
	} else {
		if(answer == -1) {
			printf("Client requested we close connection\n");
			dprintf(fd,"%s",quitMessage);
		}
		if(answer == -2) {
			printf("Received Invalid Command:%s\n",buffer);
			dprintf(fd,"%s",invalidMessage);
			*bytesWritten += answer;
		}
		shutdown(fd,SHUT_RDWR);
		close(fd);
		*bytesRead += numChars;
		return -1;
	}
	*bytesRead += numChars;
	return numChars;
}
