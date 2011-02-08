int mod_dumb(int adct[DLEN][DLEN]) {
  int x,y,z;
  int sum=0;
  for (x=0; x<DLEN; x++) {
    for (y=0; y<DLEN; y++) {
      sum += adct[x][y];
    }
  }
  return sum;
}
