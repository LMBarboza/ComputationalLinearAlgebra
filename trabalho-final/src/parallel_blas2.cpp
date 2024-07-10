#include "../include/matrix.h"
#include "../include/timer.h"
#include <iostream>

extern "C" void dgeqrf_(int *M, int *N, double *A, int *LDA, double *TAU,
                        double *WORK, int *LWORK, int *INFO);

void QR(Matrix *A) {
  double *tau;
  double *work;
  int info;
  dgeqrf_(&A->nRows, &A->nCols, A->matrixData, &A->nRows, tau, &work, &A->nRows,
          &info);
}

template <typename T>
void printFrobenius(Matrix<T> &A, Matrix<T> &B) {
  Matrix<T> C = A - B;
  double norm = frobeniusNorm(C);
  std::cout << "Norma: " << norm << std::endl;

}

void printTime(Timer *timer) {
  std::cout << "T(ms): " << timer->getTime() << std::endl;
  timer->reset()
}

int main(void) {
  Timer timer;

  Matrix<double> matrix250(250, 250);
  Matrix<double> matrix500(500, 500);
  Matrix<double> matrix1000(1000, 1000);

  matrix250.loadMatrix("matrix250.bin");
  matrix500.loadMatrix("matrix500.bin");
  matrix1000.loadMatrix("matrix1000.bin");

  timer.start();
  QR(&matrix250);
  timer.end();
  printTime(&timer);

  timer.start();
  QR(&matrix500);
  timer.end();
  printTime(&timer);


  timer.start();
  QR(&matrix1000);
  timer.end();
  printTime(&timer);

  Matrix<double> teste250(250, 250);
  Matrix<double> teste500(500, 500);
  Matrix<double> teste1000(1000, 1000);

  teste250.loadMatrix("QR250.bin");
  teste500.loadMatrix("QR500.bin");
  teste1000.loadMatrix("QR1000.bin");

  printFrobenius(matrix250, teste250);
  printFrobenius(matrix500, teste500);
  printFrobenius(matrix1000, teste1000);

  return 0;
}
