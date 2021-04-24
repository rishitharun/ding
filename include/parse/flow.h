#ifndef STDIO_H
#define STDIO_H
#include<stdio.h>
#endif

#ifndef STDLIB_H
#define STDLIB_H
#include<stdlib.h>
#endif

#ifndef COLOR
#define COLOR
#include "codes/color.h"
#endif

#ifndef STORAGE
#define STORAGE
#include "storage.h"
#endif

#ifndef BOOLEAN_H
#define BOOLEAN_H
typedef enum boolean{True=1,False=0} boolean;
#endif

boolean parseFlow(char *line, short line_len);

