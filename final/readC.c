#include <stdio.h>
#include <inttypes.h>
#include <unistd.h>

int main() {

  while (1) {

    FILE* f = fopen("xxx", "rb");
    int i;
    size_t lSize;
    fseek (f , 0 , SEEK_END);
    lSize = ftell (f);
    rewind (f);

    for (i=0; i<lSize; i++) {
      uint8_t buf;
      fread (&buf, 1, 1, f);
      printf("%d ", buf);
    }
    printf("\n");
    close(f);
    fflush (stdout);
    sleep(1);
  }

  return 0;
}
