int dumb_hello_module()
{
  printf("dumb non-existant feature\n");
  return 0;
}

int dumb_init(void)
{
  return 0;
}

int dumb_release(void)
{
  return 0;
}

int dumb_reset(void)
{
  return 0;
}

int dumb_compute(int adct[DLEN][DLEN]) {
  int x,y,z;
  int sum=0;
  for (x=0; x<DLEN; x++) {
    for (y=0; y<DLEN; y++) {
      sum += adct[x][y];
    }
  }
  return 0;
}

int dumb_get_features() {
  printf("DCT # %d of %d\n", z+1, size);
  for (x=0; x<DLEN; x++) {
    for (y=0; y<DLEN; y++) {
      printf("%4d ", dct[z][x][y]);
    }
    printf("\n");
  }
  return 0;
}
