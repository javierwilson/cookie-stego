

#define MAX_HANDLE 256


int F_h[9][9];
int F_v[9][9];
int F_d[9][9];
int F_m[9][9];


void init_module(void); 

// create a probability array from the difference
// and return the handle
int new_proba_array(int min_x, int min_y, int max_x, int max_y);

int set_value(int handle, int x, int y, int value);

int check_array(int handle);

int set_debug(int enable);
