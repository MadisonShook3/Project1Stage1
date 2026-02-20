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
		if(line[i] == '\n') {
			line[i] = '\0';
			break;
		}
		i++;
	}
	
	int in_word = 0;    //currently inside a word
 	i = 0;
	while(line[i] != '\0') {
		if(line[i] != ' ' && in_word == 0) {  
			in_word = 1;
			argv[argc] = &line[i]; //start of word
			argc++;
		}
		if(line[i] == ' ') {
			in_word = 0;
			line[i] = '\0';   //ends each word
		}
		i++;
	}
	argv[argc] = NULL;   //have to set back to null
	if(argc == 0) return; 

	if(strcmp(argv[0], "exit") == 0) {
		exit(0);
	}else if(strcmp(argv[0], "cd") == 0) {  //checks that the 1st two characters "cd"
		if(argc != 2) {           //if there is more than one word seperated by spaces, error
			print_error();
		}
	}else {
		run_external(argv);
	}
	
}

void print_error() {
	printf("An error has occurred\n");
}

void run_external(char *argv[]) {
	char path[200];
	snprintf(path, sizeof(path), "/bin/%s", argv[0]);   //I got this line from ChatGPT, you can store the string into a buffer
	//runs other processes that aren't built in
	//fork, execv, wait
	pid_t pid = fork();      //fork a child process
	
	if(pid < 0) {
		print_error();
		return;
	} else if(pid == 0) {
		//child process executes command
		execv(path, argv);
		print_error();   //only if execv fails
		_exit(1);        //using _exit is better than exit after execv failure
	} else {
		wait(NULL);
	}		 
}	
































