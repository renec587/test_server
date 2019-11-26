//  server.h
//
//
//  Created by Rene Chaddock on 2018-09-21.

#ifndef server_h
#define server_h

#include "routines.h"
#include "common.h"

#define BUFLEN 80 //Buffer size
#define POOL_SIZE 1000 //Max safe for select

#define SERVER_PORT 7005

#define TRUE 1
#define FALSE 0

void monitorServer(void);
void startServer(int);
int sendMessage(int,char,struct in_addr*,char,char);

int handleRequest(int,long*,long*);

#endif /* server_h */
