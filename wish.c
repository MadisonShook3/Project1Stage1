#include "wish.h"
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

char *path_list[20];
int path_count = 0;
static int path_initialized = 0;

//Had to make this so that test 3 passed along with the others
void init_path() {
    if(path_initialized) return;

    for(int i = 0; i < 20; i++) {
        path_list[i] = NULL;
    }

    path_list[0] = malloc(strlen("/bin") + 1);

    if(path_list[0] == NULL) {
        print_error();
        exit(1);
    }
    strcpy(path_list[0], "/bin");
    path_count = 1;
    path_initialized = 1;
}

void run_interactive(void) {
    init_path();
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
        parse_line(buffer);
        lineNum++;
    }   
}

void run_batch(const char *filename) {
    init_path();
    char buffer[100]; 
    FILE *file = fopen(filename, "r");

    if(file == NULL) {
        print_error();
        return;
    }
    
    while(fgets(buffer, sizeof(buffer), file) != NULL) {
        parse_line(buffer);
    }
    fclose(file);
}

void parse_line(char *line) {
    char *argv[20]; //holds the arguments
    int argc = 0;   //argument count
    
    int i = 0;
    while(line[i] != '\0') {   //had to remove the \n to use strcmp
        if(line[i] == '\n' || line[i] == '\r') {
            line[i] = '\0';
            break;
        }
        i++;
    }
    
    int in_word = 0;    //currently inside a word
    i = 0;
    while(line[i] != '\0') {
        if(line[i] == ' ' || line[i] == '\t') {  
            line[i] = '\0';
            in_word = 0;
        } else if(!in_word) {
            if(argc >= 19) {
                print_error();
                return;
            }
            argv[argc++] = &line[i];
            in_word = 1;
        }
        i++;
    }

    argv[argc] = NULL;   //have to set back to null
    if(argc == 0) return; 

    if(strcmp(argv[0], "exit") == 0) {
        if(argc != 1) {
            print_error();
            return;
        }
        exit(0);
    } 
    if(strcmp(argv[0], "cd") == 0) {  
        if(argc != 2) {          
            print_error();
            return;
        }
        if(chdir(argv[1]) != 0) {
            print_error();
        }
        return;
    }
    if(strcmp(argv[0], "path") == 0) {
        path_count = 0;
        for(int j = 0; j < 20; j++) {   //20 max paths
            if(path_list[j] != NULL) {
                free(path_list[j]);
                path_list[j] = NULL;
            }
        }
        if(argc == 1) {  //returns if no other arguments
            return;
        } else if(argc > 1) {
            for(int i = 1; i < argc && i <= 20; i++) {  
            //i = 1 skips "path", i < argc stops at last argument
                char *copy = malloc(strlen(argv[i]) + 1); 
                strcpy(copy, argv[i]);
                path_list[i - 1] = copy;   //i - 1 fills list at index 0
                path_count++;
            }
        }
        return;
    }
    run_external(argv);
}

void print_error() {
    const char *error = "An error has occurred\n";
    write(STDERR_FILENO, error, strlen(error));       
                    //ChatGPT claims this is safer than just printf
}

void run_external(char *argv[]) {

    if(argv == NULL || argv[0] == NULL) return;   //safety gaurd if empty
    if(path_count == 0) {
        print_error();
        return;
    }
    for(int i = 0; i < path_count; i++) {
        if(path_list[i] == NULL) continue;
        char path[200];
        snprintf(path, sizeof(path), "%s/%s", path_list[i], argv[0]);  
        //snprintf I got from ChatGPT
        //formats and stores a series of chars into a buffer
        if(access(path, X_OK) == 0) {  
            //wasn't sure if I am allowed to use access function?
            //X_OK checks if command is executable, and access makes sure
            pid_t pid = fork();    //the file path exists and is usable
            if(pid < 0) {
                print_error();
                return;
            }

            if(pid == 0) {
                execv(path, argv);
                print_error();      //only runs if execv fails
                _exit(1);
            } else {
                waitpid(pid, NULL, 0);
                return;
            }
        }
    }
    print_error();
    return;
}



























