#ifndef BOOLEAN_H
#define BOOLEAN_H
typedef enum boolean{True=1,False=0} boolean;
#endif

#ifndef NODE_H
#define NODE_H
#include "datastructures/node.h"
#endif

//--------------------------------------------------------------------//

////* VARIABLES *////

typedef struct
{
  char datatype;
  unsigned char *identifier;
  unsigned char *value;
  unsigned char length;
  Node* members;
}UserVariable;

////* VARIABLES *////

//--------------------------------------------------------------------//

////* PACKETS *////

typedef struct
{
  short packet_type;
  unsigned char* head;
  unsigned char* extra_opt_data;
  unsigned char* payload;
  unsigned char* tail;
  unsigned char* assmbld_packet_struct;
  short head_len;
  short extra_opt_data_len;
  short payload_len;
  short tail_len;
  short packet_len;
}PacketObject;

////* PACKETS *////

//--------------------------------------------------------------------//

////* TREES *////

BinarySearchTree* symbol_tree;
BinarySearchTree* packet_tree;
BinarySearchTree* datatype_tree;

BinarySearchTree* string_member_tree;

////* TREES *////

