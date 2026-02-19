#include "wish.h"

int main(int argc, char *argv[]) {
  
  if(argc == 1) {
    run_interactive();
  } else if(argc == 2) {
      run_batch(argv[1]);
    } else {
        printf("error\n");
      }

  return 0;
}

