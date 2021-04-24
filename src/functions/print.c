#include "functions/print.h"

void printStr (unsigned char *msg)
{
  fprintf(stdout, USR "%s " RESET, msg);
}

void printNum (unsigned char *num)
{
  fprintf(stdout, USR "%ld " RESET,*((long int*) num));
  free(num);
}

void printBit (unsigned char *bit)
{
  fprintf(stdout, USR "%ld " RESET,*((long int*) bit));
  free(bit);
}

void printIp4 (unsigned char *ip4)
{
  fprintf(stdout, USR "%d.%d.%d.%d " RESET, *(ip4), *(ip4+1), *(ip4+2), *(ip4+3));
}

void printIp6 (unsigned char *ip6)
{
  fprintf(stdout, USR "%s " RESET,ip6);
}

void printMac (unsigned char *mac)
{
  fprintf(stdout, USR "%02x-%02x-%02x-%02x-%02x-%02x " RESET, *(mac), *(mac+1), *(mac+2), *(mac+3), *(mac+4), *(mac+5));
}

void printHexDump(unsigned char* packet, char packet_len)
{
  fprintf(stdout, HEXDUMP"\n      0x0 0x1 0x2 0x3 0x4 0x5 0x6 0x7    0x8 0x9 0xa 0xb 0xc 0xd 0xe 0xf\n"RESET);
  fprintf(stdout, "      --- --- --- --- --- --- --- ---    --- --- --- --- --- --- --- ---\n");
  int i=0,j=0;
  while(i<packet_len)
  {
    fprintf(stdout, HEXDUMP"0x%02x"RESET" | ",i);
    for(j=0;j<8 && i<packet_len;j++,i++)
      fprintf(stdout, "%02x  ",*(packet+i));
    fprintf(stdout, "   ");
    for(j=0;j<8 && i<packet_len;j++,i++)
      fprintf(stdout, "%02x  ",*(packet+i));
    fprintf(stdout, "\n");
  }
}

