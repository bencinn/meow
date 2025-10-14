#include <stdio.h>
#include "jim2.h"

void write_start_of_json(FILE *f);
void write_middle_of_json(FILE *f);
void write_end_of_json(FILE *f);

void write_start(Jim *f);

void generate_variable_int(Jim *j, int n, char *name);
void generate_random_str(Jim *j, char* val, char *name);
