#include "ast.h"
#include "generator.h"
#include "log.h"
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

extern int yylex();
extern int yyparse();
extern FILE *yyin;

NODE *final;

int main(int argc, char **argv)
{
	if (argc != 3) {
		printf("Usage: %s <INPUT> <OUTPUT>\n", argv[0]);
		return 1;
	}

	log_trace("opening file %s", argv[1]);
	FILE *f = fopen(argv[1], "r");
	if (f == NULL) {
		log_error("failed to open file %s", argv[1]);
		return 1;
	}

	log_trace("opening folder %s", argv[2]);
	DIR *dir = opendir(argv[2]);
	if (dir) {
	} else if (ENOENT == errno) {
		log_error("path %s is not a folder, exiting...", argv[2]);
	} else {
		log_error("failed to open folder %s", argv[2]);
	}

	yyin = f;
	yyparse();

	fclose(f);
	log_trace("closed file %s", argv[1]);

	log_debug("final: %p", final);

	// since Scratch require (?) the assetId to be the md5sum, but doesn't
	// require the asset file to be valid, we just do $ md5sum <empty file>
	// and Scratch 3 will load it, and display a question mark because wtf
	// did we just do
	char *png_path = malloc(
	    sizeof(argv[2]) + sizeof("/68b329da9893e34099c7d8ad5cb9c940.png"));

	sprintf(png_path, "%s/68b329da9893e34099c7d8ad5cb9c940.png", argv[2]);
	FILE *png = fopen(png_path, "w");

	if (png == NULL) {
		log_error("failed to open <empty>.png at %s", png_path);
		return 1;
	}

	fclose(png);
	log_trace("closed file %s", png_path);

	char *project_json_path =
	    malloc(sizeof(argv[2]) + sizeof("/project.json"));
	sprintf(project_json_path, "%s/project.json", argv[2]);

	FILE *project_json = fopen(project_json_path, "w");
	if (project_json == NULL) {
		log_error("failed to open project.json at %s",
			  project_json_path);
		return 1;
	}

	write_start_of_json(project_json);

	Jim variables = {};
	jim_object_begin(&variables);
	generate_variable_int(&variables, 12, "test var");
	jim_object_end(&variables);

	// TODO: this is probably safe (?) but in case fseek fail how the fuck
	// will we recover anyways. maybe i should just crash and don't care but
	// maybe idk
	fseek(project_json, -1, SEEK_CUR);
	fwrite(variables.sink, variables.sink_count, 1, project_json);
	fseek(project_json, -1, SEEK_CUR);

	write_middle_of_json(project_json);
	Jim opcodes = {};
	jim_object_begin(&opcodes);

	write_start(&opcodes);
	write_start(&opcodes);

	jim_object_end(&opcodes);

	fseek(project_json, -1, SEEK_CUR);
	fwrite(opcodes.sink, opcodes.sink_count, 1, project_json);
	fseek(project_json, -1, SEEK_CUR);

	write_end_of_json(project_json);

	fclose(project_json);
	log_trace("closed file %s", project_json_path);

	closedir(dir);
	log_trace("closed folder %s", argv[2]);
}
