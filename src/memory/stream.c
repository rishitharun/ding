#include "utils/base.h"
#include "memory/stream.h"
#include "logger.h"

char* allocateStream(char *word, char *length)
{
  unsigned char *stream;
  short size = 1;
  char offset = 0;

  stream = (char*) malloc(size);

  if (*word == 'x')
  {
    word++;
    do
    {
      if (!*(word+offset+1)) { fprintf(stderr, ERROR"Hex Stream must be of even length !\n"RESET); free(stream); return NULL; }
      else {;}

      if ((*(word+offset)>=48) && (*(word+offset)<=57)) {;}
      else if ((*(word+offset)>=97) && (*(word+offset)<=102)) {;}
      else if ((*(word+offset)>=65) && (*(word+offset)<=70)) {;}
      else { fprintf(stderr, ERROR"Invalid hex stream <x%s> !\n"RESET,word); free(stream); return NULL; }

      if ((*(word+offset+1)>=48) && (*(word+offset+1)<=57)) {;}
      else if ((*(word+offset+1)>=97) && (*(word+offset+1)<=102)) {;}
      else if ((*(word+offset+1)>=65) && (*(word+offset+1)<=70)) {;}
      else { fprintf(stderr, ERROR"Invalid hex stream <x%s> !\n"RESET,word); free(stream); return NULL; }

      stream = (unsigned char*) realloc(stream,size);
      *(stream+(size++)-1) = hexstrToint256(word,offset);

      offset += 2;
    }while(*(word+offset));
  }
  
  else
  {
    short length = 0;

    while(*(word+offset+length))
    {
      if((*(word+offset+length) == ' ') || (*(word+offset+length) == '\t')) continue;
      else {;}

      if(*(word+offset+length) == ':')
      {
        stream = (char*) realloc(stream,size);
        *(stream+(size++)-1) = strToint256(word, offset, length);
        offset += length+1; length = 0;
        continue;
      }
      else {;}

      if(((*(word+offset+length)<48) || (*(word+offset+length)>57)))
      {
        fprintf(stderr, ERROR "Invalid Num stream <%s> !\n" RESET, word);
        free(stream);
        return NULL;
      }
      else{;}

      if(*(word+offset+length+1) == '\0')
      {
        stream = (unsigned char*) realloc(stream,size);
        *(stream+(size++)-1) = strToint256(word, offset, length+1);
        length = 0;
        break;
      }
      else {;}

      length++;
    }
  }

  *length = --size;
  #if LOGGING_ENABLED(LOG_MEMORY)
    MEMORY_LOGGING_UTIL("M\0", (long int) *length, stream, "stream in allocateStream()");
  #endif
  return stream;
}
