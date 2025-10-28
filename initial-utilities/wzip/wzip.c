#include <assert.h>
#include <stdint.h>
#include <stdio.h>

int main(int argc, char *argv[argc]) {
  if(argc < 2) {
    puts("wzip: file1 [file2 ...]");
    return 1;
  }

  int chr = 0;
  uint32_t chrCount = 0;

  for(size_t i = 1; i < argc; ++i) {
    char const *filename = argv[i];
    FILE *file = fopen(filename, "r");

    if(!file) {
      fprintf(stderr, "Failed to open %s: ", filename);
      perror(0);

      return 1;
    }

    if(i == 1) {
      chr = fgetc(file);
      chrCount = 1;
    }

    while(true)  {
      int nextChr = fgetc(file);
      if(nextChr == EOF) {
        break;
      }

      if(nextChr != chr) {
        fwrite(&chrCount, 4, 1, stdout);
        fputc(chr, stdout);

        chrCount = 1;
      } else {
        chrCount++;
      }

      chr = nextChr;
    }
  }

  fwrite(&chrCount, 4, 1, stdout);
  fputc(chr, stdout);

  return 0;
}
