/* LOGGING UTILS */

//#define LOGGING_TYPE LOG_ALL

#define LOG_NOTHING 0x00
#define LOG_CONTROL_STRUCTURES 0x08
#define LOG_GENERAL 0x04
#define LOG_NETWORK 0x02
#define LOG_MEMORY 0x01
#define LOG_ALL 0xff

#ifndef LOGGING_TYPE
#define LOGGING_TYPE LOG_NOTHING
#endif

#define LOGGING_ENABLED(LOGGING_VALUE) (LOGGING_VALUE & LOGGING_TYPE)

#if LOGGING_TYPE > 0

  #ifndef STDIO_H
  #define STDIO_H
  #include<stdio.h>
  #endif

  #ifndef STDLIB_H
  #define STDLIB_H
  #include<stdlib.h>
  #endif

  #include "utils/string.h"

  FILE* LOG;

  #define DATE_TIME() fprintf(LOG, "[%s-%s] ", __DATE__, __TIME__)

  #define NETWORK_LOGGING_UTIL_NN0(TEXT) \
  { \
    DATE_TIME(); \
    fprintf(LOG, "[NETWORK INFO]: "TEXT); \
  }

  static void logHexDump(unsigned char* packet, char len)
  {
    short i = 0, j=0;
    while(i<len)
    {
      NETWORK_LOGGING_UTIL_NN0("");
      for(j=0;j<8 && i<len;j++,i++)
        fprintf(LOG, "%02x  ",*(packet+i));
      fprintf(LOG, "   ");
      for(j=0;j<8 && i<len;j++,i++)
        fprintf(LOG, "%02x  ",*(packet+i));
      fprintf(LOG, "\n");
    }

  }
  static char* stringsJoin(char* str1, char* str2, char join)
  {
    char len1, len2, i, len;
    len1 = length(str1);
    len2 = length(str2);
    len = 0;
    char* catstr = (char*) malloc(len1+len2);

    for(i=0;i<len1-1;i++) *(catstr+len++) = *(str1+i);
    *(catstr+len++) = join;
    for(i=0;i<len2;i++) *(catstr+len++) = *(str2+i);
    return catstr;
  }

  static char* replace(char* str, char replace, char replace_with)
  {
    char i = 0;
    while(*(str+i))
    {
      if(*(str+i) == replace) *(str+i) = replace_with;
      else {;}
      i++;
    }
    return str;
  }

  #define GEN_LOGGING_UTIL1(TEXT, ARG1) \
  { \
    DATE_TIME(); \
    fprintf(LOG, "[SOURCE INFO]: "TEXT"\n", ARG1); \
  }

  #define CS_LOGGING_UTIL0(CSTYPE, TEXT) \
  { \
    DATE_TIME(); \
    fprintf(LOG, "["CSTYPE"]: "TEXT"\n"); \
  }

  #define CS_LOGGING_UTIL1(CSTYPE, TEXT, ARG1) \
  { \
    DATE_TIME(); \
    fprintf(LOG, "["CSTYPE"]: "TEXT"\n", ARG1); \
  }

  #define NETWORK_LOGGING_UTIL0(TEXT) \
  { \
    DATE_TIME(); \
    fprintf(LOG, "[NETWORK INFO]: "TEXT"\n"); \
  }

  #define NETWORK_LOGGING_UTIL1(TEXT, ARG1) \
  { \
    DATE_TIME(); \
    fprintf(LOG, "[NETWORK INFO]: "TEXT"\n", ARG1); \
  }

  #define NETWORK_LOGGING_UTIL2(TEXT, ARG1, ARG2) \
  { \
    DATE_TIME(); \
    fprintf(LOG, "[NETWORK INFO]: "TEXT"\n", ARG1, ARG2); \
  }

  #define NETWORK_LOGGING_UTIL3(TEXT, ARG1, ARG2, ARG3) \
  { \
    DATE_TIME(); \
    fprintf(LOG, "[NETWORK INFO]: "TEXT"\n", ARG1, ARG2, ARG3); \
  }

  #define MEMORY_LOGGING_UTIL(ACTION, SIZE, ADDRESS, TYPE) \
  { \
    char* ACTION_EXT; \
    switch(*(ACTION)) \
    { \
      case 'M': \
        ACTION_EXT = "allocated\0"; \
        break; \
      case 'R': \
        ACTION_EXT = "re-allocated\0"; \
        break; \
      case 'F': \
        ACTION_EXT = "freed\0"; \
        break; \
      default: \
        ACTION_EXT = "(error)\0"; \
    } \
    DATE_TIME(); \
    fprintf(LOG, "[MEMORY INFO] [%s]: %ld byte(s) of memory %s at address %p for %s\n", ACTION, SIZE, ACTION_EXT, ADDRESS, TYPE); \
  }

  #define LOGGING

  #define CREATE_LOG()  \
  { \
    char* LOG_FILE_NAME; \
    LOG_FILE_NAME = stringsJoin(__DATE__,__TIME__,'_'); \
    LOG_FILE_NAME = stringsJoin("ding", LOG_FILE_NAME,'_'); \
    LOG_FILE_NAME = stringsJoin(LOG_FILE_NAME,"log",'.'); \
    LOG_FILE_NAME = replace(LOG_FILE_NAME, ':', '_'); \
    LOG_FILE_NAME = replace(LOG_FILE_NAME, ' ', '_'); \
    LOG = fopen(LOG_FILE_NAME, "w");\
    free(LOG_FILE_NAME); \
  }

  #define CLOSE_LOG() fclose(LOG);

#endif

