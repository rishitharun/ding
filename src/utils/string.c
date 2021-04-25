#include "utils/string.h"

int length(const char* str)
{
  int i=0;
  while(*(str+i++));
  return i;
}

void clearMemory(char* buff, short size)
{
  while(size--) *buff++ = 0;
}

boolean stringsEqual(char* str1, char* str2)
{
  while (*str1 && *str2)
  {
    if (*str1++ != *str2++) return False;
    else {;}
  }
  if (*str1 != *str2) return False;
  else return True;
}

char* stringsCopy(char* str, int len)
{
  char* cpystr = (char*) malloc(sizeof(char)*len);
  int i = 0;
  while(*str) *(cpystr+i++) = *str++;
  *(cpystr+i) = 0;
  return cpystr;
}


char stringsCompare(char* str1, char* str2)
{
  char ret = 1;
  while (*str1 && *str2)
  {
    if (*str1 < *str2) return -ret;
    else if (*str1 > *str2) return ret;
    else ret++;
    str1++; str2++;
  }
  if (*str1 < *str2) return -ret;
  else if (*str1 > *str2) return ret;
  else return 0;
}


