#include "net/sockets.h"

boolean createSocket()
{
  sock_desc = socket(AF_PACKET,SOCK_RAW,htons(0x0003));
  
  if (sock_desc == -1) { fprintf(stderr, ERROR "Socket Creation Failed ! Please run as a root user!\n" RESET); return False; }
  else {;}

  return True;
}

boolean destroySocket()
{
  close(sock_desc);
  return True;
}
