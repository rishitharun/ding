#include "codes/datatype.h"
#include "memory/allocate.h"
#include "functions/stringspec.h"

UserVariable* str2ip4(Node* args)
{
  if(args == NULL) { fprintf(stderr, ERROR"[ARGNULL] Please pass a string argument !\n"RESET); return NULL; }
  else {;}
  UserVariable* val = (UserVariable*)(((Node*)(args->data))->data);

  if(val->datatype == STRING_TYPE_CODE) {;}
  else { fprintf(stderr, ERROR"Please pass a string argument <%d>!\n"RESET, val->datatype); return NULL; }

  char* len = (char*) malloc(sizeof(char));
  char* ip4val = allocateIP4(val->value, len);
  free(len);

  if(ip4val == NULL) return NULL;
  else {;}

  UserVariable* ip4var = (UserVariable*) malloc(sizeof(UserVariable));

  ip4var->identifier = NULL;
  ip4var->datatype = IP4_TYPE_CODE;
  ip4var->value = ip4val;
  ip4var->length = 4;

  return ip4var;
}

UserVariable* str2mac(Node* args)
{
  if(args == NULL) { fprintf(stderr, ERROR"[ARGNULL] Please pass a string argument !\n"RESET); return NULL; }
  else {;}
  UserVariable* val = (UserVariable*)(((Node*)(args->data))->data);

  if(val->datatype == STRING_TYPE_CODE) {;}
  else { fprintf(stderr, ERROR"Please pass a string argument <%d>!\n"RESET, val->datatype); return NULL; }

  char* len = (char*) malloc(sizeof(char));
  char* macval = allocateMAC(val->value, len);
  free(len);

  if(macval == NULL) return NULL;
  else {;}

  UserVariable* macvar = (UserVariable*) malloc(sizeof(UserVariable));

  macvar->identifier = NULL;
  macvar->datatype = MAC_TYPE_CODE;
  macvar->value = macval;
  macvar->length = 6;

  return macvar;
}

UserVariable* str2num(Node* args)
{
  if(args == NULL) { fprintf(stderr, ERROR"[ARGNULL] Please pass a string argument !\n"RESET); return NULL; }
  else {;}
  UserVariable* val = (UserVariable*)(((Node*)(args->data))->data);

  if(val->datatype == STRING_TYPE_CODE) {;}
  else { fprintf(stderr, ERROR"Please pass a string argument <%d>!\n"RESET, val->datatype); return NULL; }

  char* len = (char*) malloc(sizeof(char));
  char* numval = allocateNumber(val->value, len);

  if(numval == NULL) { free(len); return NULL; }
  else {;}

  UserVariable* numvar = (UserVariable*) malloc(sizeof(UserVariable));

  numvar->identifier = NULL;
  numvar->datatype = NUMBER_TYPE_CODE;
  numvar->value = numval;
  numvar->length = *len;

  free(len);
  return numvar;
}

UserVariable* str2bit(Node* args)
{
  if(args == NULL) { fprintf(stderr, ERROR"[ARGNULL] Please pass a string argument !\n"RESET); return NULL; }
  else {;}
  UserVariable* val = (UserVariable*)(((Node*)(args->data))->data);

  if(val->datatype == STRING_TYPE_CODE) {;}
  else { fprintf(stderr, ERROR"Please pass a string argument <%d>!\n"RESET, val->datatype); return NULL; }

  char* len = (char*) malloc(sizeof(char));
  char* bitval = allocateBit(val->value, len);

  if(bitval == NULL) { free(len); return NULL; }
  else {;}

  UserVariable* bitvar = (UserVariable*) malloc(sizeof(UserVariable));

  bitvar->identifier = NULL;
  bitvar->datatype = BIT_TYPE_CODE;
  bitvar->value = bitval;
  bitvar->length = *len;

  return bitvar;
}

UserVariable* str2stream(Node* args)
{
  if(args == NULL) { fprintf(stderr, ERROR"[ARGNULL] Please pass a string argument !\n"RESET); return NULL; }
  else {;}
  UserVariable* val = (UserVariable*)(((Node*)(args->data))->data);

  if(val->datatype == STRING_TYPE_CODE) {;}
  else { fprintf(stderr, ERROR"Please pass a string argument <%d>!\n"RESET, val->datatype); return NULL; }

  char* len = (char*) malloc(sizeof(char));
  char* streamval = allocateStream(val->value, len);

  if(streamval == NULL) { free(len); return NULL; }
  else {;}

  UserVariable* streamvar = (UserVariable*) malloc(sizeof(UserVariable));

  streamvar->identifier = NULL;
  streamvar->datatype = STREAM_TYPE_CODE;
  streamvar->value = streamval;
  streamvar->length = *len;

  return streamvar;
}

