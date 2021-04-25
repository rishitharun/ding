#define ETH_HEAD_LEN 14
#define ETH_TAIL_LEN 0

short ether_payload_len;

struct ether_head
{
  unsigned char dest_mac[6];
  unsigned char src_mac[6];
  unsigned char eth_type[2];
};

// SUPPORT FOR TAIL NOT YET IMPLEMENTED

struct ether_tail
{
  unsigned char crc[4];
};

