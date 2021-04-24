#include "utils/file.h"

int getFileSize(const char *file_path)
{
  int file_desc;
  int file_size;

  file_desc = open(file_path, O_RDONLY);
  if (file_desc < 0) fprintf(stderr, ERROR "<%s> file not found !\n" RESET, file_path);
  else {;}

  file_size = lseek(file_desc, 0, SEEK_END);
  lseek(file_desc, 0, SEEK_SET);
  close(file_desc);
  
  return file_size;
}

char* readSource(const char *source_path)
{
  int file_desc;
  off_t file_size;
  char *source;

  file_size = getFileSize(source_path);
  source = (char*)malloc((int) file_size);

  file_desc = open(source_path, O_RDONLY);
  read(file_desc, source, file_size);
  close(file_desc);
  
  return source;
}

