#ifndef STDIO_H
#define STDIO_H
#include<stdio.h>
#endif

#ifndef BOOLEAN_H
#define BOOLEAN_H
typedef enum boolean{True=1,False=0} boolean;
#endif

#ifndef COLOR
#define COLOR
#include "codes/color.h"
#endif

#include <net/if.h>

boolean getInterface(char* interface_name, short interface_name_str_len);
boolean checkInterface(char* interface_str);
