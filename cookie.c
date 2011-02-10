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
  {"svm", 1, 0, 2},
  {"steg", 0, 0, 3},
  {"help", 0, 0, 4},
  {0, 0, 0, 0}
};

extern char *optarg;

void printhelp(myname) {
  printf("Usage: %s --file=example.jpg [--module=algo] [--svm=file.ds] [--steg] [--help]\n", myname);
  printf("      --file=example.jpg sets jpeg image file to analyze.\n");
  printf("      --module=algo sets the algorithm to be used.\n");
  printf("               currently supported algos: dumb, histo, rmf.\n");
  printf("      --svm=file.ds sets the output svm data file to write to.\n");
  printf("      --steg sets stego flag on in the svm data file.\n");
  printf("      --help prints this text.\n");
  exit(0);
}

extern int (*dct)[DLEN][DLEN];
int main(int argc, char *argv[]) {
  module_t *module;
  void *result; // future : use for rmf,dumb,histo : 
                // it is an array of everything
  int type;     // float,int,double... for result[]
  int size_result;


  int steg = 0;

  int size, feature;
  char filename[MAXLEN_FILENAME];
  char svmfilename[MAXLEN_FILENAME];
  int outfile;
  char algoname[MAXLEN_ALGONAME];
  filename[0] = 0;
  svmfilename[0] = 0;  
  int opt;
  int option_index = 0;

  FILE *svmfile = NULL;
    


  // set default module option

  strcpy(algoname,"dumb");


  for(;;) {
    opt = getopt_long (argc, argv, "01234",
		       long_options, &option_index);

    if(opt==-1) break;
    switch (opt) {
    case 0: // --module=modulename
      //printf ("option %s", long_options[option_index].name);
      if (optarg) {
	if(strlen(optarg)<MAXLEN_ALGONAME) {
	strcpy(algoname,optarg);
	}
      }
      else {
	printf("Error : module name too long\n");
	exit(0);
      }
      break;
    case 1: // --file=filename
      if (optarg) {
	if(strlen(optarg)<MAXLEN_FILENAME) {
	  strcpy(filename,optarg);
	}
	else {
	  printf("Error : filename too long\n");
	  exit(0);
	}
      }
      break;
      
    case 2: // --svm=filename
      if (optarg) {
	if(strlen(optarg)<MAXLEN_FILENAME) {
	  strcpy(svmfilename,optarg);
	}
	else {
	  printf("Error : svm filename too long\n");
	  exit(0);
	}
      }
      break;

    case 3: // --steg
      if (optarg) {
	steg = atoi(optarg);
      }
      break;

    case 4: // --help
      printhelp(argv[0]);
      break;
    }
  }

  if(filename[0]==0) {
    printf("please give a file name.\n");
    printhelp(argv[0]);
    exit(1);
  }


  // extract DCT's
  size = extract_dct(filename);
  //printf("*** Number of DCTs: %d ***\n", size);

  /*********************** dumb **************************/
  if(!strcmp(algoname,"dumb")) {
    dumb_hello_module();

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

  /*********************** histo **************************/
  if(!strcmp(algoname,"histo")) {
    int *tab; // used for result

    printf("applying ");
    histo_hello_module();

    // initializes module
    histo_init();
    
    // compute histogram for each DCT
    int x,y,z;
    histo_reset(size);
    for (z=0; z<size; z++) {
      histo_compute(dct[z]);
    }

    // get features (histogram)
    size_result = histo_get_count();
    int *numbers;
    numbers = malloc(size_result*sizeof(int));
    result = (void *) malloc(size_result*sizeof(int));
    tab = (int *) result;
    histo_get_features(numbers, tab);

    // free allocations
    histo_release();
  }

  /*********************** rmf **************************/
  else if(!strcmp(algoname,"rmf")) {
    int i;
    param_t param;
    float *tab; // used for rmf = result

    printf("applying ");
    rmf_hello_module();
    param.nb_dct = size;

    // initializes module
    rmf_init();
    
    int x,y,z;

    module = rmf_get_module();
    result = (void *) malloc(sizeof(float) * module->features);
    tab = (float *) result;
    rmf_reset(&param);
    for (z=0; z<size; z++) {
      //printf("%d ",z);fflush(stdout);
      rmf_compute((int *) dct[z]);
      if(z % (size/100) == 1) {
      	printf("%d%% ", (1 + z *100 / size));fflush(stdout);
      }
    }
    printf("\n");

    rmf_get_features(tab);
    size_result = module->features;
    // free allocations
    rmf_release();
  }


  /*********************** SVM **************************/
  if(svmfilename[0]!=0) {
    int i;
    svmfile = fopen(svmfilename,"a");
    if(svmfile != NULL) {
      fprintf(svmfile, "%f ",(float) steg);
      
      if(!strcmp(algoname,"histo")) {
	int *tab = result;
	for(i=0;i<size_result;i++) {
	  fprintf(svmfile, "%d:%d ",i+1, tab[i]);
	}
        free(result);
      }
      if(!strcmp(algoname,"rmf")) {
	float *tab = result;
	for(i=0;i<size_result;i++) {
	  fprintf(svmfile, "%d:%f ",i+1, tab[i]);
	}
        free(result);
      }
      fprintf(svmfile, "\n");
      fclose(svmfile);
    }
    else {
      printf("Cannot open file %s \n", svmfilename);
    }
  }
  

  return 0;

}
