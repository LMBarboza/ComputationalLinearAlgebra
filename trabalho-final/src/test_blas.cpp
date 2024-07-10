#include "../include/matrix.h"
#include "../include/timer.h"
#include <algorithm>
#include <iostream>
#include <lapacke.h>

/*extern "C" void dgeqrf_(int *M, int *N, double *A, int *LDA, double *TAU,
                        double *WORK, int *LWORK, int *INFO);
*/

void printTime(Timer *timer) {
  std::cout << "T(ms): " << timer->getTime() << std::endl;
  timer->reset();
}

void QR(Matrix *A, Timer *timer) {
  double *tau = new double[std::min(A->nRows, A->nCols)];
  timer->start();
  LAPACKE_dgeqrf(LAPACK_ROW_MAJOR, A->nRows, A->nCols, A->matrixData, A->nCols,
                 tau);
  timer->end();
  printTime(timer);
}

int main(int argc, char *argv[]) {
  Timer timer;

  Matrix matrix250(250, 250);
  Matrix matrix500(500, 500);
  Matrix matrix1000(1000, 1000);

  matrix250.fillMatrix();
  matrix250.printMatrix();
  matrix500.fillMatrix();
  matrix1000.fillMatrix();

  // matrix250.dumpMatrix("matrix250.bin");
  //  matrix500.dumpMatrix("matrix500.bin");
  //  matrix1000.dumpMatrix("matrix1000.bin");

  QR(&matrix250, &timer);

  QR(&matrix500, &timer);

  QR(&matrix1000, &timer);
  // matrix250.dumpMatrix("QR250.bin");
  //  matrix500.dumpMatrix("QR500.bin");
  //  matrix1000.dumpMatrix("QR1000.bin");

  return 0;
}
