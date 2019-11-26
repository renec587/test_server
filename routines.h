//  routines.h
//
//
//  Created by Rene Chaddock on 2018-02-10.

#ifndef routines_h
#define routines_h

#include <stdio.h>
#include <fcntl.h>
#include <arpa/inet.h>
#ifdef LINUX
#include <sys/stat.h>
#include <sys/sendfile.h>
#endif

int parseString(char*,int);
char* getConnectedAddress(int);

#endif /* routines_h */
