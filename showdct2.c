#include <stdio.h>
#include <jpeglib.h>
#include <setjmp.h>

struct my_error_mgr {
  struct jpeg_error_mgr pub;	/* "public" fields */
  jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;

void my_error_exit (j_common_ptr cinfo)
{
  my_error_ptr myerr = (my_error_ptr) cinfo->err;
  (*cinfo->err->output_message) (cinfo);
  longjmp(myerr->setjmp_buffer, 1);
}

int main(int argc, char *argv[]) {
  int k;
  char * filename = argv[1];
  struct jpeg_decompress_struct cinfo;
  struct my_error_mgr jerr;
  FILE * infile;		/* source file */
  int row_stride;		/* physical row width in output buffer */
  JDIMENSION i, compnum, rownum, blocknum;
  jvirt_barray_ptr *coef_arrays;
  JBLOCKARRAY coef_buffers[MAX_COMPONENTS];

  int show_enable = 0;

  for(k=1;k<argc;k++) {
    if(!strcmp(argv[k],"-f")) {
      if(k+1<argc) {
	filename = argv[k+1];
	k++;
      }
    }
    if(!strcmp(argv[k],"-s")) {
      show_enable = 1;
    }
  }

  
  /* Open file */
  if ((infile = fopen(filename, "rb")) == NULL) {
    fprintf(stderr, "can't open %s\n", filename);
    return 0;
  }
  /* Allocate and initialize JPEG decompression object */
  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = my_error_exit;
  if (setjmp(jerr.setjmp_buffer)) {
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
    return 0;
  }
  /* Now we can initialize the JPEG decompression object. */
  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, infile);
  (void) jpeg_read_header(&cinfo, TRUE);
  /* Allocate memory ??? */
  for (compnum=0; compnum<cinfo.num_components; compnum++)
    coef_buffers[compnum] = ((&cinfo)->mem->alloc_barray) 
                            ((j_common_ptr) &cinfo, JPOOL_IMAGE,
                             cinfo.comp_info[compnum].width_in_blocks,
                             cinfo.comp_info[compnum].height_in_blocks);
  coef_arrays = jpeg_read_coefficients (&cinfo);
  /* Prints info */
  int len=0,len2=0;
  printf("components: %d  image width: %d  image height: %d\n", cinfo.num_components, cinfo.image_width, cinfo.image_height);
  for (compnum=0; compnum < cinfo.num_components; compnum++)
    printf("component: %d  width in blocks: %d  height in blocks: %d\n",
      compnum,cinfo.comp_info[compnum].width_in_blocks,cinfo.comp_info[compnum].height_in_blocks);
  for (compnum=0; compnum<cinfo.num_components; compnum++) {
    for (rownum=0; rownum<cinfo.comp_info[compnum].height_in_blocks; rownum++) {
      coef_buffers[compnum] = ((&cinfo)->mem->access_virt_barray) ((j_common_ptr) &cinfo, coef_arrays[compnum], rownum, (JDIMENSION) 1, FALSE);
      for (blocknum=0; blocknum<cinfo.comp_info[compnum].width_in_blocks; blocknum++) {
	if(show_enable) {
	  printf("Component: %d Row: %d Block: %d\n", compnum, rownum, blocknum);
	  
	  for (i=0; i<DCTSIZE2; i++) {
	    printf("%4d ", coef_buffers[compnum][0][blocknum][i]);
	    //printf("%4d ", coef_buffers[compnum][rownum][blocknum][i]);
	    if ((i+1)%8==0) { printf("\n"); }
	  }
	  printf("\n");
	}
      }
    }
  }
  (void) jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  fclose(infile);
  return 1;
}
