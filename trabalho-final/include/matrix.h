#ifndef MATRIX_H
#define MATRIX_H

#include <cmath>
#include <string>

struct Matrix {
  int nRows, nCols;
  double *matrixData;

  Matrix(int rows, int cols);
  ~Matrix(void);

  void fillMatrix(double mean = 0.0, double std = 1.0);
  void dumpMatrix(const std::string &filePath);
  void loadMatrix(const std::string &filePath);
  void printMatrix(void);

  Matrix operator-(Matrix &B);
};

double frobeniusNorm(Matrix &matrix);

#endif
