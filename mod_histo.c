#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "hash.h"

static int count=0;

// say hello
int histo_hello_module()
{
  printf("global histogram feature\n");
  return 0;
}

// Initializes module
int histo_init(void)
{
  return 0;
}

// free memory
int histo_release(void)
{
  //free(sum);
  return 0;
}

// Initializes variables for each image
int histo_reset(int nb_dct)
{
  inithashtab();
  return 0;
}

// Counts values
void histo_compute(int adct[DLEN][DLEN]) {
  int x,y,z;
  int current_value;
  for (x=0; x<DLEN; x++) {
    for (y=0; y<DLEN; y++) {
      current_value = hash_get(adct[x][y]);
      hash_set(adct[x][y], current_value+1);
      if(!current_value)
        count++;
    }
  }
}

// Returns how many *unique* values we have
int histo_get_count() {
  return count;
}

// Returns array of values
int histo_get_features(int *numbers, int *freq) {
  //hash_display();
  hash_get_keys(numbers, count);
  //hash_get_values(freq, count);
  int i;
  for(i=0; i<count; i++) {
    //printf("Number %d shown %d times\n",keys[i],hash_get(keys[i]));
    freq[i] = hash_get(numbers[i]);
    printf("%d:%d\n",numbers[i], freq[i]);
  }
  return 0;
}
