#include "main.h"
#include "shell.h"
#include "setup.h"
#include "logger.h"

#include "net/sockets.h"

#include "utils/file.h"
#include "utils/string.h"

#include "parse/interpret.h"

void main(int argc, char *argv[])
{

  if (argc == 1)
  {
    setup(0, NULL);
    if (createSocket()) {;}
    else { freeSetup(); return; }
    startShell(DING_VERSION);
    freeSetup();
    destroySocket();
    return;
  }
  else {;}

  char *source_path = argv[1];

  if(getFileSize(source_path)<=0) return;
  else{;}
  
  int source_length;
  char *source;

  source = readSource(source_path);
  source_length = length(source);
  fprintf(stdout,DEBUG"Source File Read...\n"RESET);

  setup(argc, argv);

  if (createSocket()) {;}
  else { free(source); freeSetup(); return; }
  fprintf(stdout, DEBUG"Socket Created...\n"RESET);

  fprintf(stdout, DEBUG"Starting Program Interpretation...\n"RESET);
  fprintf(stdout, ERROR"\n----------------------------------\n"RESET);

  #ifdef LOGGING
    CREATE_LOG();
    fprintf(LOG, "[LOG INFO]: Log File Created...\n");
  #endif

  if (interpretLines(source, source_length, 0))
  {
    fprintf(stdout, ERROR"----------------------------------\n"RESET);
    fprintf(stdout, SUCCESS"\nProgram Successfully Executed...\n"RESET);
  }
  else
  {
    fprintf(stderr, ERROR"----------------------------------\n"RESET);
    fprintf(stdout, DEBUG"\nProgram Terminated with an Error...\n"RESET);
  }

  #ifdef LOGGING
    fprintf(LOG, "[LOG INFO]: Log File Closed...\n");
    CLOSE_LOG();
  #endif

  free(source);

  destroySocket();
  
  freeSetup();
}

