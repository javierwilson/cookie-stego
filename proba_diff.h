

#define MAX_HANDLE 256

// display the name o the module
int hello_module();

// test program argument
int test_param(int *dct);

// itialize memory usage
int init(void); 

// free memory
int release(void);

// ready to restart new computations
int reset(void);


// compute from a dct
int compute(int *dct);

// extract the 324 features
int extract_features(float *result);

// create a probability array from the difference
// and return the handle
//int new_proba_array(int min_x, int min_y, int max_x, int max_y);

//int set_value(int handle, int x, int y, int value);

//int check_array(int handle);

int set_debug(int enable);

