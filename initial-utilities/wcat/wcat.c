#include <stdio.h>

#define BUFFER_SIZE 20

int main(int argc, char *argv[argc]) {
  for (size_t i = 1; i < argc; ++i) {
    FILE *file = fopen(argv[i], "r");
    if (!file) {
      puts("wcat: cannot open file");
      return 1;
    }

    char buffer[BUFFER_SIZE] = {};

    while (fgets(buffer, BUFFER_SIZE, file)) {
      printf("%s", buffer);
    }

    if (!feof(file)) {
      printf("Error reading %s: ", argv[i]);
      perror(0);

      return 1;
    }

    if (fclose(file)) {
      printf("Failed to close %s: ", argv[i]);
      perror(0);

      return 1;
    };
  }

  return 0;
}
