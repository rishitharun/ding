#include "datastructures/buffer.h"

#include "logger.h" 

Buffer* createBuffer()
{
  Buffer* new_buffer = (Buffer*) malloc(sizeof(Buffer));
  new_buffer->size = BUFFER_DEFAULT_SIZE;
  new_buffer->buffer = (char*) malloc(new_buffer->size);

  #if LOGGING_ENABLED(LOG_MEMORY) > 0
    MEMORY_LOGGING_UTIL("M\0", sizeof(Buffer), new_buffer, "Buffer");
    MEMORY_LOGGING_UTIL("M\0", (long int) new_buffer->size, new_buffer->buffer, "buffer in Buffer");
  #endif

  return new_buffer;
}

void readToBuffer(Buffer* buffer, char data, short index)
{
  if (index+1 >= buffer->size)
  {
    #if LOGGING_ENABLED(LOG_MEMORY) > 0
      MEMORY_LOGGING_UTIL("F\0", (long int) buffer->size, buffer->buffer, "buffer in Buffer");
    #endif

    buffer->size += BUFFER_DEFAULT_SIZE;
    buffer->buffer = (char*) realloc(buffer->buffer, buffer->size);

    #if LOGGING_ENABLED(LOG_MEMORY) > 0
      MEMORY_LOGGING_UTIL("R\0", (long int)buffer->size, buffer->buffer, "buffer in Buffer");
    #endif
  }
  else {;}
  *((buffer->buffer)+index) = data;
  *((buffer->buffer)+index+1) = 0;
}

void freeBuffer(Buffer* buffer)
{
  #if LOGGING_ENABLED(LOG_MEMORY) > 0
    MEMORY_LOGGING_UTIL("F\0", (long int)buffer->size, buffer->buffer, "buffer in Buffer");
    MEMORY_LOGGING_UTIL("F\0", sizeof(Buffer), buffer, "Buffer");
  #endif

  free(buffer->buffer);
  free(buffer);
}

