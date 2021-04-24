#include "shell.h"
#include "utils/string.h"
#include "parse/interpret.h"

#include "datastructures/buffer.h"

static short stdinRead(Buffer* stdin_buff)
{
  unsigned char letter;
  short index;

  while(True) 
  {
    letter = fgetc(stdin);
    if( letter == '\n') break;
    else {;}

    readToBuffer(stdin_buff, letter, index++);
  }
  return index;
}

void startShell(const char* version)
{
  fprintf(stdout, "\nStarting shell - "SHELL_PROMPT"%s\n"RESET, version);
  fprintf(stdout,"--------------------------\n\n");

  Buffer* line;
  short line_len;

  line = createBuffer();

  char* indicator = "(_ding_) \0";
  while(True)
  {
    fprintf(stdout, "\r");
    *(line->buffer) = 0;
    if(decl_flag) indicator = "(.decl:) \0";
    else if(func_flag) indicator = "(.func:) \0";
    else if(flow_flag) indicator = "(.flow:) \0";
    else  indicator = "(_ding_) \0";

    fprintf(stdout,SHELL_PROMPT "%s" RESET,  indicator);
    line_len = stdinRead(line);

    if(stringsEqual(".decl:",line->buffer)) {fprintf(stdout,"\n"); decl_flag = True; func_flag = False; flow_flag = False; continue; }
    else if(stringsEqual(".func:",line->buffer)) {fprintf(stdout,"\n"); decl_flag = False; func_flag = True; flow_flag = False; continue;}
    else if(stringsEqual(".flow:",line->buffer)) {fprintf(stdout,"\n"); decl_flag = False; func_flag = False; flow_flag = True; continue; }
    else if(stringsEqual("exit",line->buffer)) {fprintf(stdout,"\nExiting ding Shell...\n"); break; }
    else {;}
    fprintf(stdout,"         ");

    interpretLines(line->buffer, line_len+1, 1);
  }
  freeBuffer(line);
}
