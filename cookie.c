#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

#include "common.h"

#include "mod_rmf.h"
#include "mod_dumb.h"


struct option long_options[] = {
  {"module", 1, 0, 0},
  {"file", 1, 0, 1},
  {0, 0, 0, 0}
};

extern char *optarg;


extern int (*dct)[DLEN][DLEN];
int main(int argc, char *argv[]) {
  module_t *module;

  int size, feature;
  char filename[MAXLEN_FILENAME];
  char algoname[MAXLEN_ALGONAME];
  filename[0] = 0;
  
  int opt;
  int option_index = 0;

  // set default option

  strcpy(algoname,"dumb");


  for(;;) {
    opt = getopt_long (argc, argv, "012",
		       long_options, &option_index);

    if(opt==-1) break;
    printf("OPT = %d\n", opt); 
    switch (opt) {
    case 0:
      printf ("option %s", long_options[option_index].name);
      if (optarg)
	printf (" with arg %s", optarg);
      printf ("\n");
      if(strlen(optarg)<MAXLEN_ALGONAME) {
	strcpy(algoname,optarg);
      }
      else {
	printf("Error : algoname too long\n");
	exit(0);
      }
      break;
    case 1:
      printf ("option %s", long_options[option_index].name);
      if (optarg)
	printf (" with arg %s", optarg);
      printf ("\n");
      if(strlen(optarg)<MAXLEN_FILENAME) {
	strcpy(filename,optarg);
      }
      else {
	printf("Error : filename too long\n");
	exit(0);
      }
      break;
    }
  }

  if(filename[0]==0) {
    printf("please give a file name: %s --file=sample.jpg\n", argv[0]);
    exit(1);
  }


  // extract DCT's
  size = extract_dct(filename);
  printf("*** Number of DCTs: %d ***\n", size);


  if(!strcmp(algoname,"dumb")) {

    // initializes module
    dumb_init();
    
    // compute sum (dumb feature) for each DCT
    int x,y,z;
    dumb_reset(size);
    for (z=0; z<size; z++) {
      dumb_compute(dct[z]);
    }

  // get features (average of sum in this case)
    feature = dumb_get_features();
    printf("Average of sum of DCTs = %d\n", feature);

    // free allocations
    dumb_release();
  }

  else if(!strcmp(algoname,"rmf")) {
    int i;
    param_t param;
    float *tab;
    module_t *module;


    rmf_hello_module();
    param.nb_dct = size;

    // initializes module
    rmf_init();
    
    int x,y,z;

    module = rmf_get_module();
    tab = (float *) malloc(sizeof(float) * module->features);

    rmf_reset(&param);
    for (z=0; z<size; z++) {
      //printf("%d ",z);fflush(stdout);
      rmf_compute((int *) dct[z]);
      if(z % (size/100) == 0) {
      	printf("%d%% ", (z *100 / size));fflush(stdout);
      }
    }

    rmf_get_features(tab);
    for(i=0;i<module->features;i++) {
      printf("%f ",tab[i]);
    }

    // free allocations
    rmf_release();
    printf("rmf_release();\n");
  }

  return 0;

}
