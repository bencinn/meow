#include "ast.h"
#include "jim2.h"
#include <stdio.h>

char *create_random_buffer();

void write_start_of_json(FILE *f);
void write_middle_of_json(FILE *f);
void write_end_of_json(FILE *f);

// please create random string for random_id and others alike.
void write_start(Jim *f, char *random_id, char *next_id);
void generate_variable_int(Jim *j, int n, char *name, char *random_id);
void generate_variable_str(Jim *j, char *val, char *name, char *random_id);
void write_blocks(Jim *j, NODE *n, char *new_id, char *prev_id, char *next_id);
