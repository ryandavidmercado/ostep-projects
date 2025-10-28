#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int grepFile(const char *needle, const char *filename, FILE *file) {
  if (!file) {
    puts("wgrep: cannot open file");
    return 1;
  }

  char *line = nullptr;
  size_t n = 0;

  while (getline(&line, &n, file) != -1) {
    if (strstr(line, needle)) {
      printf("%s", line);
    }
  };

  if (!feof(file)) {
    printf("Error reading %s: ", filename);
    perror(0);

    return 1;
  }

  if (fclose(file)) {
    printf("Failed to close %s: ", filename);
    perror(0);

    return 1;
  };

  return 0;
}

int main(int argc, char *argv[argc]) {
  if (argc < 2) {
    puts("wgrep: searchterm [file ...]");
    return 1;
  }

  char const *needle = argv[1];
  if (argc == 2) {
    return grepFile(needle, "stdin", stdin);
  }

  for (size_t i = 2; i < argc; ++i) {
    int ret = grepFile(needle, argv[i], fopen(argv[i], "r"));
    if (ret) {
      return ret;
    }
  }

  return 0;
}
