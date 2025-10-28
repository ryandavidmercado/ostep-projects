#include <assert.h>
#include <stdint.h>
#include <stdio.h>

int main(int argc, char *argv[argc]) {
  if(argc < 2) {
    puts("wunzip: file1 [file2 ...]");
    return 1;
  }

  uint32_t chrCount = 0;
  int chr = 0;

  for(size_t i = 1; i < argc; ++i) {
    char const *filename = argv[i];
    FILE *file = fopen(filename, "r");

    if(!file) {
      fprintf(stderr, "Failed to open %s: ", filename);
      perror(0);

      return 1;
    }

    while(fread(&chrCount, 4, 1, file) == 1) {
      if(fread(&chr, 1, 1, file) != 1) {
        fprintf(stderr, "Failed to read character for corresponding chrCount %d", chrCount);
        return 1;
      }

      for(size_t i = 0; i < chrCount; ++i) {
        putc(chr, stdout);
      }
    };

    if(!feof(file)) {
      fprintf(stderr, "Error while reading file %s: ", filename);
      perror(0);

      return 1;
    }

    fclose(file);
  }

  return 0;
}
