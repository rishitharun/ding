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

UserVariable* add(Node* args);
UserVariable* subtract(Node* args);
UserVariable* lessthan(Node* args);
UserVariable* greaterthan(Node* args);

