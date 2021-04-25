#ifndef STDIO_H
#define STDIO_H
#include<stdio.h>
#endif

#ifndef STDLIB_H
#define STDLIB_H
#include<stdlib.h>
#endif

#ifndef STORAGE
#define STORAGE
#include "storage.h"
#endif

#ifndef COLOR
#define COLOR
#include "codes/color.h"
#endif

#include<linux/if_packet.h>

UserVariable* interfacefunc(Node *args);
UserVariable* Log(Node *args);
UserVariable* loop(Node *args);
UserVariable* print(Node *args);
UserVariable* ifcond(Node *args);
UserVariable* cat(Node *args);
UserVariable* sendfunc(Node *args);
UserVariable* recvfunc(Node *args);
UserVariable* container(Node *args);
UserVariable* type(Node* args);
UserVariable* checksum(Node* args);

