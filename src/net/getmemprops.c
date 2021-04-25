#include "codes/packet.h"

#include "net/getmemprops.h"

short getPacketLength(short packet_member)
{
  switch(packet_member)
  {
    case __ARP:
      return 28;

    case __ETHER:
      return 14;

    case __IP4:
      return 20;

    case __ICMP:
      return 8;

    default:
      fprintf(stderr, ERROR"Invalid Packet\n"RESET);
      return -1;
  }
}

short getMemberLength(short packet_member)
{
  switch(packet_member)
  {
    case __ARP_HW_ADDRESS_LEN:
    case __ARP_PROTO_ADDRESS_LEN:
    case __IP4_FLAGS:
    case __IP4_VERSION:
    case __IP4_HEADER_LEN:
    case __IP4_TYPE_OF_SERVICE:
    case __IP4_TTL:
    case __IP4_PROTOCOL:
    case __ICMP_TYPE:
    case __ICMP_CODE:
      return 1;

    case __ARP_HW_TYPE:
    case __ARP_PROTO_TYPE:
    case __ARP_OPERATION:
    case __IP4_TOTAL_LEN:
    case __IP4_IDENTIFICATION:
    case __IP4_FRAG_OFFSET:
    case __IP4_CHECKSUM:
    case __ETHER_TYPE:
    case __ICMP_CHECKSUM:
      return 2;

    case __ARP_SRC_IP:
    case __ARP_DEST_IP:
    case __IP4_SRC_IP:
    case __IP4_DEST_IP:
    //case __ETHER_CRC: NEED TO TAKE A LOOK AT THIS LATER
    case __ICMP_DATA:
      return 4;

    case __ARP_SRC_MAC:
    case __ARP_DEST_MAC:
    case __ETHER_DEST_MAC:
    case __ETHER_SRC_MAC:
      return 6;

    case __ARP_PAYLOAD:
    case __ETHER_PAYLOAD:
    case __IP4_PAYLOAD:
    case __ICMP_PAYLOAD:
      return 0;

    default:
      fprintf(stderr, ERROR"Invalid Member\n"RESET);
      return -1;
  }
}

short getMemberOffset(short packet_member)
{
  switch(packet_member)
  {
    case __ARP_HW_TYPE:
      return 0;

    case __ARP_PROTO_TYPE:
      return 2;

    case __ARP_HW_ADDRESS_LEN:
      return 4;

    case __ARP_PROTO_ADDRESS_LEN:
      return 5;

    case __ARP_OPERATION:
      return 6;

    case __ARP_SRC_MAC:
      return 8;

    case __ARP_SRC_IP:
      return 14;

    case __ARP_DEST_MAC:
      return 18;

    case __ARP_DEST_IP:
      return 24;

    case __ARP_PAYLOAD:
      return 28;

    case __ETHER_DEST_MAC:
      return 0;

    case __ETHER_SRC_MAC:
      return 6;

    case __ETHER_TYPE:
      return 12;

    case __ETHER_PAYLOAD:
      return 14;

    case __IP4_VERSION:
      return 0;

    case __IP4_HEADER_LEN:
      return 0;

    case __IP4_TYPE_OF_SERVICE:
      return 1;

    case __IP4_TOTAL_LEN:
      return 2;

    case __IP4_IDENTIFICATION:
      return 4;

    case __IP4_FLAGS:
      return 6;

    case __IP4_FRAG_OFFSET:
      return 6;

    case __IP4_TTL:
      return 8;

    case __IP4_PROTOCOL:
      return 9;

    case __IP4_CHECKSUM:
      return 10;

    case __IP4_SRC_IP:
      return 12;

    case __IP4_DEST_IP:
      return 16;

    case __IP4_PAYLOAD:
      return 20;

    case __ICMP_TYPE:
      return 0;

    case __ICMP_CODE:
      return 1;

    case __ICMP_CHECKSUM:
      return 2;

    case __ICMP_DATA:
      return 4;

    case __ICMP_PAYLOAD:
      return 8;


    default:
      fprintf(stderr, ERROR"Invalid Member encountered while getting props\n"RESET);
      return -1;
  }
}

