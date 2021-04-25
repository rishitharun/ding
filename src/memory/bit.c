#include "utils/base.h"
#include "memory/bit.h"

static inline unsigned char ceilDiv(unsigned char bits)
{
  return (bits%8) ? ((unsigned char)(bits/8)+1) : (unsigned char)(bits/8);
}

static inline unsigned char maskExtraBits(unsigned char msbyte, char num_xtra)
{
  return (unsigned char) msbyte & (0xff>>(8-(num_xtra%8)));
}

char* allocateBit(char *word, char *length)
{
  unsigned char* num;
  int col_off = -1, i=0, num_size;

  while(*(word+i) != 'b')
  {
    if (*(word+i)==':') col_off = i;
    else {;}
    i++;
  }

  if (col_off==0) { fprintf(stderr, ERROR "Bit value not specified in <%s> !\n" RESET, word); return NULL; }
  else if (col_off==-1) { fprintf(stderr, ERROR"Number of bits not specified in <%s> !\n"RESET,word); return NULL; }
  else if (col_off == i-1) {fprintf(stderr, ERROR"Number of bits not specified in <%s> !\n"RESET,word); return NULL; }
  else {;}

  num_size = (int) strToint256(word, col_off+1, i-col_off-1);
  i = num_size;

  num_size = (int) ceilDiv(num_size);

  num = (unsigned char*) strTobin(word, 0, col_off, num_size);

  *num = maskExtraBits(*num, i);
  *length = num_size;

  return (char*) num;
} 
