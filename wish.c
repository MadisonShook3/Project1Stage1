#include "wish.h"

void run_interactive(void) {
  char buffer[100];  //stores user input
  int lineNum = 1;

    while(1) {
      printf("wish> ");
      fflush(stdout);  //forces it to print

      //read the user input. buffer = where text gets stored.
      //stdin = keyboard input.
      if(fgets(buffer, sizeof(buffer), stdin) == NULL) {
        break;
      }
      printf("line%d: %s", lineNum, buffer);
      lineNum++;
    }
}

void run_batch(const char *filename) {
  char buffer[100]; 
  FILE *file = fopen(filename, "r");
  if(file == NULL) {
    printf("error\n");
    return;
  }
  int lineNum = 1;

  while(fgets(buffer, sizeof(buffer), file) != NULL) {
    printf("line%d: %s", lineNum, buffer);
    lineNum++;
  }
  fclose(file);
}

