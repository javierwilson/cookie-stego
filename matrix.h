#define TYPE_INT    1
#define TYPE_FLOAT  2


int matrix_init(void);
int matrix_reset(int max_array);
int matrix_release(void);
int matrix_new(int min_x, int min_y, int max_x, int max_y, int type);
int matrix_delete(int handle);
//int matrix_set(int handle, int i, int j, void *value);
//int matrix_get(int handle, int i, int j, void *value);
int matrix_set_int(int handle, int i, int j, int value);
int matrix_get_int(int handle, int i, int j, int *value);
int matrix_set_float(int handle, int i, int j, float value);
int matrix_get_float(int handle, int i, int j, float *value);
