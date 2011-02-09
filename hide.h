#include <stdio.h>
#include <jpeglib.h>
#include <string.h>
  struct jpeg_decompress_struct srcinfo;
  struct jpeg_compress_struct dstinfo;
  struct jpeg_error_mgr jsrcerr, jdsterr;
  jvirt_barray_ptr *coef_arrays;
  JDIMENSION i, compnum, rownum, blocknum;
  size_t block_row_size;
  JBLOCKARRAY coef_buffers[MAX_COMPONENTS];
  JBLOCKARRAY row_ptrs[MAX_COMPONENTS];
  FILE * input_file;
  FILE * output_file;
  char * filenamei;
  char * filenameo;
  char secret[256];

