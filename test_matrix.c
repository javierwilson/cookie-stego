#include <stdio.h>
#include "matrix.h"

int main(int argc, char **argv)
{
  int m1;
  int m2;
  int i,j;
  matrix_init();


  m1=matrix_new(-4,4,-4,4,TYPE_INT);
  if(m1<0) {
    printf("error : %d",m1);
  }
  if(m2<0) {
    printf("error : %d",m2);
  }
  
  m2=matrix_new(-4,4,-4,4,TYPE_INT);

  for(i=-4;i<=4;i++) {
    matrix_set_int(m1,i,-4,i);
  }

  matrix_printf(m1);
  
  matrix_release();

}
