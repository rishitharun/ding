#ifndef BOOLEAN_H
#define BOOLEAN_H
typedef enum boolean{True=1,False=0} boolean;
#endif

#ifndef STORAGE
#define STORAGE
#include "storage.h"
#endif

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

#define STACK_DEFAULT_SIZE 5

typedef struct
{
  short size, current;
  boolean dynamic;
  char** stack_nodes;
}Stack;

Stack* initFixedStack(short size);
Stack* initDynamicStack();
void pushStack(Stack* stack, char* data);
char* popStack(Stack* stack);
void resetStack(Stack* stack);
void freeStack(Stack* stack);

