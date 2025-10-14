// #include "log.h"
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

char *start = "\"opcode\":\"event_whenflagclicked\",\"next\":null,\"parent\":"
	      "null,\"inputs\":{},\"fields\":{},\"shadow\":false,\"topLevel\":"
	      "true,\"x\":0,\"y\":0";

void write_start(Jim *j)
{
	char *random_id = malloc(sizeof(char) * (28 + 1));
	rand_str(random_id, 28);
	jim_member_key(j, random_id);
	free(random_id);
	jim_object_begin(j);
	{
		jim_member_key(j, "opcode");
		jim_string(j, "event_whenflagclicked");

		jim_member_key(j, "next");
		jim_null(j);

		jim_member_key(j, "parent");
		jim_null(j);

		jim_member_key(j, "inputs");
		jim_object_begin(j);
		jim_object_end(j);

		jim_member_key(j, "fields");
		jim_object_begin(j);
		jim_object_end(j);

		jim_member_key(j, "shadow");
		jim_bool(j, false);

		jim_member_key(j, "topLevel");
		jim_bool(j, true);

		jim_member_key(j, "x");
		jim_integer(j, 0);

		jim_member_key(j, "y");
		jim_integer(j, 0);
	}
	jim_object_end(j);
}

void generate_variable_int(Jim *j, int n, char *name)
{
	char *random_id = malloc(sizeof(char) * (28 + 1));
	rand_str(random_id, 28);
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

void generate_random_str(Jim *j, char *val, char *name)
{
	char *random_id = malloc(sizeof(char) * (28 + 1));
	rand_str(random_id, 28);
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
