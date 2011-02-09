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
  int l=255,j=0,k=0,done=0;
  for (compnum=0; compnum<srcinfo.num_components; compnum++) {
    if (done == 1) { break; }
    for (rownum=0; rownum<srcinfo.comp_info[compnum].height_in_blocks; rownum++) {
      if (done == 1) { break; }
      row_ptrs[compnum] = ((&srcinfo)->mem->access_virt_barray) ((j_common_ptr) &srcinfo, coef_arrays[compnum], rownum, (JDIMENSION) 1, FALSE);
      for (blocknum=0; blocknum<srcinfo.comp_info[compnum].width_in_blocks; blocknum++) {
        if (done == 1) { break; }
        for (i=0; i<DCTSIZE2; i++) {
          if(i==(DCTSIZE2-1) && j<l) {
            k = row_ptrs[compnum][0][blocknum][i];
            if (k == 0) { done = 1; break; }
            secret[j] = k;
            secret[j+1] = 0;
            printf("Letra: %c (%d)\n", k, k);
            j++;
          }
        }
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
