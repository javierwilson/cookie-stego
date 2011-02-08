static int *sum;
static int count=0;
static int tsum=0;

int dumb_hello_module()
{
  printf("dumb non-existant feature\n");
  return 0;
}

// Initializes module
int dumb_init(void)
{
  return 0;
}

// 
int dumb_release(void)
{
  free(sum);
  return 0;
}

// Initializes variables for each image
int dumb_reset(int nb_dct)
{
  sum = malloc(nb_dct*sizeof(int));
  return 0;
}

// Sum values
void dumb_compute(int adct[DLEN][DLEN]) {
  int x,y,z;
  sum[count] = 0;
  for (x=0; x<DLEN; x++) {
    for (y=0; y<DLEN; y++) {
      sum[count] += adct[x][y];
    }
  }
  printf("Sum of DCT # %d is %d\n",count,sum[count]);
  count++;
}

// Return average
int dumb_get_features() {
  int i;
  int average = 0;
  for(i=0; i<count; i++) {
    tsum += sum[i];
  }
  printf("TSUM %d COUNT %d\n", tsum, count);
  return tsum/count;
}
