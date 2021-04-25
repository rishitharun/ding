#ifndef STDLIB_H
#define STDLIB_H
#include<stdlib.h>
#endif

#ifndef BOOLEAN_H
#define BOOLEAN_H
typedef enum boolean{True=1,False=0} boolean;
#endif

char* numberTolong(unsigned char* val, int size);
unsigned char hexstrToint256(char *hexstr, int offset);
unsigned char strToint256(char *str, int offset, int length);
unsigned char* strTobin(char *str, int offset, int length, int size);

