#include "utils/base.h"
#include "memory/mac.h"
#include "codes/token.h"

#include "logger.h"

char* allocateMAC(char *word, char *len)
{
  char *macaddr;
  boolean charVal;
  int i=0, length = 0, index = 0, offset = 0;

  macaddr = (char*) malloc(6*sizeof(char));

  #if LOGGING_ENABLED(LOG_MEMORY)
    MEMORY_LOGGING_UTIL("M\0", 6*sizeof(char), macaddr, "mac address in allocateMAC()");
  #endif

  while(*(word+i))
  {
    charVal = True;

    if      (*(word+i)==TILDE_TOKEN) {;}
    else if ((*(word+i)>=48) && (*(word+i)<=57)) {;}
    else if ((*(word+i)>=97) && (*(word+i)<=102)) {;}
    else if ((*(word+i)>=65) && (*(word+i)<=70)) {;}
    else {charVal = False;}

    if(!charVal)
    {
      fprintf(stderr, ERROR "Invalid MAC address ! Invalid Hex numbers in <%s>!\n" RESET, word);
      #if LOGGING_ENABLED(LOG_MEMORY)
        MEMORY_LOGGING_UTIL("F\0", 6*sizeof(char), macaddr, "mac address in allocateMAC()");
      #endif
      free(macaddr);
      return NULL;
    }
    else {;}

    if((*(word+i)==TILDE_TOKEN))
    {
      if(length!=2)
      {
        fprintf(stderr, ERROR "Invalid MAC Address ! Only octet must be given in <%s>!\n" RESET, word);
        #if LOGGING_ENABLED(LOG_MEMORY)
          MEMORY_LOGGING_UTIL("F\0", 6*sizeof(char), macaddr, "mac address in allocateMAC()");
        #endif
        free(macaddr);
        return NULL;
      }
      else {;}
      *(macaddr+index++) = hexstrToint256(word,offset);
      length = 0;
      offset=i+1;
    }
    else { length++; }
    i++;
  }

  if(length>2)
  {
    fprintf(stderr, ERROR "Invalid MAC Address !\n" RESET);
    #if LOGGING_ENABLED(LOG_MEMORY)
      MEMORY_LOGGING_UTIL("F\0", 6*sizeof(char), macaddr, "mac address in allocateMAC()");
    #endif
    free(macaddr); return NULL;
  }
  else {;}

  if(index!=5)
  {
    fprintf(stderr, ERROR "Invalid MAC Address !\n" RESET);
    #if LOGGING_ENABLED(LOG_MEMORY)
      MEMORY_LOGGING_UTIL("F\0", 6*sizeof(char), macaddr, "mac address in allocateMAC()");
    #endif
    free(macaddr); return NULL;
  }
  else if((index==5) && (*(word+i-1)==TILDE_TOKEN))
  {
    fprintf(stderr, ERROR "Invalid MAC Address !\n" RESET);
    #if LOGGING_ENABLED(LOG_MEMORY)
      MEMORY_LOGGING_UTIL("F\0", 6*sizeof(char), macaddr, "mac address in allocateMAC()");
    #endif
    free(macaddr); return NULL;
  }
  else{;}

  *(macaddr+index) = hexstrToint256(word,offset);
  *len = 6;

  return macaddr;
}


