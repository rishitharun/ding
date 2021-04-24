#ifndef STDIO_H
#define STDIO_H
#include<stdio.h>
#endif

#ifndef COLOR
#define COLOR
#include "codes/color.h"
#endif

#ifndef BOOLEAN_H
#define BOOLEAN_H
typedef enum boolean{True=1,False=0} boolean;
#endif

int line_count, *line_len;
boolean decl_flag, func_flag, flow_flag;

boolean interpret(char *line, short line_len, boolean shell);
boolean interpretLines(char* source, int source_length, boolean shell);

