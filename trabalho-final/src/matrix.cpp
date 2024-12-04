#include "../include/matrix.h"
#include <fstream>
#include <iostream>
#include <random>
#include <stdexcept>

Matrix::Matrix(int rows, int cols) : nRows(rows), nCols(cols) {
  matrixData = new double[rows * cols];
}

Matrix::~Matrix() { delete[] matrixData; }

void Matrix::fillMatrix(double mean, double std) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::normal_distribution<double> dist(mean, std);

  for (int i = 0; i < nRows * nCols; ++i) {
    matrixData[i] = dist(gen);
  }
}

void Matrix::dumpMatrix(const std::string &filePath) {
  std::ofstream ofs(filePath, std::ios::binary);

  if (!ofs) {
    std::cerr << "Error abrindo arquivo" << std::endl;
    return;
  }

  ofs.write(reinterpret_cast<const char *>(&nRows), sizeof(nRows));
  ofs.write(reinterpret_cast<const char *>(&nCols), sizeof(nCols));
  ofs.write(reinterpret_cast<const char *>(matrixData),
            nRows * nCols * sizeof(double));

  ofs.close();
}

void Matrix::loadMatrix(const std::string &filePath) {
  std::ifstream ifs(filePath, std::ios::binary);

  if (!ifs) {
    std::cerr << "Error abrindo arquivo" << std::endl;
    return;
  }

  ifs.read(reinterpret_cast<char *>(&nRows), sizeof(nRows));
  ifs.read(reinterpret_cast<char *>(&nCols), sizeof(nCols));

  delete[] matrixData;
  matrixData = new double[nRows * nCols];
  ifs.read(reinterpret_cast<char *>(matrixData),
           nRows * nCols * sizeof(double));

  ifs.close();
}

void Matrix::printMatrix() {
  for (int i = 0; i < nRows; ++i) {
    for (int j = 0; j < nCols; ++j) {
      std::cout << matrixData[i * nCols + j] << " ";
    }
    std::cout << std::endl;
  }
}

Matrix Matrix::operator-(Matrix &B) {
  if (nRows != B.nRows || nCols != B.nCols) {
    throw std::invalid_argument("Erro de dimensão");
  }

  Matrix C(nRows, nCols);
  for (int i = 0; i < nRows * nCols; ++i) {
    C.matrixData[i] = matrixData[i] - B.matrixData[i];
  }
  return C;
}

double frobeniusNorm(Matrix &matrix) {
  double norm = 0.0;
  for (int i = 0; i < matrix.nRows * matrix.nCols; ++i) {
    norm += std::pow(std::abs(matrix.matrixData[i]), 2);
  }
  return std::sqrt(norm);
}
