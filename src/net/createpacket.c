#include "codes/packet.h"

#include "utils/string.h"

#include "net/members/all.h"
#include "net/createpacket.h"

#include "logger.h"

PacketObject* getPacket(short packet_type)
{
  PacketObject* packet = (PacketObject*) malloc(1*sizeof(PacketObject));
  packet->assmbld_packet_struct = (unsigned char*) malloc(1024);
  clearMemory(packet->assmbld_packet_struct, 1024);

  #if LOGGING_ENABLED(LOG_MEMORY) > 0
    MEMORY_LOGGING_UTIL("M\0", 1024, packet->assmbld_packet_struct, "Assembled Packet Structure");
    MEMORY_LOGGING_UTIL("M\0", sizeof(PacketObject), packet, "Packet Object");
  #endif

  switch(packet_type)
  {
    case __ARP:
    {
      packet->packet_type = packet_type;
      arp_payload_len = 0;
      struct arp_head *packet_head = (struct arp_head*) packet->assmbld_packet_struct;

      packet->head_len = ARP_HEAD_LEN;
      packet->tail_len = ARP_TAIL_LEN;
      packet->head = (unsigned char*) packet_head;
      packet->tail = NULL;
      packet->extra_opt_data = NULL;

      packet->payload = NULL;
      packet->payload_len = arp_payload_len;
      packet->packet_len = ARP_HEAD_LEN + ARP_TAIL_LEN + arp_payload_len;
      break;
    }

    case __ETHER:
    {
      packet->packet_type = packet_type;
      ether_payload_len = 0;

      struct ether_head *packet_head = (struct ether_head*) packet->assmbld_packet_struct;
      struct ether_tail *packet_tail = NULL; // (struct ether_tail*) malloc(sizeof(struct ether_tail));

      packet->head_len = ETH_HEAD_LEN;
      packet->tail_len = 0;
      packet->head = (unsigned char*) packet_head;
      packet->tail = (unsigned char*) packet_tail;
      packet->extra_opt_data = NULL;

      packet->payload = NULL;
      packet->payload_len = ether_payload_len;
      packet->packet_len = ETH_HEAD_LEN + ether_payload_len + ETH_TAIL_LEN;

      break;
    }

    case __IP4:
    {
      ip4_payload_len = 0;
      packet->packet_type = __IP4;

      struct ip4_head *packet_head = (struct ip4_head*) packet->assmbld_packet_struct;

      packet->head_len = IP4_HEAD_LEN;
      packet->tail_len = IP4_TAIL_LEN;
      packet->head = (unsigned char*) packet_head;
      packet->tail = NULL;
      packet->extra_opt_data = NULL;

      packet->payload_len = ip4_payload_len;

      packet->payload = NULL;
      packet->payload_len = ip4_payload_len;
      packet->packet_len = IP4_HEAD_LEN + IP4_TAIL_LEN + ip4_payload_len;
//      printf("ENDRA\n");
      break;
    }

    case __ICMP:
    {
      icmp_payload_len = 0;
      packet->packet_type = __ICMP;

      struct icmp_head *packet_head = (struct icmp_head*) packet->assmbld_packet_struct;

      packet->head_len = ICMP_HEAD_LEN;
      packet->tail_len = ICMP_TAIL_LEN;
      packet->head = (unsigned char*) packet_head;
      packet->tail = NULL;
      packet->extra_opt_data = NULL;

      packet->payload_len = icmp_payload_len;

      packet->payload = NULL;
      packet->payload_len = icmp_payload_len;
      packet->packet_len = ICMP_HEAD_LEN + ICMP_TAIL_LEN + icmp_payload_len;
      break;
    }

    default:
      fprintf(stderr, ERROR"Invalid Packet\n"RESET);
      #if LOGGING_ENABLED(LOG_MEMORY) > 0
        MEMORY_LOGGING_UTIL("F\0", 1024, packet->assmbld_packet_struct, "Assembled Packet Structure");
        MEMORY_LOGGING_UTIL("F\0", 0, packet, "Packet Object");
      #endif
      free(packet->assmbld_packet_struct);
      packet->assmbld_packet_struct = NULL;
      free(packet);
      packet = NULL;
      return NULL;
  }
  return packet;
}


