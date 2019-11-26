//
//  routines.c
//

#include "routines.h"
#include "common.h"
#include <string.h>
#include <stdlib.h>

struct funcCalls {
	char* code;
	int (*func)(char*,char*);
};

/* Respond to a Hello */
int reply(char* filename,char* hostname) {
	static int repetition_count = 0;
	repetition_count++;
	printf("HELLO Received:%d\n",repetition_count);
	return repetition_count;
}

// Returns a -1 to indicate we should exit.
int quit(char* unused,char* unused2) {
	return -1;
}

/* Parses strings in the format: [COMMAND] <FILENAME> */
int parseString(char* value,int fd) {
	struct funcCalls functionLookup[] = { {"HELLO",&reply},{"QUIT",&quit} };
	char *code;
	char* filename = NULL;
	int ret_value;
	
	printf("Executing:%s\n",value);

	code = strsep(&value," ");
	
	// Ensure we don't allow any slash characters (Files must be in local directory)
	if (value != NULL && (filename = strrchr(value, '/')) != NULL) {
		printf("Eliminating Slashes\n");
		filename += 1; //One character past Last occurence of the '/' character
	} else {
		filename = value; //If no slashes, filename was set to NULL, need to reset.
	}
	
	for(int i = 0; i < (sizeof(functionLookup)/ sizeof(functionLookup[0]));i++) {
		if(strncmp(functionLookup[i].code,code,2) == 0) {
			ret_value = functionLookup[i].func(filename,getConnectedAddress(fd));
			if(ret_value > 0) dprintf(fd,"OK. So far %d messages.\n",ret_value);
			return ret_value;
		}
	}
	return -2; //Command was invalid
}

/* Returns the dot notatation of the address associated with the network socket.
 	Returns NULL on error */
char* getConnectedAddress(int socket) {
	char* returnName = NULL;
	struct sockaddr_in client;
	socklen_t clientLen = sizeof(client); //To pass &clientLen
	if(getpeername(socket, (struct sockaddr*)&client, &clientLen) == -1) {
		perror("Could not get address from socket");
		return NULL;
	}
	returnName = inet_ntoa(client.sin_addr);
#ifdef DEBUG
	printf("DEBUG: Converted address from socket %d to %s\n",socket,returnName);
#endif
	return returnName;
}
