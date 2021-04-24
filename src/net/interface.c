#include "net/sockets.h"
#include "net/interface.h"

boolean getInterface(char* interface_name, short interface_name_str_len)
{
  if((interface=checkInterface(interface_name)) <= 0)
  {
    fprintf(stderr, ERROR "Invalid Interface <%s> !\n" RESET, interface_name);
    return False;
  }
  else {;}

  if(setsockopt(sock_desc,SOL_SOCKET,SO_BINDTODEVICE,interface_name,interface_name_str_len+1) < 0)
  {
    fprintf(stderr, ERROR"Cannot Bind to the socket <%s> !\n"RESET, interface_str);
    return False;
  }
  else {;}

  return True;
}

boolean checkInterface(char* interface)
{
  return if_nametoindex(interface);
}

