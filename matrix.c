#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "matrix.h"
#include "error.h"

#define MAX_ARRAYS 100

typedef struct array {

  int min_x;
  int min_y;
  int max_x;
  int max_y;
  int width;
  int height;
  
  int data_type;
  
  void *data;

} array_t;

array_t **handles;

int
matrix_init(void)
{
  int i;
  handles = (array_t **) malloc(sizeof(array_t *) * MAX_ARRAYS);

  // must be set to NULL
  // at least for python-ctype program binding

  //  memset((void *) handles, 0, sizeof(sizeof(array_t *) * MAX_ARRAYS));
  // NULL seems to be different than 0
  for(i=0;i<MAX_ARRAYS;i++) {
    handles[i] = NULL; 
  }
  
  return NO_ERROR;
}

int
matrix_release(void)
{
  if(handles!=NULL) free(handles);
  return NO_ERROR;
}

int
matrix_new(int min_x, int max_x, int min_y, int max_y, int type)
{
  int index;
  int size;

  array_t *array=NULL;
  // find the first free handle
  for(index=1;index<MAX_ARRAYS;index++) {
    if(handles[index]==NULL) {
      array = (array_t *) malloc(sizeof(array_t));
      if(array==NULL) { 
	printf("erreur\n");
	perror("malloc");
      }
      handles[index] = array;
      //      printf("OK\n");
      break;
    }
  }
  
  if(array==NULL) {
    //DEBUG_PROBA_DIFF("");
    printf("ERREUR FREE_MEMMORY\n");
    return ERROR_FREE_MEMORY;
  }


  array->min_x = min_x;
  array->min_y = min_y;
  array->max_x = max_x;
  array->max_y = max_y;
  
  array->width = max_x - min_x + 1;
  array->height = max_y - min_y + 1;

#if 0
  printf("[%d %d %d %d %d %d]", 
  	 array->min_x, array->max_x,
  	 array->min_y, array->max_y,
  	 array->width, array->height
  	 ); 
#endif

  array->data_type = type;
  
  if(array->data_type==TYPE_INT) {
    size = sizeof(int);
  }
  else if(array->data_type==TYPE_FLOAT) {
    size = sizeof(float);
  }

  array->data = (int *) malloc(size*(max_x-min_x+1)*(max_y-min_y+1));
  
  //  printf("new : %d %d\n", array->width, array->height);
  
  return index;

}


int
matrix_delete(int handle) 
{
  if(handle>MAX_ARRAYS) {
    return ERROR_OVERFLOW;
  }
  
  if(handles[handle] == NULL) {
    return NO_ERROR;
  }
  
  free(handles[handle]->data);
  free(handles[handle]);
  return;
}

static int compute_index(int handle, int i, int j) 
{
  int u,v;
  int idx;
  array_t *matrix;
  matrix = handles[handle];
  
  u = i - matrix->min_x;
  v = j - matrix->min_y;
  
  idx = (v * matrix->width) + u;
  return idx;
  
}

int
matrix_set_int_from_array(int handle, int *array)
{
  array_t *matrix;
  

  if(handle>=MAX_ARRAYS) {
    return ERROR_OVERFLOW;
  }
  
  if(handles[handle]==NULL) {
    return ERROR_NO_REFERENCE;
  }
  
  matrix = handles[handle];
  
  // should be check overflow...
  memcpy(matrix->data, (void *) array,
	 sizeof(int) * matrix->width * matrix->height);
  
  return 0;
}

//int matrix_copy(int h1, int h2, int x1, int x2,...
  
int
matrix_set_int(int handle, int i, int j, int value)
{
  array_t *matrix;
  int *data;
  int idx;

  if(handle>=MAX_ARRAYS) {
    return ERROR_OVERFLOW;
  }
  
  if(handles[handle]==NULL) {
    return ERROR_NO_REFERENCE;
  }
  
  matrix = handles[handle];
  
  if((matrix->min_x>i) || (matrix->min_y>j)
     ||(matrix->max_x<i) || (matrix->max_y<j)) {
    return ERROR_OVERFLOW;
  }

  data = (int *) matrix->data;
  
  idx = compute_index(handle, i, j);
  
  data[idx] = value;

  return NO_ERROR;
}


int matrix_get_int(int handle, int i, int j, int *value)
{
    array_t *matrix;
  int *data;
  int idx;

  if(handle>=MAX_ARRAYS) {
    return ERROR_OVERFLOW;
  }
  
  if(handles[handle]==NULL) {
    return ERROR_NO_REFERENCE;
  }
  
  matrix = handles[handle];
  
  if((matrix->min_x>i) || (matrix->min_y>j)
     ||(matrix->max_x<i) || (matrix->max_y<j)) {
    return ERROR_OVERFLOW;
  }

  data = (int *) matrix->data;
  
  idx = compute_index(handle, i, j);
  
  *value = data[idx];

  return NO_ERROR;

}

int
matrix_set_float(int handle, int i, int j, float value)
{
  array_t *matrix;
  float *data;
  int idx;

  if(handle>=MAX_ARRAYS) {
    return ERROR_OVERFLOW;
  }
  
  if(handles[handle]==NULL) {
    return ERROR_NO_REFERENCE;
  }
  
  matrix = handles[handle];
  
  if((matrix->min_x>i) || (matrix->min_y>j)
     ||(matrix->max_x<i) || (matrix->max_y<j)) {
    return ERROR_OVERFLOW;
  }

  data = (float *) matrix->data;
  
  idx = compute_index(handle, i, j);
  
  data[idx] = value;

  return NO_ERROR;
}


int matrix_get_float(int handle, int i, int j, float *value)
{
  array_t *matrix;
  float *data;
  int idx;

  if(handle>=MAX_ARRAYS) {
    return ERROR_OVERFLOW;
  }
  
  if(handles[handle]==NULL) {
    return ERROR_NO_REFERENCE;
  }
  
  matrix = handles[handle];
  
  if((matrix->min_x>i) || (matrix->min_y>j)
     ||(matrix->max_x<i) || (matrix->max_y<j)) {
    return ERROR_OVERFLOW;
  }

  data = (float *) matrix->data;
  
  idx = compute_index(handle, i, j);
  
  *value = data[idx];

  return NO_ERROR;

}


int matrix_printf(int handle)
{
  int i;
  int j;
  float value_float;
  int value_int;
  
  array_t *matrix=handles[handle];

  //  printf("[%d*%d]", matrix->width, matrix->height);
  for(j=matrix->min_y;j<=matrix->max_y;j++) {
    for(i=matrix->min_x;i<=matrix->max_x;i++) {
      if(matrix->data_type==TYPE_INT) {
	matrix_get_int(handle,i,j,&value_int);
	printf("%04d",value_int);
      }
      else if(matrix->data_type==TYPE_FLOAT) {
	matrix_get_float(handle,i,j,&value_float);
	printf("%08f",value_float);
      }
      printf(" ");
    }
    printf("\n");
  }
      
}

