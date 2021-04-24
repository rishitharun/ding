#include "utils/base.h"
#include "utils/string.h"

static int power(int base, int pow)
{
  int ans=1;

  while(pow--) ans*=base;
  return ans;
}

char* numberTolong(unsigned char* val, int size)
{
  char i=0;
  long int* conv;
  conv = (long int*) malloc(sizeof(long int));
  clearMemory((char*) conv,sizeof(long int));

  size--;
  for(;size>=0;size--)
  *conv+=((int) *(val+size))*power(256,i++);

  return (char*) conv;    
}

unsigned char* strTobin(char *str, int offset, int length, int size)
{
  int i=0, j;
  unsigned char *num;
  long int temp;

  num = (unsigned char*) malloc(size);
  clearMemory(num,size);

  while(i<length)
  {
    temp = (*(str+offset++)-48)*power(10,length-++i);
    j=size-1;
    while (j>=0)
    {
      long int carry = (temp+*(num+j))/256;
      *(num+j)+=temp;
      if(!carry) break;
      else {temp = carry; j--;}
    }
  }
  return num;
}

unsigned char strToint256(char *str, int offset, int length)
{
  int i=0;
  unsigned char intVAL='\0';

  while(i<length)  intVAL =(unsigned int)intVAL + (((unsigned int) *(str+offset++))-48)*power(10,length-++i);
  return intVAL;
}

unsigned char hexstrToint256(char *hexstr, int offset)
{
  int i=0, charVal= -1;
  unsigned char intVAL='\0';

  if      ((*(hexstr+offset)>=48) && (*(hexstr+offset)<=57)) charVal = (unsigned int) *(hexstr+offset)-48;
  else if ((*(hexstr+offset)>=97) && (*(hexstr+offset)<=102)) charVal = (unsigned int) *(hexstr+offset)-87;
  else if ((*(hexstr+offset)>=65) && (*(hexstr+offset)<=70)) charVal = (unsigned int) *(hexstr+offset)-55;
  else {;}

  intVAL = charVal*16;

  if      ((*(hexstr+offset+1)>=48) && (*(hexstr+offset+1)<=57)) charVal = (unsigned int) *(hexstr+offset+1)-48;
  else if ((*(hexstr+offset+1)>=97) && (*(hexstr+offset+1)<=102)) charVal = (unsigned int) *(hexstr+offset+1)-87;
  else if ((*(hexstr+offset+1)>=65) && (*(hexstr+offset+1)<=70)) charVal = (unsigned int) *(hexstr+offset+1)-55;
  else {;}
    
  intVAL = (unsigned int) intVAL+charVal;

  return intVAL;
}

