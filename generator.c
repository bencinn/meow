#include "log.h"
#include <stdio.h>
#include <stdlib.h>

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

// $ nm minimal.json.o
// 00000000000004c4 D _binary_minimal_json_end
// 00000000000004c4 A _binary_minimal_json_size
// 0000000000000000 D _binary_minimal_json_start
extern const char _binary_minimal_json_end;
extern const char _binary_minimal_json_size;
extern const char _binary_minimal_json_start;

void write_start_of_json(FILE *f)
{
	char *ptr = (char *)&_binary_minimal_json_start;
	while (ptr != &_binary_minimal_json_start + 501) {
		fputc(*ptr, f);
		ptr++;
	}
}
void write_middle_of_json(FILE *f)
{
	char *ptr = (char *)&_binary_minimal_json_start + 501;
	while (ptr != &_binary_minimal_json_start + 540) {
		fputc(*ptr, f);
		ptr++;
	}
}
void write_end_of_json(FILE *f)
{
	char *ptr = (char *)&_binary_minimal_json_start + 540;
	while (ptr != &_binary_minimal_json_end) {
		fputc(*ptr, f);
		ptr++;
	}
}

char *start = "\"opcode\":\"event_whenflagclicked\",\"next\":null,\"parent\":"
	      "null,\"inputs\":{},\"fields\":{},\"shadow\":false,\"topLevel\":"
	      "true,\"x\":0,\"y\":0";

void write_start(FILE *f)
{
	char *random_id = malloc(28 + 1);
	rand_str(random_id, 28);
	fprintf(f, "\"%s\":", random_id);
	fputc('{', f);
	fputs(start, f);
	fputc('}', f);
	free(random_id);
}
