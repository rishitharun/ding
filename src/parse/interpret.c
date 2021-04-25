#include "codes/token.h"

#include "utils/string.h"

#include "datastructures/stack.h"
#include "datastructures/buffer.h"

#include "parse/flow.h"
#include "parse/func.h"
#include "parse/interpret.h"
#include "parse/declarations.h"

#include "logger.h"

short jump_offset, source_index;
boolean in_loop, condition, if_block, else_block;

Stack* condition_stack;

boolean interpret(char *line, short line_len, boolean shell)
{
  if(shell) goto _shell;
  else {;}
  if      (stringsEqual(line, ".decl:\0")) { decl_flag = True; func_flag = False; flow_flag = False; return True; }
  else if (stringsEqual(line, ".func:\0")) { decl_flag = False; func_flag = True; flow_flag = False; return True; }
  else if (stringsEqual(line, ".flow:\0")) { decl_flag = False; func_flag = False; flow_flag = True; return True; }

  else if (in_loop && !condition && *(line+0)=='l' && *(line+1)=='o' && *(line+2)=='o' && *(line+3)=='p')
  {
    short* dummy = malloc(3*sizeof(short));
    *(dummy+0) = 0; *(dummy+1) = 1; *(dummy+2) = 0;
    pushStack(condition_stack, (char*) dummy);
  }

  else if (stringsEqual(line, "endloop\0"))
  {
    #if LOGGING_ENABLED(LOG_CONTROL_STRUCTURES)
      CS_LOGGING_UTIL0("loop","endloop encountered");
    #endif

    unsigned short* prev_values = (short*) popStack(condition_stack);

    if(prev_values == NULL) { fprintf(stderr, DEBUG_B"[DEBUG INFO]:"RESET" NULL from Condition Stack !\n"); return False; }
    else {;}

    unsigned short prev_jump_offset = *(prev_values+0);
    unsigned short prev_inloop = *(prev_values+1);
    unsigned short prev_condn = *(prev_values+2);

    #if LOGGING_ENABLED(LOG_CONTROL_STRUCTURES)
      CS_LOGGING_UTIL0("loop","loop control data after popping from stack");
      CS_LOGGING_UTIL0("loop","------------------------------------------");
      CS_LOGGING_UTIL1("loop","jump_offset %d",prev_jump_offset);
      CS_LOGGING_UTIL1("loop","in_loop %d", prev_inloop);
      CS_LOGGING_UTIL1("loop","condition %d", prev_condn);
      CS_LOGGING_UTIL0("loop","------------------------------------------");
    #endif

    free(prev_values);
    in_loop = (boolean) prev_inloop;
    condition = (boolean) prev_condn;

    if(!condition)
    {
      if(prev_jump_offset) in_loop = False;
      else{;}
      #if LOGGING_ENABLED(LOG_CONTROL_STRUCTURES)
        CS_LOGGING_UTIL0("loop","loop condition is False; breaking out from loop\n");
      #endif
      return True;
    }
    else if(condition)
    {
      jump_offset = prev_jump_offset;
      source_index = jump_offset-1;
      #if LOGGING_ENABLED(LOG_CONTROL_STRUCTURES)
        CS_LOGGING_UTIL0("loop","loop condition is True; looping again\n");
      #endif
      return True;
    }
    else { fprintf(stderr, DEBUG_B"[DEBUG INFO]:"RESET" Unhandled condition in endloop statement !\n"); return False; }
  }
  else if (*(line+0)=='e' && *(line+1)=='l' && *(line+2)=='i' && *(line+3)=='f')
  {
    if_block = False; else_block = False; condition = False;
  }
  else if (stringsEqual(line, "else\0"))
  {
    if_block = False; else_block = True; return True;
  }
  else if (stringsEqual(line, "endif\0"))
  {
    if_block = False; else_block = False; condition = False; return True;
  }
  else {;}

  _shell:
  if(in_loop && !condition)
  {
    #if LOGGING_ENABLED(LOG_CONTROL_STRUCTURES)
      CS_LOGGING_UTIL1("loop","(skipsource) %s", line);
    #endif
    return True;
  }
  else if(if_block && !condition) return True;
  else if(else_block && condition) return True;
  else if (decl_flag) return declareVariables(line, line_len);
  else if (func_flag) return True; /* NOT DEFINED */
  else if (flow_flag) return parseFlow(line, line_len);
  else return True;
}

boolean interpretLines(char* source, int source_length, boolean shell)
{
  condition_stack = initDynamicStack();

  jump_offset = source_index = 0;
  if_block = else_block = in_loop = condition = False;

  Buffer *line;
  line = createBuffer();

  boolean isString = False, comment = False, status = True;
  short line_index = 0, read_lines = 0, paranthesis=0;

  for(;source_index<source_length;source_index++)
  {
    switch(*(source+source_index))
    {
      case BW_SLASH_TOKEN:
        if(isString && *(source+source_index+1) == DQUOTE_TOKEN)
        {
          readToBuffer(line, *(source+source_index++), line_index++);
          readToBuffer(line, *(source+source_index), line_index++);
          continue;
        }
        else break;

      case COMMENT_TOKEN:
        if(!isString) comment = True;
        else {;}
        break;

      case OPEN_PARANTH_TOKEN:
        if(isString || comment) {;}
        else paranthesis++;
        break;

      case CLOSE_PARANTH_TOKEN:
        if(isString || comment) {;}
        else paranthesis--;
        break;

      case TABSPACE_TOKEN:
      case WHITESPACE_TOKEN:
        if(isString) break;
        else {;}
        if(!line_index) continue;
        else if((source_index+1) <source_length)
        {
          switch(*(source+source_index+1))
          {
            case WHITESPACE_TOKEN:
            case TABSPACE_TOKEN:
            case NULL_TOKEN:
            case NEW_LINE_TOKEN:
            case COMMENT_TOKEN:
              continue;
            default:
              break;
          }
        }
        else {;}
        break;

      case DQUOTE_TOKEN:
        isString = !isString;
        break;
      default:;
    }

    if(*(source+source_index) == NEW_LINE_TOKEN || *(source+source_index) == NULL_TOKEN)
    {
      if(isString)
      {
        fprintf(stderr, ERROR"Incomplete String Declaration !\n"RESET);
        fprintf(stderr, ERROR "Error in Interpreting the following line !\n" RESET);
        fprintf(stderr, ERROR"{"RESET" "ERR_L "%s"RESET" "ERROR "}\n"RESET,line->buffer);
        status = False;
        break;
      }
      else {;}

      if(paranthesis)
      {
        fprintf(stderr, ERROR"Paranthesis count mismatch !\n"RESET);
        fprintf(stderr, ERROR "Error in Interpreting the following line !\n" RESET);
        fprintf(stderr, ERROR"{"RESET" "ERR_L "%s"RESET" "ERROR "}\n"RESET,line->buffer);
        status = False;
        break;
      }
      else {;}

      comment = False;
      isString = False;
      if(!line_index) goto skip_line;
      else {;}

      if(!interpret(line->buffer, line_index+1, shell))
      {
        if(shell) break;
        else {;}
        fprintf(stderr, ERROR "Error in Interpreting the following line !\n" RESET);
        fprintf(stderr, ERROR"{"RESET" "ERR_L "%s"RESET" "ERROR "}\n"RESET,line->buffer);
        status = False;
        break;
      }
      else{;}

      jump_offset = source_index+1;

      skip_line:;
      line_index = 0;
      *(line->buffer) = 0;
    }
    else if(comment) continue;
    else readToBuffer(line, *(source+source_index), line_index++);
  }
  freeStack(condition_stack);
  freeBuffer(line);
  return status;
}

