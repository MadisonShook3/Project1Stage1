#include <stdio.h>
#include <stdlib.h>

void init_path(void);

void run_interactive(void);

int run_batch(const char *filename);

int check_redirection(char *argv[], int argc, char **out_file);

void setup_redirect(char *out_file);

int run_command(char *argv[], int argc);

void parse_line(char *line);

void print_error();

int run_external(char *argv[], char *out_file);
