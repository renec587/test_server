//  common.c
//  c7005a1
//
//  Created by Rene Chaddock, A01041775,  on 2018-09-21.
/*  Contains Routines common to client and server */

#include "common.h"

/* error - wrapper for perror */
void error(char *msg) {
	perror(msg);
	exit(0);
}

/* Makes a single connection to server and returns the fd */
int makeConnection(char* serverName,int serverPort) {
	int fd;
	struct hostent* hp;
	struct sockaddr_in* server = (struct sockaddr_in*)calloc(1,sizeof(struct sockaddr_in));
	server->sin_family = AF_INET;
	server->sin_port = htons(serverPort);
	if((hp = gethostbyname(serverName)) == NULL) {
		perror("Unknown server address");
		return 0;
	}
	bcopy(hp->h_addr,(char*)&server->sin_addr,hp->h_length);
	if((fd = socket(AF_INET,SOCK_STREAM,0)) == -1) {
		perror("Error opening socket");
		return 0;
	}
	if(connect(fd,(struct sockaddr*)server,sizeof(struct sockaddr_in)) == -1) {
		perror("Can't connect to server");
		close(fd);
		return 0;
	}
	return fd;
}

/* Binds a socket on the specified port number, and sets up a listen. Returns the file descriptor */
int openPort(int portnum) {
	int serverSock;
	struct sockaddr_in* server;
	if ((serverSock = socket(AF_INET,SOCK_STREAM,0)) == -1) error("Can't create a socket");
	
	server = (struct sockaddr_in*)calloc(1,sizeof(struct sockaddr_in));
	server->sin_family = AF_INET;
	server->sin_port = htons(portnum);
	server->sin_addr.s_addr = htonl(INADDR_ANY);
	
	int opt = 1;
	if (setsockopt (serverSock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
		perror("setsockopt");
	
	if(bind(serverSock,(struct sockaddr*)server,sizeof(struct sockaddr_in)) == -1) error("Can't find name to socket");
	printf("Listening to TCP port %d.\n",portnum);
	listen(serverSock,LISTEN_CONNECTIONS);
	return serverSock;
}
