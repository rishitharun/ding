#ifndef STDIO_H
#define STDIO_H
#include<stdio.h>
#endif

#ifndef COLOR
#define COLOR
#include "codes/color.h"
#endif

short getMemberLength(short packet_member);
short getMemberOffset(short packet_member);
short getPacketLength(short packet_member);

