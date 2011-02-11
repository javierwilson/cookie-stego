/*
 * unhide.c
 */

#include "hide.h"

int main (int argc, char **argv) {
  if (argc < 2) {
    printf("Usage: %s <input_jpeg>\n",argv[0]);
    return 1;
  }

  filenamei = argv[1];
  srcinfo.err = jpeg_std_error(&jsrcerr);
  jpeg_create_decompress(&srcinfo);
  if ((input_file = fopen(filenamei, "rb")) == NULL) {
    fprintf(stderr, "can't open input file %s\n", filenamei);
    return 1;
  }
  jpeg_stdio_src(&srcinfo, input_file);
  (void) jpeg_read_header(&srcinfo, TRUE);
  coef_arrays = jpeg_read_coefficients(&srcinfo);
  int l=MAX_SECRET_LEN,j=0,k=0,done=0;

  int total_size = 0;
  int size[srcinfo.num_components];
  for (compnum=1; compnum<srcinfo.num_components; compnum++) {
    size[compnum] = srcinfo.comp_info[compnum].width_in_blocks * srcinfo.comp_info[compnum].height_in_blocks;
    total_size += size[compnum];
  }

  int dctnum=0;
  int dist = (int)(total_size/310);
  int c1,c2,c3,c4,c5,c6,c7,c8;
  printf("Usable DCTs: %d Distribution: %d\n", total_size, dist);

  for (compnum=0; compnum<srcinfo.num_components; compnum++) {
    if (done == 1) { break; }
    for (rownum=0; rownum<srcinfo.comp_info[compnum].height_in_blocks; rownum++) {
      if (done == 1) { break; }
      row_ptrs[compnum] = ((&srcinfo)->mem->access_virt_barray) ((j_common_ptr) &srcinfo, coef_arrays[compnum], rownum, (JDIMENSION) 1, FALSE);
      for (blocknum=0; blocknum<srcinfo.comp_info[compnum].width_in_blocks; blocknum++) {
        if (done == 1) { break; }
        for (i=0; i<DCTSIZE2; i++) {
          //if(i==(DCTSIZE2-1) && j<l && (dctnum%dist==0) && compnum>0) {
          if(i==(DCTSIZE2-1) && j<l && compnum>0) {
            c1 = row_ptrs[compnum][0][blocknum][i];
            c2 = row_ptrs[compnum][0][blocknum][i-1];
            c3 = row_ptrs[compnum][0][blocknum][i-2];
            c4 = row_ptrs[compnum][0][blocknum][i-3];
            c5 = row_ptrs[compnum][0][blocknum][i-4];
            c6 = row_ptrs[compnum][0][blocknum][i-5];
            c7 = row_ptrs[compnum][0][blocknum][i-6];
            c8 = row_ptrs[compnum][0][blocknum][i-7];
            c1 <<= 7;
            c2 <<= 6;
            c3 <<= 5;
            c4 <<= 4;
            c5 <<= 3;
            c6 <<= 2;
            c7 <<= 1;
            k = c1|c2|c3|c4|c5|c6|c7|c8;
            secret[j] = k;
            secret[j+1] = 0;
            printf("Char: %c (%d) \n", k, k);
            j++;
            if (k == 0) { done = 1; break; }
          }
        }
        dctnum++;
      }
    }
  }
  l=strlen(secret);
  printf("Mensaje: %s Size: %d\n", secret, l);
  (void) jpeg_finish_decompress(&srcinfo);
  jpeg_destroy_decompress(&srcinfo);
  fclose(input_file);
  return 0;
}
