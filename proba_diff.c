#include <stdio.h>
#include "proba_diff.h"
#include "error.h"
#include "matrix.h"

//return an array of 9*9*4 features from a dct 8*8

#define PROBA_DIFF_DEBUG(...) 

int
hello_module()
{
  printf("reduced markov feature\n");
  return 0;
}

int
test_param(int *dct)
{
  int i;
  for(i=0;i<64;i++) {
    printf("%d ",dct[i]);
    if(i%8==7) printf("\n");
  }
}

int
do_extraction(int *dct, int w, int h, float *result, int result_size)
{
  int i,j,k,u,v;

  if(result_size<9*9*4) {
    return -1;
  }

  matrix_init();

  int F = matrix_new(0,7,0,7,TYPE_INT);


  // should be from -4 to 4 instead of -128 to 127
  int M_h = matrix_new(-128,127,-128,127,TYPE_FLOAT);
  int M_v = matrix_new(-128,127,-128,127,TYPE_FLOAT);
  int M_d = matrix_new(-128,127,-128,127,TYPE_FLOAT);
  int M_m = matrix_new(-128,127,-128,127,TYPE_FLOAT);

  int F_h = matrix_new(0,7,0,7,TYPE_INT);
  int F_v = matrix_new(0,7,0,7,TYPE_INT);
  int F_d = matrix_new(0,7,0,7,TYPE_INT);
  int F_m = matrix_new(0,7,0,7,TYPE_INT);


  int S_u = 8;
  int S_v = 8;
  
  int denominator;
  int numerator;
  
  int value;
  int value1;
  int value2;

  float fvalue;



  matrix_set_int_from_array(F,dct);
  
  //  matrix_printf(F);
  
  // compute F_h, F_v, F_d and F_m
  // algo should be in matrix.c (done)
  
  // F_h
  for(j=0;j<8;j++) {
    for(i=0;i<7;i++) {
      matrix_get_int(F, i, j, &value1);
      matrix_get_int(F, i+1, j, &value2);
      value = value1 - value2;
      matrix_set_int(F_h, i, j, value);
    }
  }

  // F_v
  for(j=0;j<7;j++) {
    for(i=0;i<8;i++) {
      matrix_get_int(F, i, j, &value1);
      matrix_get_int(F, i, j+1, &value2);
      value = value1 - value2;
      matrix_set_int(F_h, i, j, value);
    }
  }

  // F_d
  for(j=0;j<7;j++) {
    for(i=0;i<7;i++) {
      matrix_get_int(F, i, j, &value1);
      matrix_get_int(F, i+1, j+1, &value2);
      value = value1 - value2;
      matrix_set_int(F_h, i, j, value);
    }
  }


  // F_m
  for(j=0;j<7;j++) {
    for(i=0;i<7;i++) {
      matrix_get_int(F, i+1, j, &value1);
      matrix_get_int(F, i, j+1, &value2);
      value = value1 - value2;
      matrix_set_int(F_h, i, j, value);
    }
  }

  // matrix M_h, M_v, M_d and M_m
  // compute denominator first, if 0 return 0 or max???

  // matrix M_h
  for(i=-128;i<=127;i++) {
    for(j=-128;j<=127;j++) {

      denominator = 0;
      for(u=1;u<S_u-2;u++) {
	for(v=1;v<S_v;v++) {
	  matrix_get_int(F_h,u,v,&value);
	  if(value==i) {
	    matrix_get_int(F_h,u+1,v,&value);
	    if(value==j) {
	      denominator++; // perhaps numerator can be computed here also
	    }
	  }
	}
      }
      if(denominator != 0) { 
      
	numerator = 0;
	for(u=1;u<S_u-2;u++) {
	  for(v=1;v<S_v;v++) {
	    matrix_get_int(F_h,u,v,&value);
	    if(value==i) {
	      matrix_get_int(F_h,u+1,v,&value);
	      if(value==j) {
		numerator++;
	      }
	    }
	  }
	}
	matrix_set_float(M_h,i,j,((float)numerator)/((float) denominator));
      }
      else {
	matrix_set_float(M_h,i,j,0.0); // not sure that is zero
      }
    }
  }
  PROBA_DIFF_DEBUG("matrix M_h done\n");

  // matrix M_v
  for(i=-128;i<=127;i++) {
    for(j=-128;j<=127;j++) {

      denominator = 0;
      for(u=1;u<S_u;u++) {
	for(v=1;v<S_v-2;v++) {
	  matrix_get_int(F_v,u,v,&value);
	  if(value==i) {
	    matrix_get_int(F_v,u,v+1,&value);
	    if(value==j) {
	      denominator++;
	    }
	  }
	}
      }
      if(denominator != 0) { 
      
	numerator = 0;
	for(u=1;u<S_u-2;u++) {
	  for(v=1;v<S_v;v++) {
	    matrix_get_int(F_v,u,v,&value);
	    if(value==i) {
	      matrix_get_int(F_v,u+1,v,&value);
	      if(value==j) {
		numerator++;
	      }
	    }
	  }
	}
	matrix_set_float(M_v,i,j,((float)numerator)/((float) denominator));
      }
      else {
	matrix_set_float(M_v,i,j,0.0); // not sure that is zero
      }
    }
  }

  PROBA_DIFF_DEBUG("matrix M_v done\n");
  
  for(j=-4,k=0;j<=4;j++) {
    for(i=-4;i<=4;i++,k++) {
      matrix_get_float(M_h,i,j,&fvalue);
      result[k] = fvalue;
    }
  }

  for(j=-4;j<=4;j++) {
    for(i=-4;i<=4;i++,k++) {
      matrix_get_float(M_v,i,j,&fvalue);
      result[k] = fvalue;
    }
  }

  for(j=-4;j<=4;j++) {
    for(i=-4;i<=4;i++,k++) {
      matrix_get_float(M_d,i,j,&fvalue);
      result[k] = fvalue;
    }
  }


  for(j=-4;j<=4;j++) {
    for(i=-4;i<=4;i++,k++) {
      matrix_get_float(M_m,i,j,&fvalue);
      result[k] = fvalue;
    }
  }

  return 0;
  //  matrix_printf(M_h);
  
}
