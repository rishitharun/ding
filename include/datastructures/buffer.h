#define BUFFER_DEFAULT_SIZE 100

#ifndef STDLIB_H
#define STDLIB_H
#include<stdlib.h>
#endif

typedef struct
{
  char* buffer;
  short size;
}Buffer;

Buffer* createBuffer();
void readToBuffer(Buffer* buffer, char data, short index);
void freeBuffer(Buffer* buffer);
