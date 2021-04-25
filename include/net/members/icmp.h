#define ICMP_HEAD_LEN 8
#define ICMP_TAIL_LEN 0

short icmp_payload_len;

struct icmp_head
{
  unsigned char type[1];
  unsigned char code[1];
  unsigned char checksum[2];
  unsigned char data[4];
};

