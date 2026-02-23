#include "wish.h"
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

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
      	parse_line(buffer);
      	lineNum++;
    }	
}

void run_batch(const char *filename) {
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
	while(line[i] != '\0') {      //had to remove the \n to use strcmp
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
	if(strcmp(argv[0], "cd") == 0) {  //checks that the 1st two characters "cd"
		if(argc != 2) {           //if there is more than one word seperated by spaces, error
			print_error();
			return;
		}
		if(chdir(argv[1]) != 0) {
			print_error();
		}
		return;
	}
	run_external(argv);
}

void print_error() {
	const char *error = "An error has occurred\n";
	write(STDERR_FILENO, error, strlen(error));       //ChatGPT claims this is safer than just printf
}

void run_external(char *argv[]) {

	if(argv == NULL || argv[0] == NULL) return;    //safety gaurd if empty
	char path[200];
	snprintf(path, sizeof(path), "/bin/%s", argv[0]);  //got this from ChatGPT
													   // you can store the string into a buffer
	pid_t pid = fork();
	if(pid < 0) {
		print_error();
		return;
	}

	if(pid == 0) {
		execv(path, argv);
		print_error();
		_exit(1);
	} else {
		waitpid(pid, NULL, 0);
	}
}



