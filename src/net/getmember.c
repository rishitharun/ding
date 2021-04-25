#include "codes/packet.h"
#include "codes/datatype.h"

#include "net/getmember.h"
#include "net/members/all.h"

extern short allocd;
extern char** extracted_bit_values;

UserVariable* getMemberVariable(PacketObject* packet, short packet_member)
{
  UserVariable* member_variable = (UserVariable*) malloc(sizeof(UserVariable));

  member_variable->identifier = NULL;
  member_variable->datatype = 0;
  member_variable->value = NULL;
  member_variable->length = 0;

  switch(packet_member)
  {
    case __ARP_HW_TYPE:
      member_variable->datatype = NUMBER_TYPE_CODE;
      member_variable->value = (((struct arp_head*)(packet->head))->hw_type);
      member_variable->length = 2;
      break;

    case __ARP_PROTO_TYPE:
      member_variable->datatype = NUMBER_TYPE_CODE;
      member_variable->value = (((struct arp_head*)(packet->head))->protocol_type);
      member_variable->length = 2;
      break;

    case __ARP_HW_ADDRESS_LEN:
      member_variable->datatype = NUMBER_TYPE_CODE;
      member_variable->value = (((struct arp_head*)(packet->head))->hw_addr_len);
      member_variable->length = 1;
      break;

    case __ARP_PROTO_ADDRESS_LEN:
      member_variable->datatype = NUMBER_TYPE_CODE;
      member_variable->value = (((struct arp_head*)(packet->head))->protocol_addr_len);
      member_variable->length = 1;
      break;

    case __ARP_OPERATION:
      member_variable->datatype = NUMBER_TYPE_CODE;
      member_variable->value = (((struct arp_head*)(packet->head))->operation);
      member_variable->length = 2;
      break;

    case __ARP_SRC_MAC:
      member_variable->datatype = MAC_TYPE_CODE;
      member_variable->value = (((struct arp_head*)(packet->head))->src_mac);
      member_variable->length = 6;
      break;

    case __ARP_SRC_IP:
      member_variable->datatype = IP4_TYPE_CODE;
      member_variable->value = (((struct arp_head*)(packet->head))->src_ip);
      member_variable->length = 4;
      break;

    case __ARP_DEST_MAC:
      member_variable->datatype = MAC_TYPE_CODE;
      member_variable->value = (((struct arp_head*)(packet->head))->dest_mac);
      member_variable->length = 6;
      break;

    case __ARP_DEST_IP:
      member_variable->datatype = IP4_TYPE_CODE;
      member_variable->value = (((struct arp_head*)(packet->head))->dest_ip);
      member_variable->length = 4;
      break;

    case __ETHER_DEST_MAC:
      member_variable->datatype = MAC_TYPE_CODE;
      member_variable->value = (((struct ether_head*)(packet->head))->dest_mac);
      member_variable->length = 6;
      break;

    case __ETHER_SRC_MAC:
      member_variable->datatype = MAC_TYPE_CODE;
      member_variable->value = (((struct ether_head*)(packet->head))->dest_mac);
      member_variable->length = 6;
      break;

    case __ETHER_TYPE:
      member_variable->datatype = NUMBER_TYPE_CODE;
      member_variable->value = (((struct ether_head*)(packet->head))->eth_type);
      member_variable->length = 2;
      break;

    /* NEED TO TAKE A LOOK AT THIS LATER
    case __ETHER_CRC:
      member_variable->datatype = NUMBER_TYPE_CODE;
      member_variable->value = (((struct ether_tail*)(packet->tail))->crc);
      member_variable->length = 4;
      break;
    */
    case __IP4_VERSION:
      extracted_bit_values = (char**) realloc(extracted_bit_values, allocd+1);
      member_variable->datatype = BIT_TYPE_CODE;
      member_variable->value = (char*) malloc(sizeof(char));
      *(member_variable->value) = (0xf0 & *(((struct ip4_head*)(packet->head))->version_headerlen))>>4;
      member_variable->length = 1;
      *(extracted_bit_values+allocd++) = member_variable->value;
      break;

    case __IP4_HEADER_LEN:
      extracted_bit_values = (char**) realloc(extracted_bit_values, allocd+1);
      member_variable->datatype = BIT_TYPE_CODE;
      member_variable->value = (char*) malloc(sizeof(char));
      *(member_variable->value) = 0x0f & *(((struct ip4_head*)(packet->head))->version_headerlen);
      member_variable->length = 1;
      *(extracted_bit_values+allocd++) = member_variable->value;
      break;

    case __IP4_TYPE_OF_SERVICE:
      member_variable->datatype = NUMBER_TYPE_CODE;
      member_variable->value = (((struct ip4_head*)(packet->head))->type_of_service);
      member_variable->length = 1;
      break;

    case __IP4_TOTAL_LEN:
      member_variable->datatype = NUMBER_TYPE_CODE;
      member_variable->value = (((struct ip4_head*)(packet->head))->total_length);
      member_variable->length = 2;
      break;

    case __IP4_IDENTIFICATION:
      member_variable->datatype = NUMBER_TYPE_CODE;
      member_variable->value = (((struct ip4_head*)(packet->head))->identification);
      member_variable->length = 2;
      break;

    case __IP4_FLAGS:
      extracted_bit_values = (char**) realloc(extracted_bit_values, allocd+1);
      member_variable->datatype = BIT_TYPE_CODE;
      member_variable->value = (char*) malloc(sizeof(char));
      *(member_variable->value) = (0xe0 & *(((struct ip4_head*)(packet->head))->flags_fragoff)) >> 5;
      member_variable->length = 1;
      *(extracted_bit_values+allocd++) = member_variable->value;
      break;

    case __IP4_FRAG_OFFSET:
      extracted_bit_values = (char**) realloc(extracted_bit_values, allocd+1);
      member_variable->datatype = BIT_TYPE_CODE;
      member_variable->value = (char*) malloc(2*sizeof(char));
      *(member_variable->value+0) = (0x1f & *((char*)((struct ip4_head*)(packet->head))->flags_fragoff+0));
      *(member_variable->value+1) = *((char*)((struct ip4_head*)(packet->head))->flags_fragoff+1);
      member_variable->length = 2;
      *(extracted_bit_values+allocd++) = member_variable->value;
      break;

    case __IP4_TTL:
      member_variable->datatype = NUMBER_TYPE_CODE;
      member_variable->value = (((struct ip4_head*)(packet->head))->time_to_live);
      member_variable->length = 1;
      break;

    case __IP4_PROTOCOL:
      member_variable->datatype = NUMBER_TYPE_CODE;
      member_variable->value = (((struct ip4_head*)(packet->head))->protocol);
      member_variable->length = 1;
      break;

    case __IP4_CHECKSUM:
      member_variable->datatype = NUMBER_TYPE_CODE;
      member_variable->value = (((struct ip4_head*)(packet->head))->header_chksum);
      member_variable->length = 2;
      break;

    case __IP4_SRC_IP:
      member_variable->datatype = IP4_TYPE_CODE;
      member_variable->value = (((struct ip4_head*)(packet->head))->src_ip);
      member_variable->length = 4;
      break;

    case __IP4_DEST_IP:
      member_variable->datatype = IP4_TYPE_CODE;
      member_variable->value = (((struct ip4_head*)(packet->head))->dest_ip);
      member_variable->length = 4;
      break;

    case __ICMP_TYPE:
      member_variable->datatype = NUMBER_TYPE_CODE;
      member_variable->value = (((struct icmp_head*)(packet->head))->type);
      member_variable->length = 1;
      break;

    case __ICMP_CODE:
      member_variable->datatype = NUMBER_TYPE_CODE;
      member_variable->value = (((struct icmp_head*)(packet->head))->code);
      member_variable->length = 1;
      break;

    case __ICMP_CHECKSUM:
      member_variable->datatype = NUMBER_TYPE_CODE;
      member_variable->value = (((struct icmp_head*)(packet->head))->checksum);
      member_variable->length = 2;
      break;

    case __ICMP_DATA:
      member_variable->datatype = NUMBER_TYPE_CODE;
      member_variable->value = (((struct icmp_head*)(packet->head))->data);
      member_variable->length = 4;
      break;

    case __ARP_PAYLOAD:
    case __ETHER_PAYLOAD:
    case __IP4_PAYLOAD:
    case __ICMP_PAYLOAD:
      member_variable->datatype = STREAM_TYPE_CODE;
      member_variable->value = (packet->payload);
      member_variable->length = packet->payload_len;
      break;

    default:
      fprintf(stderr, ERROR"Invalid Member in GetMem\n"RESET);
      free(member_variable);
      return NULL;
  }
  return member_variable;
}

