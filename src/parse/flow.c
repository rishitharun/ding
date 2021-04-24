/* THIS IS WHERE ALL THE MAGIC HAPPENS */

#include "parse/flow.h"
#include "memory/allocate.h"

#include "codes/flag.h"
#include "codes/node.h"
#include "codes/token.h"
#include "codes/datatype.h"

#include "net/getmember.h"
#include "net/setmember.h"
#include "net/getmemprops.h"

#include "datastructures/bst.h"
#include "datastructures/ntree.h"
#include "datastructures/stack.h"
#include "datastructures/buffer.h"

#include "functions/operations.h"

#include "logger.h"

static char flag;
static short buffer_ind;
static boolean null_node, isMemb;
static PacketObject* temp_packet;

// BIT TYPE MEMBER SPECIFIC
short allocd;
short* bit_specific;
char** extracted_bit_values;
// BIT TYPE MEMBER SPECIFIC

// CONDITION STRING SPECIFICS
short cstr_offset, cstr_default_len;
short *start_end;
// CONDITION STRING SPECIFICS

static void resetFlagsAndCounters()
{
  buffer_ind = flag = 0;
  null_node = isMemb = False;
}

static UserVariable* getNewUVObject(char type, char* value, char length, Node* member)
{
  UserVariable *new_UVObj = (UserVariable*) malloc(sizeof(UserVariable));
  new_UVObj->datatype = type;
  new_UVObj->identifier = NULL;
  new_UVObj->value = value;
  new_UVObj->length = length;
  new_UVObj->members = member;
  return new_UVObj;
}

static Node* getRawVariable(UserVariable* data)
{
  Node *new_node = (Node*) malloc(sizeof(Node));
  new_node->key = NULL;
  new_node->type = RAW_VARIABLE_NODE;
  new_node->data = (char*)data;
  return new_node;
}

static Node* allocCondStr(char* cstr_buff, char len)
{
  char* cstr_val = (char*) malloc(sizeof(char)*len);
  short ind = 0;
  for(;ind<len;ind++) *(cstr_val+ind) = *(cstr_buff+ind);
  return getRawVariable(getNewUVObject(STREAM_TYPE_CODE, cstr_val, len, NULL));
}

static Node* allocate(char* word, char flag)
{
  char *len = malloc(1);
  unsigned char *val = NULL;
  Node* raw_node = NULL;
  switch(flag)
  {
    case IP4_TRUE_FLAG:
      val = allocateIP4(word,len);
      if(val == NULL) { free(len); return NULL; }
      else {;}
      raw_node = getRawVariable(getNewUVObject(IP4_TYPE_CODE, val, 4, NULL));
      break;
    case MAC_TRUE_FLAG:
      val = allocateMAC(word,len);
      if(val == NULL) { free(len); return NULL; }
      else {;}
      raw_node = getRawVariable(getNewUVObject(MAC_TYPE_CODE, val, 6, NULL));
      break;
    case DQUOTE_TOKEN:
      val = allocateString(word,len);
      raw_node = getRawVariable(getNewUVObject(STRING_TYPE_CODE, val, *len, string_member_tree));
      break;
    case COLON_TOKEN:
    case HEXSTREAM_FLAG:
      val = allocateStream(word,len);
      if(val == NULL) { free(len); return NULL; }
      else {;}
      raw_node = getRawVariable(getNewUVObject(STREAM_TYPE_CODE, val, *len, NULL));
      break;
    case BIT_FLAG:
      val = allocateBit(word,len);
      if(val == NULL) { free(len); return NULL; }
      else {;}
      raw_node = getRawVariable(getNewUVObject(BIT_TYPE_CODE, val, *len, NULL));
      break;
    case NUMBER_FLAG:
      val = allocateNumber(word,len);
      if(val == NULL) { free(len); return NULL; }
      else {;}
      raw_node = getRawVariable(getNewUVObject(NUMBER_TYPE_CODE, val, *len, NULL));
      break;
    case PERIOD_FLAG:
      fprintf(stderr, ERROR"Invalid packet membership access !\nUndeclared Packet !\n"RESET);
      free(len);
      return NULL;
    default:
      free(len);
      return NULL;
  }
  free(len);
  return raw_node;
}

static boolean evaluate(Node* root, Stack* exec_stack)
{
  if(root == NULL) return True;
  else {;}
  char node_type = getNodeType(root);
  switch(node_type)
  {
    case EQUAL_NODE:
      if(!evaluate(root->left, exec_stack)) return False;
      else {;}
      if(!evaluate(root->right, exec_stack)) return False;
      else {;}

      Node* rhs_node = (Node*) popStack(exec_stack);
      Node* lhs_node = (Node*) popStack(exec_stack);

      if(rhs_node == NULL) fprintf(stderr, DEBUG_B"[DEBUG INFO]:"RESET " RHS not found in Stack !\n");
      else {;}
      if(lhs_node == NULL) fprintf(stderr, DEBUG_B"[DEBUG INFO]:"RESET " LHS not found in Stack !\n");
      else {;}

      if(((Node*)(lhs_node->data))->type == MEMBER_NODE)
      {
        UserVariable* rhs = (UserVariable*) (((Node*)(rhs_node->data))->data);
        UserVariable* member_variable = (UserVariable*)((Node*)(lhs_node->data))->data;
        if((member_variable->datatype == rhs->datatype)||(member_variable->datatype == STREAM_TYPE_CODE)||(member_variable->datatype != STRING_TYPE_CODE && rhs->datatype == STREAM_TYPE_CODE)){;}
        else {fprintf(stderr, ERROR"Incompatible types for lhs and rhs !\nCannot Assign !\n"RESET); return False;}

        if(member_variable->length == rhs->length) {;}
        else if(member_variable->length == 0)
        {
          char* rhs_value;
          short rhs_length;
          if(rhs->datatype == PACKET_TYPE_CODE)
          {
            PacketObject* packet_object = (PacketObject*) rhs->value;
//            assemblePacket(packet_object);
            rhs_value = packet_object->assmbld_packet_struct;
            rhs_length = packet_object->packet_len;
          }
          else
          {
            rhs_value = rhs->value;
            rhs_length = rhs->length;
          }
          // VAR SIZED MEMBERS - PAYLOAD DONE... DO OPTIONS
          char i =0;
          temp_packet->payload_len = member_variable->length = rhs_length;
          temp_packet->packet_len = temp_packet->payload_len + temp_packet->head_len + temp_packet->tail_len;
          char* mem_payload = (temp_packet->assmbld_packet_struct)+temp_packet->head_len;
//          *mem_payload = ;
          for(;i<member_variable->length; i++) *(mem_payload+i) = *(rhs_value+i);
          goto assignEnd;
        }
        else
        {
          fprintf(stderr, ERROR"Incompatible variable sizes for lhs and rhs !\n"RESET);
          fprintf(stderr, ERROR"lhs requires <%d> bytes, but rhs size is <%d> bytes !\n"RESET,member_variable->length, rhs->length);
          fprintf(stderr, ERROR"Cannot Assign !\n"RESET);
          return False;
        }

        if(member_variable->datatype == BIT_TYPE_CODE)
        {
          if(member_variable->length == 1)
          {
            *((member_variable->value)+*((char*)bit_specific+0))|=(*(rhs->value)<<*((char*)bit_specific+1));
          }
          else if(member_variable->length == 2)
          {
            *(member_variable->value+0)|=(*(rhs->value+0)<<*((char*)bit_specific+1));
            *(member_variable->value+1)|=(*(rhs->value+1)<<*((char*)bit_specific+1));
          }
          else {;}
          goto assignEnd;
        }
        else {;}

        char i, *member_value;
        i=0; member_value = member_variable->value;
        for(;i<member_variable->length; i++) *(member_value+i) = *((rhs->value)+i);
        // try freeing mem nodes
        goto assignEnd;
      }
      else {;}
      UserVariable* rhs = (UserVariable*) (((Node*)(rhs_node->data))->data);
      UserVariable* lhs = (UserVariable*) (((Node*)(lhs_node->data))->data);

      if((lhs->datatype == rhs->datatype) || (lhs->datatype == STREAM_TYPE_CODE) || (lhs->datatype != STRING_TYPE_CODE && rhs->datatype == STREAM_TYPE_CODE)) {;}
      else {fprintf(stderr, ERROR"Incompatible types !\nCannot Assign !\n"RESET); return False;}

      if(!lhs->length) { free(lhs->value); lhs->value = NULL; }
      else {;}

      if(rhs->identifier == NULL) lhs->value = rhs->value;
      else
      {
        lhs->value = (char*) malloc(rhs->length); 
        short i=0;
        for(;i<rhs->length;i++) *((lhs->value)+i) = *((rhs->value)+i);
      }

      lhs->length = rhs->length;
      lhs->members = rhs->members;
      assignEnd:;
      break;

    case VARIABLE_NODE:
    case RAW_VARIABLE_NODE:
    case MEMBER_NODE:
      pushStack(exec_stack, (char*) root);
      if(!evaluate(root->right,exec_stack)) return False;
      else {;}
      break;

    case LESSER_NODE:
    case GREATER_NODE:
    case PLUS_NODE:
    case MINUS_NODE:
    case FUNCTION_NODE:
      pushStack(exec_stack, (char*) root);
      root = root->left;
      while(root != NULL)
      {
        if (getNodeType(root) == FUNCTION_NODE || getNodeType(root) == PLUS_NODE || getNodeType(root) == MINUS_NODE || getNodeType(root) == LESSER_NODE || getNodeType(root) == GREATER_NODE)
        {
          if(!evaluate(root, exec_stack)) return False;
          else {;}
          Node* evaluated_node = (Node*) popStack(exec_stack);

          if(evaluated_node == NULL) fprintf(stderr, DEBUG_B"[DEBUG INFO]:"RESET " FUNCTION RETURN VALUE not found in Stack !\n");
          else {;}

          root->data = evaluated_node->data;
          free(evaluated_node);
          evaluated_node = NULL;
          root = root->right;
        }
        else root = root->right;
      }
      Node* node = (Node*) popStack(exec_stack);

      if(node == NULL) fprintf(stderr, DEBUG_B"[DEBUG INFO]:"RESET " FUNCTION NODE not found in Stack !\n");
      else {;}

      UserVariable* return_var;
      UserVariable* (*func) (Node* args);

      switch(getNodeType(node))
      {
        case FUNCTION_NODE:
          func = (UserVariable* (*)(Node* args)) (((Node*)(node->data))->data);
          break;
        case PLUS_NODE:
          func = &add;
          break;
        case MINUS_NODE:
          func = &subtract;
          break;
        case GREATER_NODE:
          func = &greaterthan;
          break;
        case LESSER_NODE:
          func = &lessthan;
          break;
        default:
          fprintf(stderr, DEBUG_B"[DEBUG INFO]:"RESET " Invalid Operator Node <%d> in evaluate()\n", getNodeType(node));
          return False;
      }
      return_var = func(node->left);

      if(return_var == NULL) return False;
      else {;}

      pushStack(exec_stack, (char*) createNode(NULL, 0, (char*) getRawVariable(return_var)));

      break;
  }
  return True;
}

boolean parseFlow(char *line, short line_len)
{
  #if LOGGING_ENABLED(LOG_GENERAL)
    GEN_LOGGING_UTIL1("%s", line);
  #endif

  short *index, i, cstr_buff_ind;
  Node* line_tree;
  Stack* line_stack;
  Buffer *bufferObj, *cstr;
  BinarySearchTree* current;
  boolean isString, ctoken, status, isCondStr, operation, escape;

  index = (short*) malloc(sizeof(short));
  start_end = (short*) malloc(sizeof(short));
  bit_specific = (short*) malloc(sizeof(short));
  extracted_bit_values = (char**) malloc(sizeof(char*));

  line_tree = NULL;
  current = symbol_tree;
  *index = buffer_ind = flag = allocd = cstr_offset = cstr_buff_ind = 0;

  line_stack = initDynamicStack();

  ctoken = null_node = isString = isMemb = isCondStr = operation = False;

  bufferObj = createBuffer();
  cstr = createBuffer();
  char* buffer = bufferObj->buffer;

  i=-1;
  while(i<line_len)
  {
    i++;

    switch(*(line+i))
    {
      // SEPARATOR TOKENS //

      case BW_SLASH_TOKEN:
        if((isCondStr || isString) && *(line+i+1) == DQUOTE_TOKEN) escape = True;
        else {;}
        break;

      case PIPE_TOKEN:
        continue;

      case TABSPACE_TOKEN:
        if(!isString) continue;
        else {;}
        break;

      case WHITESPACE_TOKEN:
        if(!isString) continue;
        else {;}
        break;

      case GREATER_TOKEN:
      case LESSER_TOKEN:
      case PLUS_TOKEN:
      case MINUS_TOKEN:
        if(isString) break;
        else {;}

        if(ctoken) continue;
        else ctoken = True;

        if(!null_node)
        {
          if(!*((current->key)+(*index)))
          {
            if(line_tree == NULL)
            {
              switch(*(line+i))
              {
                case GREATER_TOKEN:;
                  line_tree = createNode(NULL, 0, (char*) createNode(">\0", GREATER_NODE, NULL));
                  pushStack(line_stack, (char*) line_tree);
                  insertNodeNTree(line_tree, createNode(NULL, 0, (char*) current));
                  operation = True;
                  break;
                case LESSER_TOKEN:;
                  line_tree = createNode(NULL, 0, (char*) createNode("<\0", LESSER_NODE, NULL));
                  pushStack(line_stack, (char*) line_tree);
                  insertNodeNTree(line_tree, createNode(NULL, 0, (char*) current));
                  operation = True;
                  break;
                case PLUS_TOKEN:;
                  line_tree = createNode(NULL, 0, (char*) createNode("+\0", PLUS_NODE, NULL));
                  pushStack(line_stack, (char*) line_tree);
                  insertNodeNTree(line_tree, createNode(NULL, 0, (char*) current));
                  operation = True;
                  break;
                case MINUS_TOKEN:;
                  line_tree = createNode(NULL, 0, (char*) createNode("-\0", MINUS_NODE, NULL));
                  pushStack(line_stack, (char*) line_tree);
                  insertNodeNTree(line_tree, createNode(NULL, 0, (char*) current));
                  operation = True;
                  break;
              }
            }
            else
            {
              switch(*(line+i))
              {
                case GREATER_TOKEN:;
                  Node* greater_node = createNode(NULL, 0, (char*) createNode(">\0", GREATER_NODE, NULL));
                  insertNodeNTree(line_tree, greater_node);
                  pushStack(line_stack, (char*) line_tree);
                  line_tree = greater_node;
                  insertNodeNTree(line_tree, createNode(NULL, 0, (char*) current));
                  operation = True;
                  break;
                case LESSER_TOKEN:;
                  Node* lesser_node = createNode(NULL, 0, (char*) createNode("<\0", LESSER_NODE, NULL));
                  insertNodeNTree(line_tree, lesser_node);
                  pushStack(line_stack, (char*) line_tree);
                  line_tree = lesser_node;
                  insertNodeNTree(line_tree, createNode(NULL, 0, (char*) current));
                  operation = True;
                  break;
                case PLUS_TOKEN:;
                  Node* plus_node = createNode(NULL, 0, (char*) createNode("+\0", PLUS_NODE, NULL));
                  insertNodeNTree(line_tree, plus_node);
                  pushStack(line_stack, (char*) line_tree);
                  line_tree = plus_node;
                  insertNodeNTree(line_tree, createNode(NULL, 0, (char*) current));
                  operation = True;
                  break;
                case MINUS_TOKEN:;
                  Node* minus_node = createNode(NULL, 0, (char*) createNode("-\0", MINUS_NODE, NULL));
                  insertNodeNTree(line_tree, minus_node);
                  pushStack(line_stack, (char*) line_tree);
                  line_tree = minus_node;
                  insertNodeNTree(line_tree, createNode(NULL, 0, (char*) current));
                  operation = True;
                  break;
              }
            }

            resetFlagsAndCounters();
            *index = 0;
            current = symbol_tree;
            continue;
          }
          else
          {
            while(True)
            {
              current = getNode(current, 0, index);
              if (current == NULL)
              {
                fprintf(stderr, ERROR"Undeclared variable <%s> !\n"RESET, buffer);
                status = False;
                goto deallocate_and_exit;
              }
              else if(!*((current->key)+(*index)))
              {
                if(line_tree == NULL)
                {
                  switch(*(line+i))
                  {
                    case GREATER_TOKEN:;
                      line_tree = createNode(NULL, 0, (char*) createNode(">\0", GREATER_NODE, NULL));
                      pushStack(line_stack, (char*) line_tree);
                      insertNodeNTree(line_tree, createNode(NULL, 0, (char*) current));
                      operation = True;
                      break;
                    case LESSER_TOKEN:;
                      line_tree = createNode(NULL, 0, (char*) createNode("<\0", LESSER_NODE, NULL));
                      pushStack(line_stack, (char*) line_tree);
                      insertNodeNTree(line_tree, createNode(NULL, 0, (char*) current));
                      operation = True;
                      break;
                    case PLUS_TOKEN:;
                      line_tree = createNode(NULL, 0, (char*) createNode("+\0", PLUS_NODE, NULL));
                      pushStack(line_stack, (char*) line_tree);
                      insertNodeNTree(line_tree, createNode(NULL, 0, (char*) current));
                      operation = True;
                      break;
                    case MINUS_TOKEN:;
                      line_tree = createNode(NULL, 0, (char*) createNode("-\0", MINUS_NODE, NULL));
                      pushStack(line_stack, (char*) line_tree);
                      insertNodeNTree(line_tree, createNode(NULL, 0, (char*) current));
                      operation = True;
                      break;
                  }
                }
                else
                {
                  switch(*(line+i))
                  {
                    case GREATER_TOKEN:;
                      Node* greater_node = createNode(NULL, 0, (char*) createNode(">\0", GREATER_NODE, NULL));
                      insertNodeNTree(line_tree, greater_node);
                      pushStack(line_stack, (char*) line_tree);
                      line_tree = greater_node;
                      insertNodeNTree(line_tree, createNode(NULL, 0, (char*) current));
                      operation = True;
                      break;
                    case LESSER_TOKEN:;
                      Node* lesser_node = createNode(NULL, 0, (char*) createNode("<\0", LESSER_NODE, NULL));
                      insertNodeNTree(line_tree, lesser_node);
                      pushStack(line_stack, (char*) line_tree);
                      line_tree = lesser_node;
                      insertNodeNTree(line_tree, createNode(NULL, 0, (char*) current));
                      operation = True;
                      break;
                    case PLUS_TOKEN:;
                      Node* plus_node = createNode(NULL, 0, (char*) createNode("+\0", PLUS_NODE, NULL));
                      insertNodeNTree(line_tree, plus_node);
                      pushStack(line_stack, (char*) line_tree);
                      line_tree = plus_node;
                      insertNodeNTree(line_tree, createNode(NULL, 0, (char*) current));
                      operation = True;
                      break;
                    case MINUS_TOKEN:;
                      Node* minus_node = createNode(NULL, 0, (char*) createNode("-\0", MINUS_NODE, NULL));
                      insertNodeNTree(line_tree, minus_node);
                      pushStack(line_stack, (char*) line_tree);
                      line_tree = minus_node;
                      insertNodeNTree(line_tree, createNode(NULL, 0, (char*) current));
                      operation = True;
                      break;
                  }
                }
                resetFlagsAndCounters();
                *index = 0;
                current = symbol_tree;
                break;
              }
              else {;}
            }
            continue;
          }
        }
        else
        {
          if(isMemb)
          {
            fprintf(stderr, ERROR"Invalid Member Name <%s> !\n"RESET, buffer); 

            status = False;
            goto deallocate_and_exit;
          }
          else {;}
          Node* raw_var = allocate(buffer, flag);

          if(raw_var == NULL)
          {
            fprintf(stderr, ERROR"Undeclared variable <%s> !\n"RESET,buffer);

            status = False;
            goto deallocate_and_exit;
          }
          else {;}
          switch(*(line+i))
          {
            case GREATER_TOKEN:;
              Node* greater_node = createNode(NULL, 0, (char*) createNode(">\0", GREATER_NODE, NULL));
              insertNodeNTree(line_tree, greater_node);
              pushStack(line_stack, (char*) line_tree);
              line_tree = greater_node;
              insertNodeNTree(line_tree, createNode(NULL, 0, (char*) raw_var));
              operation = True;
              break;
            case LESSER_TOKEN:;
              Node* lesser_node = createNode(NULL, 0, (char*) createNode("<\0", LESSER_NODE, NULL));
              insertNodeNTree(line_tree, lesser_node);
              pushStack(line_stack, (char*) line_tree);
              line_tree = lesser_node;
              insertNodeNTree(line_tree, createNode(NULL, 0, (char*) raw_var));
              operation = True;
              break;
            case PLUS_TOKEN:;
              Node* plus_node = createNode(NULL, 0, (char*) createNode("+\0", PLUS_NODE, NULL));
              insertNodeNTree(line_tree, plus_node);
              pushStack(line_stack, (char*) line_tree);
              line_tree = plus_node;
              insertNodeNTree(line_tree, createNode(NULL, 0, (char*) raw_var));
              operation = True;
              break;
            case MINUS_TOKEN:;
              Node* minus_node = createNode(NULL, 0, (char*) createNode("-\0", MINUS_NODE, NULL));
              insertNodeNTree(line_tree, minus_node);
              pushStack(line_stack, (char*) line_tree);
              line_tree = minus_node;
              insertNodeNTree(line_tree, createNode(NULL, 0, (char*) raw_var));
              operation = True;
              break;
          }
          resetFlagsAndCounters();
          *index = 0;
          current = symbol_tree;
          continue;
        }
        break;

      case EQUAL_TOKEN:
        if(!*(line+i+1))
        {
          fprintf(stderr, ERROR"Incomplete line !\nEnd of line encountered without rhs !\n"RESET);

          status = False;
          goto deallocate_and_exit;
        }
        else {;}

///////////// CSTR SPEC ////////////////////
        if(isCondStr)
        {
          if(!null_node)
          {
            if(!*((current->key)+(*index)))
            {
              cstr_default_len = getMemberLength(current->type);
              *start_end = cstr_offset + getMemberOffset(current->type);
              readToBuffer(cstr,*(((char*)start_end)+0), cstr_buff_ind++);
              readToBuffer(cstr,*(((char*)start_end)+1), cstr_buff_ind++);
              *start_end += cstr_default_len;
              readToBuffer(cstr,*(((char*)start_end)+0), cstr_buff_ind++);
              readToBuffer(cstr,*(((char*)start_end)+1), cstr_buff_ind++);

              current = symbol_tree;
              //pushStack(line_stack, (char*) current); need to be somethin
              resetFlagsAndCounters();
              *index = 0;
              continue;
            }
//ISSUE
            else
            {
              fprintf(stderr,ERROR"Invalid Conditional String !\n<%s> is not a valid member !\n"RESET, buffer);
              status = False;
              goto deallocate_and_exit;
            }
          }
          else
          {
            fprintf(stderr,ERROR"Invalid Conditional String !\n<%s> is not a valid member !\n"RESET, buffer);
            status = False;
            goto deallocate_and_exit;
          }
        }
        else {;}

///////////// CSTR SPEC ////////////////////

        if(isString) break;
        else {;}
        if(!null_node)
        {
          if(!*((current->key)+(*index)))
          {
            if(isMemb)
            {
              Node* mem_variable = (Node*) popStack(line_stack);

              if(mem_variable == NULL) fprintf(stderr, DEBUG_B"[DEBUG INFO]:"RESET " PACKET -1 not found in Stack !\n");
              else {;}

              UserVariable* packet_variable = (UserVariable*) mem_variable->data;

              PacketObject* current_packet = (PacketObject*) packet_variable->value;
//              assmbld_packet = current_packet
              temp_packet = current_packet;
              UserVariable* member_variable = makeMemberVariable(current_packet, current->type, (char*) bit_specific);
              if (member_variable == NULL)
              {
                fprintf(stderr, ERROR"Undeclared variable <%s> !\n"RESET,buffer);

                status = False;
                goto deallocate_and_exit;
              }
              else {;}
              line_tree = createNode(NULL, 0, (char*) createNode("=\0", EQUAL_NODE, NULL));
              insertNodeNTree(line_tree, createNode(NULL, 0, (char*) createNode(NULL, MEMBER_NODE, (char*) member_variable)));

              resetFlagsAndCounters();
              current = symbol_tree;
              *index =0;
              continue;
            }
            else {;}

            if(line_tree == NULL)
            {
              line_tree = createNode(NULL, 0, (char*) createNode("=\0", EQUAL_NODE, NULL));
              insertNodeNTree(line_tree, createNode(NULL, 0, (char*) current));
            }
            else{;}

            resetFlagsAndCounters();
            *index = 0;
            current = symbol_tree;
            continue;
          }
          else
          {
            while(True)
            {
              current = getNode(current, 0, index);
              if (current == NULL)
              {
                fprintf(stderr, ERROR"Undeclared variable <%s> !\n"RESET, buffer);

                status = False;
                goto deallocate_and_exit;
              }
              else if(!*((current->key)+(*index)))
              {
                if(line_tree == NULL)
                {
                  line_tree = createNode(NULL, 0, (char*) createNode("=\0", EQUAL_NODE, NULL));
                  insertNodeNTree(line_tree, createNode(NULL, 0, (char*) current));
                }
                else {;}

                resetFlagsAndCounters();
                *index = 0;
                current = symbol_tree;
                break;
              }
              else {;}
            }
            continue;
          }
        }
        else
        {
          if(isMemb) fprintf(stderr, ERROR"Invalid Member Name <%s> !\n"RESET, buffer);
          else fprintf(stderr, ERROR"Undeclared variable <%s> !\n"RESET, buffer);

          status = False;
          goto deallocate_and_exit;
        }
        break;

      case OPEN_PARANTH_TOKEN:

///////////// CSTR SPEC ////////////////////

        if(isCondStr)
        {
          if(!null_node)
          {
            if(!*((current->key)+(*index)))
            {
              pushStack(line_stack, (char*) current);
              current = (Node*) current->data;
              resetFlagsAndCounters();
              *index = 0;
              continue;
            }
            else
            {
              fprintf(stderr,ERROR"Invalid Conditional String !\n<%s> is not a valid packet !\n"RESET, buffer);
              status = False;
              goto deallocate_and_exit;
            }
          }
          else
          {
            fprintf(stderr,ERROR"Invalid Conditional String !\n<%s> is not a valid packet !\n"RESET, buffer);
            status = False;
            goto deallocate_and_exit;
          }
        }
        else {;}

///////////// CSTR SPEC ////////////////////

        if(isString) break;
        else {;}
        if(ctoken) continue;
        else ctoken = True;
        if(!null_node)
        {
          if(!*((current->key)+(*index)))
          {
            if(isMemb)
            {
              //printf("MEMKEY<%s>\n",current->key);
              isMemb = False;
              Node* mem_variable = (Node*) popStack(line_stack);

              if(mem_variable == NULL) fprintf(stderr, DEBUG_B"[DEBUG INFO]:"RESET " PACKET -1 not found in Stack !\n");
              else {;}

              //UserVariable* var = (UserVariable*) mem_variable->data;

              if(line_tree == NULL)
              {
                line_tree = createNode(NULL, 0, (char*) current);
                pushStack(line_stack, (char*) line_tree);
                insertNodeNTree(line_tree, createNode(NULL, 0, (char*) mem_variable));
                //printf("KEY<%s>\n",var->identifier);
              }
              else
              {
                Node* temp = createNode(NULL, 0, (char*) current);
                insertNodeNTree(line_tree, temp);
                pushStack(line_stack, (char*) line_tree);
                line_tree = temp;
                //printf("KEY<%s>\n",var->identifier);
                insertNodeNTree(line_tree, createNode(NULL, 0, (char*) mem_variable));
                //printf("\nKOKO");
                //pl(line_tree);
              }
            }
            else
            {
              if(line_tree == NULL)
              {
                line_tree = createNode(NULL, 0, (char*) current);
                pushStack(line_stack, (char*) line_tree);
                //printf("\nKOKONOMEM");
                //pl(line_tree);
              }
              else
              {
                Node* temp = createNode(NULL, 0, (char*) current);
                insertNodeNTree(line_tree, temp);
                pushStack(line_stack, (char*) line_tree);
                //printf("\nKOKONOMEM");
                //pl(line_tree);
                line_tree = temp;
              }
            }

            resetFlagsAndCounters();
            *index = 0;
            current = symbol_tree;
            continue;
          }
          else
          {
            while(True)
            {
              current = getNode(current, 0, index);
              if (current == NULL)
              {
                fprintf(stderr, ERROR"Undeclared function <%s> !\n"RESET, buffer); 

                status = False;
                goto deallocate_and_exit;
              }
              else if(!*((current->key)+(*index)))
              {
                if(line_tree == NULL)
                {
                  line_tree = createNode(NULL, 0, (char*) current);
                  pushStack(line_stack, (char*) line_tree);
                }
                else
                {
                  Node* temp = createNode(NULL, 0, (char*) current);
                  insertNodeNTree(line_tree, temp);
                  pushStack(line_stack, (char*) line_tree);
                  line_tree = temp;
                }
                resetFlagsAndCounters();
                *index = 0;
                current = symbol_tree;
                break;
              }
              else {;}
            }
            continue;
          }
        }
        else
        {
          if(isMemb)
          {
            fprintf(stderr, ERROR"Invalid Member Name <%s> !\n"RESET, buffer); 

            status = False;
            goto deallocate_and_exit;
          }
          else {;}
          Node* raw_var = allocate(buffer, flag);
          if(raw_var == NULL)
          {
            fprintf(stderr, ERROR"Undeclared function <%s> !\n"RESET,buffer);

            status = False;
            goto deallocate_and_exit;
          }
          else {;}
          insertNodeNTree(line_tree, createNode(NULL, 0, (char*) raw_var));
          resetFlagsAndCounters();
          *index = 0;
          current = symbol_tree;
          continue;
        }
        break;
      case CLOSE_PARANTH_TOKEN:
///////////// CSTR SPEC ////////////////////

        if(flag == c_TOKEN) {flag = 0; continue;}
        else {;}

        if(isCondStr)
        {
          UserVariable* var;
          Node* raw_var;
          char var_len;
          char* var_val;

          if(!null_node && !*((current->key)+(*index)))
          {
            if(current->type != VARIABLE_NODE)
            {
              fprintf(stderr, ERROR"<%s> is not a variable !\n"RESET, current->key);
              status = False;
              goto deallocate_and_exit;
            }
            else {;}

            var = (UserVariable*) current->data;
            var_len = var->length;
            if(!var_len)
            {
              fprintf(stderr, ERROR"Variable <%s> not defined !\n"RESET, var->identifier);
              status = False;
              goto deallocate_and_exit;
            }
            else {;}
            var_val = var->value;
          }
          else
          {
            raw_var = allocate(buffer, flag);
            if(raw_var == NULL)
            {
              fprintf(stderr, ERROR"Invalid value <%s> !\n"RESET, buffer);
              status = False;
              goto deallocate_and_exit;
            }
            else {;}
            var = (UserVariable*) (raw_var->data);
            var_len = var->length;
            var_val = var->value;
          }

          if(var_len == cstr_default_len) {;}
          else if(!cstr_default_len)
          {
            cstr_buff_ind -= 4;
            unsigned short start = *((short*)((cstr->buffer)+cstr_buff_ind++));
            start+=var_len; cstr_buff_ind++;
            *((short*)((cstr->buffer)+cstr_buff_ind++)) = start;
            cstr_buff_ind++;
          }
          else
          {
            fprintf(stderr, ERROR"Size Mismatch in condition string lhs and rhs !\n"RESET);
            fprintf(stderr, ERROR"lhs requires <%d> bytes(s) and rhs is <%d> byte(s) !\n"RESET, cstr_default_len, var_len);

            if(!null_node && !*((current->key)+(*index))) {;}
            else
            {
              free(var_val);
              free(var);
              free(raw_var);
              var_val = NULL; var = NULL; raw_var = NULL;
            }

            status = False;
            goto deallocate_and_exit;
          }

          char in = 0;
          for(; in<var_len; in++) readToBuffer(cstr, *(var_val+in), cstr_buff_ind++);

          if(!null_node && !*((current->key)+(*index))) {;}
          else
          {
            free(var_val);
            free(var);
            free(raw_var);
            var_val = NULL; var = NULL; raw_var = NULL;
          }

          Node* current_packet = (Node*) popStack(line_stack);
          cstr_offset += getPacketLength(current_packet->type);
          current = packet_tree;
          resetFlagsAndCounters();
          *index = 0;
          continue;
        }
        else {;}

///////////// CSTR SPEC ////////////////////
        if(isString) break;
        else {;}
        if(ctoken)
        {
          line_tree = (Node*) popStack(line_stack);
//printf("\nCLSE");pl(line_tree);

          if(line_tree == NULL) fprintf(stderr, DEBUG_B"[DEBUG INFO]:"RESET " NESTED FUNCTION RETURN VALUE not found in Stack !\n");
          else {;}

          continue;
        }
        else ctoken = True;

        if(!null_node)
        {
          if(!*((current->key)+(*index)))
          {
            if(isMemb)
            {
              Node* mem_variable = (Node*) popStack(line_stack);

              if(mem_variable == NULL) fprintf(stderr, DEBUG_B"[DEBUG INFO]:"RESET " PACKET -1 not found in Stack !\n");
              else {;}

              UserVariable* packet_variable = (UserVariable*) mem_variable->data;

              if(operation)
              {
                line_tree = (Node*) popStack(line_stack);
                if(line_tree == NULL) fprintf(stderr, DEBUG_B"[DEBUG INFO]:"RESET " Operator Pred not found in Stack !\n");
                else {;}
                operation = False;
              }
              else {;}

              PacketObject* current_packet = (PacketObject*) packet_variable->value;
              UserVariable* member_variable = getMemberVariable(current_packet, current->type);
              if (member_variable == NULL) return False;
              else {;}

              insertNodeNTree(line_tree, createNode(NULL, 0, (char*) createNode("NULL", RAW_VARIABLE_NODE, (char*) member_variable)));

              resetFlagsAndCounters();
              *index = 0;
              current = symbol_tree;
              continue;
            }
            else {;}

            if(line_tree == NULL) line_tree = createNode(NULL, 0, (char*) current);
            else insertNodeNTree(line_tree, createNode(NULL, 0, (char*) current));
            line_tree = (Node*) popStack(line_stack);

            if(line_tree == NULL) fprintf(stderr, DEBUG_B"[DEBUG INFO]:"RESET " OUTER FUNCTION NODE - 1 not found in Stack !\n");
            else {;}

            if(operation)
            {
              line_tree = (Node*) popStack(line_stack);
              if(line_tree == NULL) fprintf(stderr, DEBUG_B"[DEBUG INFO]:"RESET " Operator Pred not found in Stack !\n");
              else {;}
              operation = False;
            }
            else {;}

            *index = buffer_ind = flag = 0;
            current = symbol_tree;
            null_node = False;
            continue;
          }
          else
          {
            while(True)
            {
              current = getNode(current, 0, index);
              if (current == NULL)
              {
                fprintf(stderr, ERROR"Undeclared variable <%s> !\n"RESET, buffer); 

                status = False;
                goto deallocate_and_exit;
              }
              else if(!*((current->key)+(*index)))
              {
                if(line_tree == NULL) line_tree = createNode(NULL, 0, (char*) current);
                else insertNodeNTree(line_tree, createNode(NULL, 0, (char*) current));

                line_tree = (Node*) popStack(line_stack);

                if(line_tree == NULL) fprintf(stderr, DEBUG_B"[DEBUG INFO]:"RESET " OUTER FUNCTION NODE - 2 not found in Stack !\n");
                else {;}

                if(operation)
                {
                  line_tree = (Node*) popStack(line_stack);
                  if(line_tree == NULL) fprintf(stderr, DEBUG_B"[DEBUG INFO]:"RESET " Operator Pred not found in Stack !\n");
                  else {;}
                  operation = False;
                }
                else {;}

                resetFlagsAndCounters();
                *index = 0;
                current = symbol_tree;
                break;
              }
              else {;}
            }
            continue;
          }
        }
        else
        {
          if(isMemb)
          {
            fprintf(stderr, ERROR"Invalid Member Name <%s> !\n"RESET, buffer); 

            status = False;
            goto deallocate_and_exit;
          }
          else {;}
          Node* raw_var = allocate(buffer, flag);
          if(raw_var == NULL)
          {
            fprintf(stderr, ERROR"Undeclared variable <%s> !\n"RESET,buffer);

            status = False;
            goto deallocate_and_exit;
          }
          else {;}
          insertNodeNTree(line_tree, createNode(NULL, 0, (char*) raw_var));
          line_tree = (Node*) popStack(line_stack);

          if(line_tree == NULL) fprintf(stderr, DEBUG_B"[DEBUG INFO]:"RESET " OUTER FUNCTION NODE - 3 not found in Stack !\n");
          else {;}

          if(operation)
          {
            line_tree = (Node*) popStack(line_stack);
            if(line_tree == NULL) fprintf(stderr, DEBUG_B"[DEBUG INFO]:"RESET " Operator Pred not found in Stack !\n");
            else {;}
            operation = False;
          }
          else {;}

          resetFlagsAndCounters();
          *index = 0;
          current = symbol_tree;
          continue;
        }
        break;
      case COMMA_TOKEN:
///////////// CSTR SPEC ////////////////////

        if(flag == c_TOKEN) {flag = 0; continue;}
        else {;}

        if(isCondStr)
        {
          UserVariable* var;
          Node* raw_var;
          char var_len;
          char* var_val;

          if(!null_node && !*((current->key)+(*index)))
          {
            if(current->type != VARIABLE_NODE)
            {
              fprintf(stderr, ERROR"<%s> is not a variable !\n"RESET, current->key);
              status = False;
              goto deallocate_and_exit;
            }
            else {;}

            var = (UserVariable*) current->data;
            var_len = var->length;
            if(!var_len)
            {
              fprintf(stderr, ERROR"Variable <%s> not defined !\n"RESET, var->identifier);
              status = False;
              goto deallocate_and_exit;
            }
            else {;}
            var_val = var->value;
          }
          else
          {
            raw_var = allocate(buffer, flag);
            if(raw_var == NULL)
            {
              fprintf(stderr, ERROR"Invalid value <%s> !\n"RESET, buffer);
              status = False;
              goto deallocate_and_exit;
            }
            else {;}
            var = (UserVariable*) (raw_var->data);
            var_len = var->length;
            var_val = var->value;
          }

          if(var_len == cstr_default_len) {;}
          else if(!cstr_default_len)
          {
            cstr_buff_ind -= 4;
            unsigned short start = *((short*)((cstr->buffer)+cstr_buff_ind++));
            start+=var_len; cstr_buff_ind++;
            *((short*)((cstr->buffer)+cstr_buff_ind++)) = start;
            cstr_buff_ind++;
          }
          else
          {
            fprintf(stderr, ERROR"Size Mismatch in condition string lhs and rhs !\n"RESET);
            fprintf(stderr, ERROR"lhs requires <%d> bytes(s) and rhs is <%d> byte(s) !\n"RESET, cstr_default_len, var_len);

            if(!null_node && !*((current->key)+(*index))) {;}
            else
            {
              free(var_val);
              free(var);
              free(raw_var);
              var_val = NULL; var = NULL; raw_var = NULL;
            }

            status = False;
            goto deallocate_and_exit;
          }

          char in = 0;
          for(; in<var_len; in++) readToBuffer(cstr, *(var_val+in), cstr_buff_ind++);

          if(!null_node && !*((current->key)+(*index))) {;}
          else
          {
            free(var_val);
            free(var);
            free(raw_var);
            var_val = NULL; var = NULL; raw_var = NULL;
          }

          Node* current_packet = (Node*) popStack(line_stack);
          current = (Node*) current_packet->data;
          pushStack(line_stack, (char*) current_packet);
          resetFlagsAndCounters();
          *index = 0;
          continue;
        }
        else {;}


///////////// CSTR SPEC ////////////////////

        if(isString) break;
        else {;}
        if(ctoken) continue;
        else ctoken = True;
        if(!null_node)
        {
          if(!*((current->key)+(*index)))
          {

            if(isMemb)
            {
              Node* mem_variable = (Node*) popStack(line_stack);

              if(mem_variable == NULL) fprintf(stderr, DEBUG_B"[DEBUG INFO]:"RESET " PACKET -1 not found in Stack !\n");
              else {;}

              UserVariable* packet_variable = (UserVariable*) mem_variable->data;

              PacketObject* current_packet = (PacketObject*) packet_variable->value;
              UserVariable* member_variable = getMemberVariable(current_packet, current->type);
              if (member_variable == NULL) return False;
              else {;}

              insertNodeNTree(line_tree, createNode(NULL, 0, (char*) createNode("NULL", RAW_VARIABLE_NODE, (char*) member_variable)));

              if(operation)
              {
                line_tree = (Node*) popStack(line_stack);
                if(line_tree == NULL) fprintf(stderr, DEBUG_B"[DEBUG INFO]:"RESET " Operator Pred not found in Stack !\n");
                else {;}
                operation = False;
              }
              else {;}

              resetFlagsAndCounters();
              *index = 0;
              current = symbol_tree;
              continue;
            }
            else {;}

            if(line_tree == NULL) line_tree = createNode(NULL, 0, (char*) current);
            else insertNodeNTree(line_tree, createNode(NULL, 0, (char*) current));

            if(operation)
            {
              line_tree = (Node*) popStack(line_stack);
              if(line_tree == NULL) fprintf(stderr, DEBUG_B"[DEBUG INFO]:"RESET " Operator Pred not found in Stack !\n");
              else {;}
              operation = False;
            }
            else {;}

            resetFlagsAndCounters();
            *index = 0;
            current = symbol_tree;
            continue;
          }
          else
          {
            while(True)
            {
              current = getNode(current, 0, index);
              if (current == NULL)
              {
                fprintf(stderr, ERROR"Undeclared variable <%s> !\n"RESET, buffer); 

                status = False;
                goto deallocate_and_exit;
              }
              else if(!*((current->key)+(*index)))
              {
                if(line_tree == NULL) line_tree = createNode(NULL, 0, (char*) current);
                else insertNodeNTree(line_tree, createNode(NULL, 0, (char*) current));

                if(operation)
                {
                  line_tree = (Node*) popStack(line_stack);
                  if(line_tree == NULL) fprintf(stderr, DEBUG_B"[DEBUG INFO]:"RESET " Operator Pred not found in Stack !\n");
                  else {;}
                  operation = False;
                }
                else {;}

                resetFlagsAndCounters();
                *index = 0;
                current = symbol_tree;
                break;
              }
              else {;}
            }
            continue;
          }
        }
        else
        {
          if(isMemb)
          {
            fprintf(stderr, ERROR"Invalid Member Name <%s> !\n"RESET, buffer); 

            status = False;
            goto deallocate_and_exit;
          }
          else {;}
          Node* raw_var = allocate(buffer, flag);
          if(raw_var == NULL)
          {
            fprintf(stderr, ERROR"Undeclared variable <%s> !\n"RESET,buffer);

            status = False;
            goto deallocate_and_exit;
          }
          else {;}
          insertNodeNTree(line_tree, createNode(NULL, 0, (char*) raw_var));

          if(operation)
          {
            line_tree = (Node*) popStack(line_stack);
            if(line_tree == NULL) fprintf(stderr, DEBUG_B"[DEBUG INFO]:"RESET " Operator Pred not found in Stack !\n");
            else {;}
            operation = False;
          }
          else {;}

          resetFlagsAndCounters();
          *index = 0;
          current = symbol_tree;
          continue;
        }
        break;
      case NULL_TOKEN:
        if(isString) break;
        else {;}

        if(ctoken) break;
        else {;}

        if(flag == c_TOKEN) {flag = 0; continue;}
        else {;}

        if(!null_node)
        {
          if(isMemb)
          {
            Node* mem_variable = (Node*) popStack(line_stack);

            if(mem_variable == NULL) fprintf(stderr, DEBUG_B"[DEBUG INFO]:"RESET " PACKET -1 not found in Stack !\n");
            else {;}

            UserVariable* packet_variable = (UserVariable*) mem_variable->data;

            PacketObject* current_packet = (PacketObject*) packet_variable->value;
            UserVariable* member_variable = getMemberVariable(current_packet, current->type);
            if (member_variable == NULL) return False;
            else {;}

            insertNodeNTree(line_tree, createNode(NULL, 0, (char*) createNode("NULL", RAW_VARIABLE_NODE, (char*) member_variable)));

            if(operation)
            {
              line_tree = (Node*) popStack(line_stack);
              if(line_tree == NULL) fprintf(stderr, DEBUG_B"[DEBUG INFO]:"RESET " Operator Pred not found in Stack !\n");
              else {;}
              operation = False;
            }
            else {;}

            resetFlagsAndCounters();
            *index = 0;
            current = symbol_tree;
            break;
          }
          else {;}


          if(!*((current->key)+(*index)))
          {
            if(line_tree == NULL) line_tree = createNode(NULL, 0, (char*) current);
            else insertNodeNTree(line_tree, createNode(NULL, 0, (char*) current));

            if(operation)
            {
              line_tree = (Node*) popStack(line_stack);
              if(line_tree == NULL) fprintf(stderr, DEBUG_B"[DEBUG INFO]:"RESET " Operator Pred not found in Stack !\n");
              else {;}
              operation = False;
            }
            else {;}

            resetFlagsAndCounters();
            *index = 0;
            current = symbol_tree;
            break;
          }
          else {;}
        }
        else
        {
          if(isMemb)
          {
            fprintf(stderr, ERROR"Invalid Member Name <%s> !\n"RESET, buffer); 

            status = False;
            goto deallocate_and_exit;
          }
          else {;}

          Node* raw_var = allocate(buffer, flag);
          if(raw_var == NULL)
          {
            fprintf(stderr, ERROR"Undeclared variable <%s> !\n"RESET,buffer);

            status = False;
            goto deallocate_and_exit;
          }
          else {;}
          insertNodeNTree(line_tree, createNode(NULL, 0, (char*) raw_var));

          if(operation)
          {
            line_tree = (Node*) popStack(line_stack);
            if(line_tree == NULL) fprintf(stderr, DEBUG_B"[DEBUG INFO]:"RESET " Operator Pred not found in Stack !\n");
            else {;}
            operation = False;
          }
          else {;}

          resetFlagsAndCounters();
          *index = 0;
          current = symbol_tree;
          break;
        }
        break;

      // NON - SEPARATOR TOKENS //
      case PERIOD_TOKEN:
        if(isString) break;
        else {;}

        if(ctoken) continue;
        else {;}
        if(!null_node)
        {
          if(!*((current->key)+(*index)))
          {
            if (!((UserVariable*)(current->data))->length)
            {
              fprintf(stderr, ERROR"Invalid membership access !\nVariable <%s> is not defined !\n"RESET,buffer);

              status = False;
              goto deallocate_and_exit;
            }
            else {;}
            pushStack(line_stack, (char*) current);
            //printf("KEY<%s>\n",((UserVariable*)(current->data))->identifier);

            *index = buffer_ind = flag = 0;
            UserVariable* uv = (UserVariable*)(current->data);
            //payload_len = &((PacketObject*)(uv->value))->payload_len;
            current = uv->members;
            //pre(current);

            null_node = False; isMemb = True;
            continue;
          }
          else {;}
        }
        else
        {
          Node* raw_var = allocate(buffer, flag);
          if(raw_var == NULL)
          {
            fprintf(stderr, ERROR"Undeclared variable <%s> !\n"RESET,buffer);

            status = False;
            goto deallocate_and_exit;
          }
          else {;}
          pushStack(line_stack, (char*) raw_var);

          *index = buffer_ind = flag = 0;
          UserVariable* uv = (UserVariable*)(raw_var->data);
          current = uv->members;

          null_node = False; isMemb = True;
          continue;
        }

        flag += PERIOD_FLAG;
        break;
      case TILDE_TOKEN:
        if(isString) break;
        else {;}
        flag += TILDE_FLAG;
        break;
      case DQUOTE_TOKEN:
        if(escape) { escape = False; break; }
        else {;}
        if(isCondStr && flag == c_TOKEN)
        {
          current = packet_tree;
          flag = 0;
          continue;
        }
        else if(isCondStr && flag!=c_TOKEN)
        {
          isCondStr = False;
          flag = c_TOKEN;
          Node* cstr_node = allocCondStr(cstr->buffer,cstr_buff_ind);
          cstr_buff_ind = 0;
          insertNodeNTree(line_tree, createNode(NULL, 0, (char*) cstr_node));
          continue;
        }
        else
        {
          isString = !isString;
          flag = DQUOTE_TOKEN;
        }
        break;
      case HEXSTREAM_FLAG:
        if(!buffer_ind) flag = HEXSTREAM_FLAG;
        else {;}
        break;
      case COLON_TOKEN:
        flag = COLON_TOKEN;
        break;
      case b_TOKEN:
        if(flag == COLON_TOKEN) flag = BIT_FLAG;
        else {;}
        break;
      case B_TOKEN:
        if(flag == COLON_TOKEN) flag = NUMBER_FLAG;
        else {;}
        break;

      case c_TOKEN:
        if(isString) break;
        else {;}
        if(*(line+i+1)==DQUOTE_TOKEN) { isCondStr = True; flag = c_TOKEN; continue; }
        else {;}
        break;
    }

    if(!*(line+i)) break;
    else {;}

    ctoken = False;

    readToBuffer(bufferObj, *(line+i), buffer_ind++);

    if(!null_node)
    {
      current = getNode(current, *(line+i), index);
      if (current == NULL) null_node = True;
      else {;}
    }
    else {;}
  }
  resetStack(line_stack);
  status = evaluate(line_tree, line_stack);

  deallocate_and_exit:
    free(index);
    free(start_end);
    free(bit_specific);

    freeBuffer(cstr);
    freeBuffer(bufferObj);

    freeNTree(line_tree);
    freeStack(line_stack);

    short z = 0;
    for(;z<allocd; z++) { free(*(extracted_bit_values+z)); *(extracted_bit_values+z) = NULL; }
    free(extracted_bit_values);
    extracted_bit_values = NULL;

  return status;
}
