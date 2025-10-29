#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

struct lineNode {
  char *line;
  struct lineNode *next;
};
typedef struct lineNode lineNode;

int main(int argc, char *argv[argc]) {
  if(argc > 3) {
    fprintf(stderr, "usage: reverse <input> <output>\n");
    return 1;
  }

  FILE *input = (argc > 1) ? fopen(argv[1], "r") : stdin;
  if(!input) {
    fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
    return 1;
  }

  FILE *output = (argc > 2) ? fopen(argv[2], "w") : stdout;
  if(!output) {
    fprintf(stderr, "reverse: cannot open file '%s'\n", argv[2]);
    return 1;
  }

  if(argc > 2) {
    struct stat input_info;
    struct stat output_info;

    int input_id = fileno(input);
    int output_id = fileno(output);

    if(fstat(input_id, &input_info)) {
      fprintf(stderr, "reverse: failed to get info for '%s'\n", argv[1]);
      return 1;
    } else if(fstat(output_id, &output_info)) {
      fprintf(stderr, "reverse: failed to get info for '%s'\n", argv[1]);
      return 1;
    }

    if(input_info.st_dev == output_info.st_dev && input_info.st_ino == output_info.st_ino) {
      fprintf(stderr, "reverse: input and output file must differ\n");
      return 1;
    }
  }

  lineNode *ln = nullptr;

  char *line = nullptr;
  size_t linecap = 0;

  while(getline(&line, &linecap, input) > 0) {
    lineNode *newLn = malloc(sizeof(lineNode));
    if(!newLn) {
      fprintf(stderr, "malloc failed");
      return 1;
    }

    newLn->line = strdup(line);
    newLn->next = ln;

    ln = newLn;
  }

  while(ln) {
    fputs(ln->line, output);
    ln = ln->next;    
  }

  if(argc > 1) {
    fclose(input);
  }

  if(argc > 2) {
    fclose(output);
  }

  return 0;
}
