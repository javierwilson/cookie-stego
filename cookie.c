#include "extract_dct.h"
#include "mod_rmf.h"
#include "mod_dumb.h"

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
