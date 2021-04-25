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

UserVariable* str2ip4(Node* args);
UserVariable* str2mac(Node* args);
UserVariable* str2num(Node* args);
UserVariable* str2bit(Node* args);
UserVariable* str2stream(Node* args);

