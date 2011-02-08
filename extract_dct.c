#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <jpeglib.h>


#include "matrix.h"
#include "mod_rmf.h"
#define NB_FEATURES 9*9*4
#define DLEN 8
#define MAXLEN_FILENAME 1000

int (*dct)[DLEN][DLEN];

int extract_dct(char *filename) {
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  FILE * infile;
  JDIMENSION i, compnum, rownum, blocknum;
  jvirt_barray_ptr *coef_arrays;
  JBLOCKARRAY coef_buffers[MAX_COMPONENTS];

  if ((infile = fopen(filename, "rb")) == NULL) {
    fprintf(stderr, "can't open %s\n", filename);
    return 0;
  }
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, infile);
  (void) jpeg_read_header(&cinfo, TRUE);
  coef_arrays = jpeg_read_coefficients (&cinfo);

  // print basic info and get specific index values
  int total_size = 0;
  int size[3];
  printf("components: %d  image width: %d  image height: %d\n", cinfo.num_components, cinfo.image_width, cinfo.image_height);
  for (compnum=0; compnum < cinfo.num_components; compnum++) {
    size[compnum] = cinfo.comp_info[compnum].width_in_blocks * cinfo.comp_info[compnum].height_in_blocks;
    total_size += size[compnum];
    printf("component: %d  width in blocks: %d  height in blocks: %d DCTs: %d\n",
      compnum,cinfo.comp_info[compnum].width_in_blocks,cinfo.comp_info[compnum].height_in_blocks, size[compnum]);
  }
  printf("Total number of DCTs: %d\n", total_size);

  dct = malloc(total_size*DLEN*DLEN*sizeof(int));
  int index = 0;
  for (compnum=0; compnum<cinfo.num_components; compnum++) {
    for (rownum=0; rownum<cinfo.comp_info[compnum].height_in_blocks; rownum++) {
      coef_buffers[compnum] = ((&cinfo)->mem->access_virt_barray) ((j_common_ptr) &cinfo, coef_arrays[compnum], rownum, (JDIMENSION) 1, FALSE);
      for (blocknum=0; blocknum<cinfo.comp_info[compnum].width_in_blocks; blocknum++) {
        printf("Component: %d Row: %d Block: %d\n", compnum, rownum, blocknum);
        int j=0;
        for (i=0; i<DCTSIZE2; i++) {
          dct[index][j][i%DLEN] = coef_buffers[compnum][0][blocknum][i];
          printf("%4d ", coef_buffers[compnum][0][blocknum][i]);
          if ((i+1)%DLEN==0) { printf("\n"); j++; }
        }
        index++;
        printf("\n");
      }
    }
  }
  (void) jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  fclose(infile);
  return total_size;
}



struct option long_options[] = {
  {"module", 1, 0, 0},
  {"file", 1, 0, 0},
  {0, 0, 0, 0}
};

extern char *optarg;

/*
int main(int argc, char *argv[]) {
  int opt;
  int option_index = 0;

  char filename[MAXLEN_FILENAME];
  float tab[NB_FEATURES];
  int size, feature;

  filename[0] = 0;

  for(;;) {
    opt = getopt_long (argc, argv, "abc:d:012",
		       long_options, &option_index);

    if(opt==-1) break;
    switch (opt) {
    case 0:
      printf ("option %s", long_options[option_index].name);
      if (optarg)
	printf (" with arg %s", optarg);
      printf ("\n");
      if(strlen(optarg)<MAXLEN_FILENAME) {
	strcpy(filename,optarg);
      }
      else {
	printf("Error : filename too long");
	exit(0);
      }
      break;
    }
  }

  if(filename[0]==0) {
    printf("please give a file name\n");
  }

}
*/
