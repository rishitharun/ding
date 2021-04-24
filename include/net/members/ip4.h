
#define IP4_HEAD_LEN 20
#define IP4_TAIL_LEN 0
#define IP4_OPTN_LEN 2

short ip4_payload_len;

struct ip4_head
{
  // BITS 4 & 4
  unsigned char version_headerlen[1];

  unsigned char type_of_service[1];
  unsigned char total_length[2];
  unsigned char identification[2];

  // BITS 3 & 13
  unsigned short flags_fragoff[1];

  unsigned char time_to_live[1];
  unsigned char protocol[1];
  unsigned char header_chksum[2];
  unsigned char src_ip[4];
  unsigned char dest_ip[4];
};

struct ip4_options
{
  struct
  {
    unsigned char copied:1;
    unsigned char option_class:2;
    unsigned char option_number:5;
  } option_params;
  
  unsigned char option_length[1];
  unsigned char* option_data;
};

