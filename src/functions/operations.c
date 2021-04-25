#include "utils/string.h"
#include "codes/datatype.h"
#include "functions/operations.h"

UserVariable* add(Node* args)
{
  UserVariable* value = (UserVariable*)(((Node*)(args->data))->data);
  UserVariable* return_var = (UserVariable*) malloc(1*sizeof(UserVariable));
  return_var->identifier = NULL;
  return_var->datatype = NUMBER_TYPE_CODE;
  return_var->value = (char*) malloc(sizeof(value->length));
  return_var->length = value->length;

  clearMemory(return_var->value, return_var->length);

  while(args != NULL)
  {
    UserVariable* value = (UserVariable*)(((Node*)(args->data))->data);

    if(!value->length)
    {
      fprintf(stderr, ERROR"Variable <%s> not defined !\n"RESET, value->identifier);
      free(return_var->value);
      free(return_var);
      return NULL;
    }
    else if(value->datatype != NUMBER_TYPE_CODE)
    {
      fprintf(stderr, ERROR"Addition can be done only between numbers !\n"RESET);
      free(return_var->value);
      free(return_var);
      return NULL;
    }
    else if(return_var->length != value->length)
    {
      fprintf(stderr, ERROR"Value size mismatch !\n"RESET);
      free(return_var->value);
      free(return_var);
      return NULL;
    }
    else {;}

    #define maximum(a,b) ((a>b)?a:b)

    char i = return_var->length - 1, carry = 0;
    for(; i>=0; i--)
    {
      unsigned char greater = maximum(*(return_var->value+i), *(value->value+i));
      *(return_var->value+i) += *(value->value+i) + carry;
      carry = (greater <= *(return_var->value+i)) ? 0 : 1;
    }
  args = args->right;
  }
  return return_var;
}

UserVariable* subtract(Node* args)
{
  if((args == NULL) || (args->right == NULL)) return NULL;
  else {;}

  UserVariable* lvalue = (UserVariable*)(((Node*)(args->data))->data);
  UserVariable* rvalue = (UserVariable*)(((Node*)(args->right->data))->data);

  UserVariable* return_var = (UserVariable*) malloc(1*sizeof(UserVariable));
  return_var->identifier = NULL;
  return_var->datatype = NUMBER_TYPE_CODE;
  return_var->value = (char*) malloc(sizeof(lvalue->length));
  return_var->length = lvalue->length;

  clearMemory(return_var->value, return_var->length);

  if(!lvalue->length)
  {
    fprintf(stderr, ERROR"Variable <%s> not defined !\n"RESET, lvalue->identifier);
    free(return_var->value);
    free(return_var);
    return NULL;
  }
  else if(!rvalue->length)
  {
    fprintf(stderr, ERROR"Variable <%s> not defined !\n"RESET, rvalue->identifier);
    free(return_var->value);
    free(return_var);
    return NULL;
  }
  else if(lvalue->datatype != NUMBER_TYPE_CODE || rvalue->datatype != NUMBER_TYPE_CODE)
  {
    fprintf(stderr, ERROR"Subtraction can be done only between numbers !\n"RESET);
    free(return_var->value);
    free(return_var);
    return NULL;
  }
  else if(lvalue->length != rvalue->length)
  {
    fprintf(stderr, ERROR"Operand size mismatch !\n"RESET);
    free(return_var->value);
    free(return_var);
    return NULL;
  }
  else {;}

  #define minimum(a,b) ((a<b)?a:b)

  char i = return_var->length - 1, carry = 0;
  for(; i>=0; i--)
  {
    unsigned char smaller = minimum(*(lvalue->value+i), *(rvalue->value+i));
    *(return_var->value+i) = *(lvalue->value+i) - *(rvalue->value+i) - carry;
    carry = (smaller >= *(return_var->value+i)) ? 0 : 1;
  }

  return return_var;
}

UserVariable* lessthan(Node* args)
{
  Node* left_op = args;
  Node* right_op = args->right;

  if(left_op == NULL || right_op == NULL)
  {
    fprintf(stderr, ERROR"Pass two Arguments to compare !\n"RESET);
    return NULL;
  }
  else {;}

  UserVariable* left_value = (UserVariable*)(((Node*)(left_op->data))->data);
  UserVariable* right_value = (UserVariable*)(((Node*)(right_op->data))->data);

  if(!left_value->length)
  {
    fprintf(stderr, ERROR"Variable <%s> not defined !\n"RESET, left_value->identifier);
    return NULL;
  }
  else {;}

  if(!right_value->length)
  {
    fprintf(stderr, ERROR"Variable <%s> not defined !\n"RESET, right_value->identifier);
    return NULL;
  }
  else {;}

  if(left_value->length != right_value->length)
  {
    fprintf(stderr, ERROR"Value size mismatch !\n"RESET);
    return NULL;
  }
  else {;}

  if(left_value->datatype != NUMBER_TYPE_CODE || right_value->datatype != NUMBER_TYPE_CODE)
  {
    fprintf(stderr, ERROR"Pass arguments of number type !\n"RESET);
    return NULL;
  }
  else {;}

  UserVariable* return_var = (UserVariable*) malloc(1*sizeof(UserVariable));
  return_var->identifier = NULL;
  return_var->datatype = NUMBER_TYPE_CODE;
  return_var->value = (char*) malloc(sizeof(char));
  return_var->length = 1;

  *(return_var->value) = 0;

  char i = 0;
  for(; i<left_value->length; i++)
  {
    if(*(left_value->value+i) < *(right_value->value+i))
    {
      *(return_var->value) = 1;
      break;
    }
    else if(*(left_value->value+i) > *(right_value->value+i))
    {
      *(return_var->value) = 0;
      break;
    }
    else {;}
  }
  return return_var;
}

UserVariable* greaterthan(Node* args)
{
  Node* left_op = args;
  Node* right_op = args->right;

  if(left_op == NULL || right_op == NULL)
  {
    fprintf(stderr, ERROR"Pass two Arguments to compare !\n"RESET);
    return NULL;
  }
  else {;}

  UserVariable* left_value = (UserVariable*)(((Node*)(left_op->data))->data);
  UserVariable* right_value = (UserVariable*)(((Node*)(right_op->data))->data);

  if(!left_value->length)
  {
    fprintf(stderr, ERROR"Variable <%s> not defined !\n"RESET, left_value->identifier);
    return NULL;
  }
  else {;}

  if(!right_value->length)
  {
    fprintf(stderr, ERROR"Variable <%s> not defined !\n"RESET, right_value->identifier);
    return NULL;
  }
  else {;}

  if(left_value->length != right_value->length)
  {
    fprintf(stderr, ERROR"Value size mismatch !\n"RESET);
    return NULL;
  }
  else {;}

  if(left_value->datatype != NUMBER_TYPE_CODE || right_value->datatype != NUMBER_TYPE_CODE)
  {
    fprintf(stderr, ERROR"Pass arguments of number type !\n"RESET);
    return NULL;
  }
  else {;}

  UserVariable* return_var = (UserVariable*) malloc(1*sizeof(UserVariable));
  return_var->identifier = NULL;
  return_var->datatype = NUMBER_TYPE_CODE;
  return_var->value = (char*) malloc(sizeof(char));
  return_var->length = 1;

  *(return_var->value) = 0;

  char i = 0;
  for(; i<left_value->length; i++)
  {
    if(*(left_value->value+i) > *(right_value->value+i))
    {
      *(return_var->value) = 1;
      break;
    }
    else if(*(left_value->value+i) < *(right_value->value+i))
    {
      *(return_var->value) = 0;
      break;
    }
    else {;}
  }

  return return_var;
}

