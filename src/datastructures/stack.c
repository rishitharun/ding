#include "datastructures/stack.h"

#include "logger.h"

// DO NOT FREE STACKNODES
// FREE ONLY STACKS

static Stack* initStack(short size, boolean dynamic)
{
  Stack* new_stack = (Stack*) malloc(sizeof(Stack));
  new_stack-> size = size;
  new_stack-> current = -1;
  new_stack-> dynamic = dynamic;
  new_stack->stack_nodes = (char**) malloc(size*sizeof(char*));

  #if LOGGING_ENABLED(LOG_MEMORY) > 0
    MEMORY_LOGGING_UTIL("M\0", sizeof(Stack), new_stack, "Stack");
    MEMORY_LOGGING_UTIL("M\0", (long int)(new_stack->size)*(sizeof(char*)), new_stack->stack_nodes, "stack_nodes in Stack");
  #endif

  return new_stack;
}

Stack* initDynamicStack()
{
  return initStack(STACK_DEFAULT_SIZE, True);
}

Stack* initFixedStack(short size)
{
  return initStack(size, False);
}

void pushStack(Stack* stack, char* data)
{
  if (stack->current >= (stack->size)-1)
  {
    if(stack->dynamic)
    {
      #if LOGGING_ENABLED(LOG_MEMORY) > 0
        MEMORY_LOGGING_UTIL("F\0", (long int)(stack->size)*(sizeof(char*)), stack->stack_nodes, "stack_nodes in Stack");
      #endif

      stack->size += STACK_DEFAULT_SIZE;
      stack->stack_nodes = (char**) realloc(stack->stack_nodes, (stack->size)*sizeof(char*));

      #if LOGGING_ENABLED(LOG_MEMORY) > 0
        MEMORY_LOGGING_UTIL("R\0", (long int)(stack->size)*(sizeof(char*)), stack->stack_nodes, "stack_nodes in Stack");
      #endif
    }
    else
    {
      fprintf(stderr, DEBUG_B"[DEBUG INFO]:"RESET" Stack Full !\n");
      return;
    }
  }
  else {;}

  stack->current += 1;
  *((stack->stack_nodes)+(stack->current)) = data;
}

char* popStack(Stack* stack)
{
  if((stack->current)<0) { fprintf(stderr, DEBUG_B"[DEBUG INFO]:"RESET " Stack Empty !\n"); return NULL; }
  else {;}

  char* data = *((stack->stack_nodes)+(stack->current));
  stack->current -= 1;

  return data;
}

void resetStack(Stack* stack)
{
  stack-> current = -1;
}

void freeStack(Stack* stack)
{
  #if LOGGING_ENABLED(LOG_MEMORY) > 0
    MEMORY_LOGGING_UTIL("F\0", (long int)(stack->size)*(sizeof(char*)), stack->stack_nodes, "stack_nodes in Stack");
    MEMORY_LOGGING_UTIL("F\0", sizeof(Stack), stack, "Stack");
  #endif

  free(stack->stack_nodes);
  free(stack);
}


/*
======================================================================
|----------------- DEBUG FUNCTION USED BY THE AUTHOR ----------------|
|--------------------------------------------------------------------|
|-USE THESE OR DEFINE YOUR OWN FUNCTIONS TO DEBUG DURING DEVELOPMENT-|
======================================================================

void printStack(Stack* stack, short current)
{
  if (current < 0) return;
  else
  {
    printf("");
    printStack(stack, current-1);
  }
}
*/
