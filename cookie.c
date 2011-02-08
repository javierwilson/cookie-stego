#define DLEN 8
#include "extract_dct.c"
#include "matrix.h"
//#include "mod_rmf.h"
#include "mod_dumb.h"
#include "mod_dumb.c"
#define NB_FEATURES 9*9*4
#define MAXLEN_FILENAME 1000

int main(int argc, char *argv[]) {
  int size, feature;
  char * filename = argv[1];

  // extract DCT's
  size = extract_dct(filename);
  printf("*** Number of DCTs: %d ***\n", size);

  // initializes module
  dumb_init();

  // compute sum (dumb feature) for each DCT
  int x,y,z;
  dumb_reset(size);
  for (z=0; z<size; z++) {
    dumb_compute(dct[z]);
  }

  // get features (average of sum in this case)
  feature = dumb_get_features();
  printf("Average of sum of DCTs = %d\n", feature);

  // free allocations
  dumb_release();
}
