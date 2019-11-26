//  common.h
//  c7005a1
//
//  Created by Rene Chaddock on 2018-09-21.

#ifndef common_h
#define common_h

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <unistd.h>
#include <fcntl.h>

#define SERVER_PORT 7005
#define CLIENT_PORT 7006
#define LISTEN_CONNECTIONS 10
#define BUFFER_SIZE 10000
#define RESPONSE_SIZE 40

void error(char*);
int makeConnection(char*,int);
int openPort(int);

#endif /* common_h */
