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

int main(int argc, char **argv) {
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
    int n = 0;
    struct dirent *d;
    while ((d = readdir(dir)) != NULL) {
      if (++n > 2) {
        log_error("path %s is not empty, exiting...", argv[2]);
        return 1;
      }
    }
  } else if (ENOENT == errno) {
    log_error("path %s is not a folder, exiting...", argv[2]);
  } else {
    log_error("failed to open folder %s", argv[2]);
  }

  yyin = f;
  yyparse();

  fclose(f);
  log_trace("closed file %s", argv[1]);

  // since Scratch require (?) the assetId to be the md5sum, but doesn't require
  // the asset file to be valid, we just do $ md5sum <empty file> and Scratch 3
  // will load it, and display a question mark because wtf did we just do
  char *svg_path =
      malloc(sizeof(argv[2]) + sizeof("/d41d8cd98f00b204e9800998ecf8427e.svg"));
  sprintf(svg_path, "%s/d41d8cd98f00b204e9800998ecf8427e.svg", argv[2]);
  FILE *svg = fopen(svg_path, "w");

  if (svg == NULL) {
    log_error("failed to open <empty>.svg at %s", svg_path);
    return 1;
  }

  fclose(svg);
  log_trace("closed file %s", svg_path);

  char *project_json_path = malloc(sizeof(argv[2]) + sizeof("/project.json"));
  sprintf(project_json_path, "%s/project.json", argv[2]);

  FILE *project_json = fopen(project_json_path, "w");
  if (project_json == NULL) {
    log_error("failed to open project.json at %s", project_json_path);
    return 1;
  }

  write_start_of_json(project_json);
  write_middle_of_json(project_json);
  write_start(project_json);
  putc(',', project_json);
  write_start(project_json);
  write_end_of_json(project_json);

  fclose(project_json);
  log_trace("closed file %s", project_json_path);

  closedir(dir);
  log_trace("closed folder %s", argv[2]);
}
