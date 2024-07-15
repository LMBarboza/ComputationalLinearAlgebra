#include "../include/matrix.h"
#include "../include/timer.h"
#include <algorithm>
#include <iostream>
#include <lapack.h>
#include <lapacke.h>
#include <ostream>

/*extern "C" void dgeqrf_(int *M, int *N, double *A, int *LDA, double *TAU,
                        double *WORK, int *LWORK, int *INFO);
*/

void printTime(Timer *timer) {
  std::cout << "T(ms): " << timer->getTime() << std::endl;
  timer->reset();
}

void QR(Matrix *A, Timer *timer, int lvl = 2) {
  if (lvl == 2) {

    double *tau = new double[std::min(A->nRows, A->nCols)];
    timer->start();

    LAPACKE_dgeqrf(LAPACK_ROW_MAJOR, A->nRows, A->nCols, A->matrixData,
                   A->nCols, tau);

    timer->end();
    std::cout << "BLAS-2" << std::endl;

    printTime(timer);
    timer->reset();

  } else {
    int nb = 200;
    int ldt = A->nCols;
    double *T = new double[ldt * std::min(A->nRows, A->nCols)];
    double *work = (double *)LAPACKE_malloc(sizeof(double) * std::max(1, nb) *
                                            std::max(1, A->nRows));
    timer->start();

    LAPACKE_dgeqrt_work(LAPACK_ROW_MAJOR, A->nRows, A->nCols, nb, A->matrixData,
                        A->nCols, T, ldt, work);

    timer->end();
    std::cout << "BLAS-3" << std::endl;
    printTime(timer);
    timer->reset();
  }
}

int main(int argc, char *argv[]) {
  Timer timer;

  Matrix matrix250(250, 250);
  Matrix matrix500(500, 500);
  Matrix matrix1000(1000, 1000);

  matrix250.fillMatrix();
  matrix500.fillMatrix();
  matrix1000.fillMatrix();

  // matrix250.dumpMatrix("matrix250.bin");
  //  matrix500.dumpMatrix("matrix500.bin");
  //  matrix1000.dumpMatrix("matrix1000.bin");

  QR(&matrix250, &timer);
  QR(&matrix250, &timer, 3);

  QR(&matrix500, &timer);
  QR(&matrix500, &timer, 3);

  QR(&matrix1000, &timer);
  QR(&matrix1000, &timer, 3);

  //  matrix250.dumpMatrix("QR250.bin");
  //   matrix500.dumpMatrix("QR500.bin");
  //   matrix1000.dumpMatrix("QR1000.bin");

  return 0;
}
