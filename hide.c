/*
 * hide.c
 */

#include "hide.h"

int main (int argc, char **argv) {
  if (argc < 4) {
    printf("Usage: %s <input_jpeg> <output_jpeg> <message>\n",argv[0]);
    return 1;
  }

  filenamei = argv[1];
  filenameo = argv[2];
  strncpy(secret,argv[3],MAX_SECRET_LEN-1);
  srcinfo.err = jpeg_std_error(&jsrcerr);
  jpeg_create_decompress(&srcinfo);
  dstinfo.err = jpeg_std_error(&jdsterr);
  jpeg_create_compress(&dstinfo);

  jsrcerr.trace_level = jdsterr.trace_level;
  srcinfo.mem->max_memory_to_use = dstinfo.mem->max_memory_to_use;

  if ((input_file = fopen(filenamei, "rb")) == NULL) {
    fprintf(stderr, "can't open input file %s\n", filenamei);
    return 1;
  }
  if ((output_file = fopen(filenameo, "wb")) == NULL) {
    fprintf(stderr, "can't open output file %s\n", filenameo);
    return 1;
  }

 
  jpeg_stdio_src(&srcinfo, input_file);
  (void) jpeg_read_header(&srcinfo, TRUE);

  /*
   * Allocate memory to hold entire coefficient array for all components ???
   */
  int total_size = 0;
  int size[srcinfo.num_components];
  for (compnum=0; compnum<srcinfo.num_components; compnum++) {
    coef_buffers[compnum] = ((&dstinfo)->mem->alloc_barray) 
                            ((j_common_ptr) &dstinfo, JPOOL_IMAGE,
                             srcinfo.comp_info[compnum].width_in_blocks,
                             srcinfo.comp_info[compnum].height_in_blocks);
    if (compnum>0) {
      size[compnum] = srcinfo.comp_info[compnum].width_in_blocks * srcinfo.comp_info[compnum].height_in_blocks;
      total_size += size[compnum];
    }
  }

  /*
   * Read source file as DCT coefficients
   */
  coef_arrays = jpeg_read_coefficients(&srcinfo);

  /*
   * Initialize destination compression parameters from source values
   */
  jpeg_copy_critical_parameters(&srcinfo, &dstinfo);

  /*
   * Copy coefficients into new real array, inserting the secret message
   */
  int l=0,j=0,k=0;
  l=strlen(secret);

  int dctnum=0;
  int dist = (int)(total_size/l);
  int c1,c2,c3,c4,c5,c6,c7,c8;

  printf("Mensaje: %s\nMessage Size: %d\nUsable DCTs: %d Distribution: %d\n", secret, l, total_size, dist);
/*
11111111 = FF
10000000 = 80
01000000 = 40
00100000 = 20
00010000 = 10
00001000 = 08
00000100 = 04
00000010 = 02
00000001 = 01
*/

  for (compnum=0; compnum<srcinfo.num_components; compnum++) {
    block_row_size = (size_t) sizeof(JCOEF)*DCTSIZE2
                              *srcinfo.comp_info[compnum].width_in_blocks;
    for (rownum=0; rownum<srcinfo.comp_info[compnum].height_in_blocks; rownum++) {
      row_ptrs[compnum] = ((&dstinfo)->mem->access_virt_barray) 
                          ((j_common_ptr) &dstinfo, coef_arrays[compnum], 
                            rownum, (JDIMENSION) 1, FALSE);
      for (blocknum=0; blocknum<srcinfo.comp_info[compnum].width_in_blocks; blocknum++) {
        for (i=0; i<DCTSIZE2; i++) {
         /*
          * Writes ASCII code of character each end of DCT block (DCTSIZE-1)
          */
          //if(i==(DCTSIZE2-1) && j<=l && (dctnum%dist==0) && compnum>0) {
          if(i==(DCTSIZE2-1) && j<=l && compnum>0) {
            k=secret[j];
            c1=k&0x80; c1 >>= 7;
            c2=k&0x40; c2 >>= 6;
            c3=k&0x20; c3 >>= 5;
            c4=k&0x10; c4 >>= 4;
            c5=k&0x08; c5 >>= 3;
            c6=k&0x04; c6 >>= 2;
            c7=k&0x02; c7 >>= 1;
            c8=k&0x01;
            coef_buffers[compnum][rownum][blocknum][i-7] = c8;
            coef_buffers[compnum][rownum][blocknum][i-6] = c7;
            coef_buffers[compnum][rownum][blocknum][i-5] = c6;
            coef_buffers[compnum][rownum][blocknum][i-4] = c5;
            coef_buffers[compnum][rownum][blocknum][i-3] = c4;
            coef_buffers[compnum][rownum][blocknum][i-2] = c3;
            coef_buffers[compnum][rownum][blocknum][i-1] = c2;
            coef_buffers[compnum][rownum][blocknum][i] = c1;
            printf("Char: %c (%d) DCT: %d\n", k, k, dctnum);
            //printf("DCT Info: %d %d\n", dctnum, dist);
            j++;
          } else {
            coef_buffers[compnum][rownum][blocknum][i] = row_ptrs[compnum][0][blocknum][i];
          }
        }
        dctnum++;
        //printf("DCT # %d \n", dctnum);
      }
    }
  }

  /*
   * Write coefficient block rows back into virtual array
   */

  for (compnum=0; compnum<srcinfo.num_components; compnum++) {
    block_row_size = (size_t) sizeof(JCOEF)*DCTSIZE2
                              *srcinfo.comp_info[compnum].width_in_blocks;
    for (rownum=0; rownum<srcinfo.comp_info[compnum].height_in_blocks; rownum++) {
      row_ptrs[compnum] = ((&dstinfo)->mem->access_virt_barray) 
                          ((j_common_ptr) &dstinfo, coef_arrays[compnum], 
                           rownum, (JDIMENSION) 1, TRUE);
      memcpy(row_ptrs[compnum][0][0], coef_buffers[compnum][rownum][0], block_row_size);
    }
  }

  jpeg_stdio_dest(&dstinfo, output_file);
  jpeg_write_coefficients(&dstinfo, coef_arrays);

  jpeg_finish_compress(&dstinfo);
  jpeg_destroy_compress(&dstinfo);
  (void) jpeg_finish_decompress(&srcinfo);
  jpeg_destroy_decompress(&srcinfo);
  return 0;
}
