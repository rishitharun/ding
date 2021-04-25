
#define ARP_HEAD_LEN 28
#define ARP_TAIL_LEN 0

short arp_payload_len;

struct arp_head
{
  unsigned char hw_type[2];
  unsigned char protocol_type[2];
  unsigned char hw_addr_len[1];
  unsigned char protocol_addr_len[1];
  unsigned char operation[2];
  unsigned char src_mac[6];
  unsigned char src_ip[4];
  unsigned char dest_mac[6];
  unsigned char dest_ip[4];
};

