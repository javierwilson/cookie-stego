#include <stdio.h>
#include "matrix.h"
#include "proba_diff.h"


#define NB_FEATURES 9*9*4

int main(int argc, char **argv)
{
  int i,j;


  float tab[NB_FEATURES];

  int dct[8][8] = { 
    { 45, 8, 6, 4, 2, 0, 0, 0 },
    { 4, 4, 2, 1, 0, 0, 0, 0 },
    { 3, 3, 1, 1, 0, 0, 0, 0 },
    { 2, 2, 0, 0, 0, 0, 0, 0 },
    { 1, 1, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 }};
  
  
  //  printf("do_extraction()...\n");

  rmf_hello_module();
  rmf_init();
  rmf_reset();
  rmf_compute((int *) dct);
  rmf_extract_features((float *) &tab);
    

  //  do_extraction((int *)dct, 8,8, &tab, NB_FEATURES);
  //  printf("done\n");

  return 0;
}

