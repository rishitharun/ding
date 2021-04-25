#include "utils/base.h"
#include "memory/number.h"

char* allocateNumber(char *word, char* length)
{
  unsigned char* num;
  int col_off = -1, i=0, num_size;

  while(*(word+i)!='B')
  {
    if (*(word+i)==':') col_off = i;
    else {;}
    i++;
  }

  if (col_off==0) { fprintf(stderr, ERROR "Number value not specified in <%s> !\n" RESET, word); return NULL; }
  else if (col_off==-1) { fprintf(stderr, ERROR"Size of number not specified in <%s> !\n"RESET,word); return NULL; }
  else if (col_off == i-1) {fprintf(stderr, ERROR"Size of number not specified in <%s> !\n"RESET,word); return NULL; }
  else {;}

  num_size = (int) strToint256(word, col_off+1, i-col_off-1);
  num = (unsigned char*) strTobin(word, 0, col_off, num_size);

  *length = num_size;

  return (char*) num;
}
