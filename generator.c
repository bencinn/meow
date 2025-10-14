#include "generator.h"
#include "ast.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>

#define JIM_IMPLEMENTATION
#include "jim2.h"

// https://stackoverflow.com/a/15768317
void rand_str(char *dest, size_t length)
{
	char charset[] = "0123456789"
			 "abcdefghijklmnopqrstuvwxyz"
			 "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	while (length-- > 0) {
		size_t index = (double)rand() / RAND_MAX * (sizeof charset - 1);
		*dest++ = charset[index];
	}
	*dest = '\0';
}

char *create_random_buffer()
{
	char *buf = malloc(sizeof(char) * (28 + 1));
	rand_str(buf, 28);
	return buf;
}

// $ nm minify.o
// 00000000000003ff D _binary_minify_json_end
// 00000000000003ff A _binary_minify_json_size
// 0000000000000000 D _binary_minify_json_start
extern const char _binary_minify_json_end;
extern const char _binary_minify_json_size;
extern const char _binary_minify_json_start;

void write_start_of_json(FILE *f)
{
	char *ptr = (char *)&_binary_minify_json_start;
	while (ptr != &_binary_minify_json_start + 501) {
		fputc(*ptr, f);
		ptr++;
	}
}
void write_middle_of_json(FILE *f)
{
	char *ptr = (char *)&_binary_minify_json_start + 501;
	while (ptr != &_binary_minify_json_start + 540) {
		fputc(*ptr, f);
		ptr++;
	}
}
void write_end_of_json(FILE *f)
{
	char *ptr = (char *)&_binary_minify_json_start + 540;
	while (ptr != &_binary_minify_json_end) {
		fputc(*ptr, f);
		ptr++;
	}
}

void write_start(Jim *j, char *random_id, char *next_id)
{
	jim_member_key(j, random_id);

	jim_object_begin(j);
	{
		jim_member_key(j, "opcode");
		jim_string(j, "event_whenflagclicked");

		jim_member_key(j, "next");
		if (next_id == NULL)
			jim_null(j);
		else
			jim_string(j, next_id);

		// jim_member_key(j, "parent");
		// jim_null(j);

		// jim_member_key(j, "inputs");
		// jim_object_begin(j);
		// jim_object_end(j);

		// jim_member_key(j, "fields");
		// jim_object_begin(j);
		// jim_object_end(j);

		// jim_member_key(j, "shadow");
		// jim_bool(j, false);

		jim_member_key(j, "topLevel");
		jim_bool(j, true);

		// jim_member_key(j, "x");
		// jim_integer(j, 0);

		// jim_member_key(j, "y");
		// jim_integer(j, 0);
	}
	jim_object_end(j);
}

void write_block(Jim *j, char *opcode, char *new_id, char *prev_id,
		 char *next_id, ScratchArgs *sc, int args_count)
{
	jim_member_key(j, new_id);
	jim_object_begin(j);
	{
		jim_member_key(j, "opcode");
		jim_string(j, opcode);
		jim_member_key(j, "topLevel");
		jim_bool(j, false);

		jim_member_key(j, "next");
		if (next_id == NULL)
			jim_null(j);
		else
			jim_string(j, next_id);

		jim_member_key(j, "parent");
		jim_string(j, prev_id);

		jim_member_key(j, "inputs");
		jim_object_begin(j);
		for (int i = 0; i < args_count; i++) {
			jim_member_key(j, sc[i].name);
			jim_array_begin(j);

			jim_integer(j, 1);
			jim_array_begin(j);
			{
				switch (sc[i].t) {
				case STR:
					jim_integer(j, 10);
					jim_string(j, sc[i].u.string);
				default:
					log_error("ICE: argument type %d is "
						  "not implemented",
						  sc[i].t);
					break;
				}
			}
			jim_array_end(j);
			jim_array_end(j);
		}
		// {
		// 	jim_member_key(j, "MESSAGE");
		// 	jim_array_begin(j);
		// 	{
		// 		jim_integer(j, 3);

		// 		jim_array_begin(j);
		// 		jim_integer(j, 10);
		// 		jim_string(j, "yo");
		// 		jim_array_end(j);
		// 	}
		// 	jim_array_end(j);
		// }
		jim_object_end(j);
	}
	jim_object_end(j);
}

void generate_variable_int(Jim *j, int n, char *name, char *random_id)
{
	jim_member_key(j, random_id);
	{
		jim_array_begin(j);
		{
			jim_string(j, name);
			jim_integer(j, n);
		}
		jim_array_end(j);
	}
}

void generate_variable_str(Jim *j, char *val, char *name, char *random_id)
{
	jim_member_key(j, random_id);
	{
		jim_array_begin(j);
		{
			jim_string(j, name);
			jim_string(j, val);
		}
		jim_array_end(j);
	}
}
