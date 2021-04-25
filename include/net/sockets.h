#ifndef STDIO_H
#define STDIO_H
#include<stdio.h>
#endif

#ifndef COLOR
#define COLOR
#include "codes/color.h"
#endif

#ifndef SYS_SOCK_H
#define SYS_SOCK_H
#include <sys/socket.h>
#endif

#ifndef UNISTD_H
#define UNISTD_H
#include<unistd.h>
#endif

#include<arpa/inet.h>

#ifndef BOOLEAN_H
#define BOOLEAN_H
typedef enum boolean{True=1,False=0} boolean;
#endif

int sock_desc;
int interface;

char* interface_str;
char interface_str_len;

boolean createSocket();
boolean destroySocket();

