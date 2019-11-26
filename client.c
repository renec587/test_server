/*
//  client.c
//  COMP 7881
//
//  Created by Rene Chaddock, 26 October 2019.
For Container Deployment testing. Connects to the server and says hello. */

#include "client.h"

char* Server_Name = "localhost";

int main(int argc,char** argv) {
	char* command = "HELLO";
	char* response = "OK";
	char buf[RESPONSE_SIZE+1]; // Add 1 to store a null value.
	buf[RESPONSE_SIZE] = 0; // Ensure the last character is null (won't be erased)

	if(argc > 2) {
		printf("Usage: %s <serverName or IP>)\n",argv[0]);
		exit(EXIT_FAILURE);
	}
	if(argc > 1) Server_Name = argv[1];

	int serverfd = makeConnection(Server_Name, SERVER_PORT);
	if(serverfd == 0) printf("Error making server connection\n");

	// Send HELLO to server ever second.
	while(serverfd) {
		write(serverfd, command, strlen(command));
		
		// Check server response
		read(serverfd, buf, RESPONSE_SIZE);
		printf("Server said: %s\n",buf);
		
		if(strncasecmp(buf, response, strlen(response)) != 0) {
			printf("Server reported an error?\n");
			serverfd = 0;
		}
		sleep(1);
	}

	printf("We are done. Quitting\n");
	// Disconnect from Server
	shutdown(serverfd, SHUT_RDWR);
	close(serverfd);
}
