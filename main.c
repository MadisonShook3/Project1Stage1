#include "wish.h"
#include <string.h>

int main(int argc, char *argv[]) {
    if(argc == 1) {
        run_interactive();
        return 0;
    }
    if(argc == 2) {
        return run_batch(argv[1]);
    }
    print_error();
    return 1;
}
