#include "../include/matrix.h"
#include "../include/timer.h"
#include <iostream>

extern "C" void dgeqrf_(int *M, int *N, double *A, int *LDA, double *TAU,
                        double *WORK, int *LWORK, int *INFO);

extern "C" void dgeqrt_(int *M, int *N, int * NB, double *A, int *LDA, double *T,
                        double *LDT, int *WORK, int *INFO);

int main(void) {
  Matrix<double> teste(200, 200);

  teste.fillMatrix();

  double *tau = new double[teste.nRows];
  int lwork = -1;
  double work;
  int info;
  dgeqrf_(&teste.nRows, &teste.nCols, teste.matrixData, &teste.nRows, tau, &work, &lwork, &info);

  return 0;
}
