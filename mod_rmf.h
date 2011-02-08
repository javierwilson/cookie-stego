#include "module.h"

#define MAX_HANDLE 256

module_t * rmf_get_module(void);

// display the name o the module
int rmf_hello_module(void);

// test program argument
int rmf_test_param(int *dct);

// itialize memory usage
int rmf_init(void); 

// free memory
int rmf_release(void);

// ready to restart new computations
int rmf_reset(void);


// compute from a dct
int rmf_compute(int *dct);

// extract the 324 features
int rmf_get_features(float *result);

// create a probability array from the difference
// and return the handle
//int new_proba_array(int min_x, int min_y, int max_x, int max_y);

//int set_value(int handle, int x, int y, int value);

//int check_array(int handle);

int set_debug(int enable);

