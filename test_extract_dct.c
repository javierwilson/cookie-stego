#include "extract_dct.c"
#include "mod_dumb.c"
#define MAXLEN_FILENAME 1000

int main(int argc, char *argv[]) {
  int size, feature;
  char * filename = argv[1];
  size = extract_dct(filename);
  printf("*** Number of DCTs: %d ***\n", size);
  int x,y,z;
  for (z=0; z<size; z++) {
    // do sum
    feature = mod_dumb(dct[z]);
    printf("Sum of DCT # %d = %d\n", z+1, feature);
  }
}
