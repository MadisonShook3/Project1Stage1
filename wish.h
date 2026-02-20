#include <stdio.h>
#include <stdlib.h>

void run_interactive(void);

void run_batch(const char *filename);

void parse_line(char *line);

void print_error();

void run_external(char *argv[]);
