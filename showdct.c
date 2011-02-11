#include <stdio.h>
#include <jpeglib.h>

int main(int argc, char *argv[]) {
  char * filename = argv[1];
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  FILE * infile;
  JDIMENSION i, compnum, rownum, blocknum;
  jvirt_barray_ptr *coef_arrays;
  JBLOCKARRAY coef_buffers[MAX_COMPONENTS];

  if ((infile = fopen(filename, "rb")) == NULL) {
    fprintf(stderr, "can't open %s\n", filename);
    return 1;
  }
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, infile);
  (void) jpeg_read_header(&cinfo, TRUE);
  coef_arrays = jpeg_read_coefficients (&cinfo);
  int dctnum=0;
  printf("components: %d  image width: %d  image height: %d\n", cinfo.num_components, cinfo.image_width, cinfo.image_height);
  for (compnum=0; compnum < cinfo.num_components; compnum++)
    printf("component: %d  width in blocks: %d  height in blocks: %d\n",
      compnum,cinfo.comp_info[compnum].width_in_blocks,cinfo.comp_info[compnum].height_in_blocks);
  for (compnum=0; compnum<cinfo.num_components; compnum++) {
    for (rownum=0; rownum<cinfo.comp_info[compnum].height_in_blocks; rownum++) {
      coef_buffers[compnum] = ((&cinfo)->mem->access_virt_barray) ((j_common_ptr) &cinfo, coef_arrays[compnum], rownum, (JDIMENSION) 1, FALSE);
      for (blocknum=0; blocknum<cinfo.comp_info[compnum].width_in_blocks; blocknum++) {
        printf("DCT # %d. Component: %d Row: %d Block: %d\n", dctnum, compnum, rownum, blocknum);
        for (i=0; i<DCTSIZE2; i++) {
          printf("%4d ", coef_buffers[compnum][0][blocknum][i]);
          if ((i+1)%8==0) { printf("\n"); }
        }
        dctnum++;
        printf("\n");
      }
    }
  }
  (void) jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  fclose(infile);
  return 0;
}
