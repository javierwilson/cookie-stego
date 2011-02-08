#include <stdio.h>
#include "matrix.h"
#include "mod_rmf.h"
#include "module.h"

#define NB_FEATURES 9*9*4

//#define MATRIX_DEBUG(...) 
//#define MATRIX_DEBUG(...) fprintf(stderr, __VA_ARGS__)



int main(int argc, char **argv)
{
  int i,j;
  param_t param;

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

  param.nb_dct = 1;
  rmf_reset(&param);
  rmf_compute((int *) dct);
  rmf_get_features((float *) &tab);
    

  //  do_extraction((int *)dct, 8,8, &tab, NB_FEATURES);
  //  printf("done\n");

  return 0;
}

