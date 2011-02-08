#include <stdio.h>
#include <stdlib.h>
#include "mod_rmf.h"
#include "error.h"
#include "matrix.h"

//return an array of 9*9*4 features from a dct 8*8

#define PROBA_DIFF_DEBUG(...) 

static int F;

static int M_h;
static int M_v;
static int M_d;
static int M_m;

static int F_h;
static int F_v;
static int F_d;
static int F_m;


int
rmf_hello_module()
{
  printf("reduced markov feature\n");
  return 0;
}

int
rmf_test_param(int *dct)
{
  int i;
  for(i=0;i<64;i++) {
    printf("%d ",dct[i]);
    if(i%8==7) printf("\n");
  }
}

module_t *module;

module_t * rmf_get_module(void)
{
  return module;
}



#define LIMIT 4

int rmf_init(void)
{
  module = (module_t *) malloc(sizeof(module_t));
  module->features = 9 * 9 * 4;

  matrix_init();

  F = matrix_new(0,7,0,7,TYPE_INT);

  // should be from -4 to 4 instead of -128 to LIMIT
  M_h = matrix_new(-LIMIT,LIMIT,-LIMIT,LIMIT,TYPE_FLOAT);
  M_v = matrix_new(-LIMIT,LIMIT,-LIMIT,LIMIT,TYPE_FLOAT);
  M_d = matrix_new(-LIMIT,LIMIT,-LIMIT,LIMIT,TYPE_FLOAT);
  M_m = matrix_new(-LIMIT,LIMIT,-LIMIT,LIMIT,TYPE_FLOAT);

  F_h = matrix_new(0,6,0,6,TYPE_INT);
  F_v = matrix_new(0,6,0,6,TYPE_INT);
  F_d = matrix_new(0,6,0,6,TYPE_INT);
  F_m = matrix_new(0,6,0,6,TYPE_INT);
  
  int * components = (int *) malloc(sizeof(int) * (2 * LIMIT + 1)); 

  return 0;
}

int
rmf_release(void)
{

  matrix_delete(F);

  matrix_delete(M_h);
  matrix_delete(M_v);
  matrix_delete(M_d);
  matrix_delete(M_m);
  
  matrix_delete(F_h);
  matrix_delete(F_v);
  matrix_delete(F_d);
  matrix_delete(F_m);

  matrix_release();
  
  free(module);

  return 0;
}

int
rmf_reset(void)
{
  return 0;
}

int
rmf_get_features(float *result)
{
  int i,j,k;
  float fvalue;

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
}

int
rmf_compute(int *dct)
{
  int i,j,k,u,v;

#if 0
  if(result_size<9*9*4) {
    return -1;
  }
#endif

  int S_u = 7; // not used
  int S_v = 7; // not used
  
  int denominator;
  int numerator;
  float sum;

  
  int value;
  int value1;
  int value2;


  matrix_set_int_from_array(F,dct);
  
  //  matrix_printf(F);
  
  // compute F_h, F_v, F_d and F_m
  // algo should be in matrix.c (done)
  
  // F_h
  for(j=0;j<6;j++) {
    for(i=0;i<6;i++) {
      matrix_get_int(F, i, j, &value1);
      matrix_get_int(F, i+1, j, &value2);
      value = value1 - value2;
      matrix_set_int(F_h, i, j, value);
    }
  }

  // F_v
  for(j=0;j<6;j++) {
    for(i=0;i<6;i++) {
      matrix_get_int(F, i, j, &value1);
      matrix_get_int(F, i, j+1, &value2);
      value = value1 - value2;
      matrix_set_int(F_h, i, j, value);
    }
  }

  // F_d
  for(j=0;j<6;j++) {
    for(i=0;i<6;i++) {
      matrix_get_int(F, i, j, &value1);
      matrix_get_int(F, i+1, j+1, &value2);
      value = value1 - value2;
      matrix_set_int(F_h, i, j, value);
    }
  }


  // F_m
  for(j=0;j<6;j++) {
    for(i=0;i<6;i++) {
      matrix_get_int(F, i+1, j, &value1);
      matrix_get_int(F, i, j+1, &value2);
      value = value1 - value2;
      matrix_set_int(F_h, i, j, value);
    }
  }

  // matrix M_h, M_v, M_d and M_m
  // compute denominator first, if 0 return 0 or max???

  // matrix M_h
  sum = 0.0;
  for(i=-LIMIT;i<=LIMIT;i++) {
    for(j=-LIMIT;j<=LIMIT;j++) {

      denominator = 0;
      for(u=0;u<6;u++) {
	for(v=0;v<7;v++) {
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
	for(u=0;u<6;u++) {
	  for(v=0;v<7;v++) {
	    matrix_get_int(F_h,u,v,&value);
	    if(value==i) {
	      matrix_get_int(F_h,u+1,v,&value);
	      if(value==j) {
		numerator++;
	      }
	    }
	  }
	}
	sum = sum + ((float)numerator)/((float) denominator);
	matrix_set_float(M_h,i,j,((float)numerator)/((float) denominator));
      }
      else {
	matrix_set_float(M_h,i,j,0.0); // not sure that is zero
      }
    }
  }
  printf("sum = %f\n", sum);
  matrix_printf(M_h);
  matrix_scale_float(M_h, 1.0/sum);
  matrix_printf(M_h);
 
 
  PROBA_DIFF_DEBUG("matrix M_h done\n");

  // matrix M_v
  for(i=-LIMIT;i<=LIMIT;i++) {
    for(j=-LIMIT;j<=LIMIT;j++) {

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
  

  return 0;
  
}
