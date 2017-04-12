

#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>

#include "projL1Inf.c"

int main(int argc, char** argv) {
  if (argc <3) {
    printf("usage: runprojection <nrows> <ncols>\n");
    exit(1);
  }
  int nrows = atoi(argv[1]);
  int ncols = atoi(argv[2]);
  
  double* A = malloc(sizeof(double)*nrows*ncols);
  double* B = malloc(sizeof(double)*nrows*ncols);
  
  // generate random A
  int max = pow(2,31)-1;
  double* pA = A;
  int i;
  for (i=0; i<nrows*ncols; ++i) {
    *pA = (double) random()/max; 
    ++pA;
  }

  // generate weights
  double* w = malloc(sizeof(double)*nrows);
  for (i=0; i<nrows; ++i) {
    w[i] = 1;
  }

  int printmatrices = 0;
  if (nrows*ncols<=100) printmatrices = 1;

  double norm = normL1Inf(A, nrows, ncols);
  printf("Input Matrix A: norm=%f\n", norm);
  if (printmatrices) {
    printMatrix(A, nrows, ncols);
    printf("\n");
  }


  time_t  t0, t1; /* time_t is defined on <time.h> and <sys/types.h> as long */
  clock_t c0, c1; /* clock_t is defined on <time.h> and <sys/types.h> as int */


  int T = 9;
  double F[9] = {0.01, 0.05, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7};

  for (i=0; i<T; ++i) {
    double f = F[i];

    t0 = time(NULL);
    c0 = clock();
    projL1Inf(B, f*norm, A, w, nrows, ncols);
    t1 = time(NULL);
    c1 = clock();

    double pnorm = normL1Inf(B, nrows, ncols);
    printf("Projected Matrix B: factor=%f  norm=%f  time = %f\n", f, pnorm, (float) (c1 - c0)/CLOCKS_PER_SEC);

    if (printmatrices) {
      printMatrix(B, nrows, ncols);
      printf("\n");
    }
  }
  

  
  
  //  printf("max = %i, a = %f\n", max, a); 


}
