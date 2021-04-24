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

#ifndef BOOLEAN_H
#define BOOLEAN_H
typedef enum boolean{True=1,False=0} boolean;
#endif

void printStr (unsigned char *msg);
void printBit (unsigned char *bit);
void printNum (unsigned char *num);
void printIp4 (unsigned char *ip4);
void printIp6 (unsigned char *ip6);
void printMac (unsigned char *mac);
void printHexDump (unsigned char* packet, char packet_len);

