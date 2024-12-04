#include <cuda_runtime.h>
#include <cusolverDn.h>
#include <iostream>
#include <vector>

void printMatrix(int m, int n, const float *A, int lda, const char *name) {
  for (int row = 0; row < m; row++) {
    for (int col = 0; col < n; col++) {
      std::cout << name << "(" << row + 1 << "," << col + 1
                << ") = " << A[row + col * lda] << "\n";
    }
  }
}

void qrFactorization(const std::vector<float> &A, int m, int n) {
  cusolverDnHandle_t cusolverH = nullptr;
  cudaStream_t stream = nullptr;

  float *d_A = nullptr;
  float *d_tau = nullptr;
  float *d_work = nullptr;
  int *devInfo = nullptr;

  const int lda = m;
  int lwork = 0;
  int info_gpu = 0;

  cusolverDnCreate(&cusolverH);
  cudaStreamCreateWithFlags(&stream, cudaStreamNonBlocking);
  cusolverDnSetStream(cusolverH, stream);

  cudaMalloc((void **)&d_A, sizeof(float) * lda * n);
  cudaMalloc((void **)&d_tau, sizeof(float) * n);
  cudaMalloc((void **)&devInfo, sizeof(int));

  cudaMemcpy(d_A, A.data(), sizeof(float) * lda * n, cudaMemcpyHostToDevice);

  cusolverDnSgeqrf_bufferSize(cusolverH, m, n, d_A, lda, &lwork);
  cudaMalloc((void **)&d_work, sizeof(float) * lwork);

  cusolverDnSgeqrf(cusolverH, m, n, d_A, lda, d_tau, d_work, lwork, devInfo);
  cudaMemcpy(&info_gpu, devInfo, sizeof(int), cudaMemcpyDeviceToHost);

  if (info_gpu != 0) {
    std::cerr << "Error: " << info_gpu << std::endl;
  }

  cusolverDnSorgqr(cusolverH, m, n, n, d_A, lda, d_tau, d_work, lwork, devInfo);
  cudaMemcpy(&info_gpu, devInfo, sizeof(int), cudaMemcpyDeviceToHost);

  if (info_gpu != 0) {
    std::cerr << "Error " << info_gpu << std::endl;
  }

  std::vector<float> h_A(m * n);
  cudaMemcpy(h_A.data(), d_A, sizeof(float) * lda * n, cudaMemcpyDeviceToHost);

  std::vector<float> R(m * n, 0);
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j) {
      if (i <= j) {
        R[i + j * lda] = h_A[i + j * lda];
      }
    }
  }

  std::cout << "Q matrix:\n";
  printMatrix(m, n, h_A.data(), lda, "Q");

  std::cout << "R matrix:\n";
  printMatrix(m, n, R.data(), lda, "R");

  cudaFree(d_A);
  cudaFree(d_tau);
  cudaFree(d_work);
  cudaFree(devInfo);
  cusolverDnDestroy(cusolverH);
  cudaStreamDestroy(stream);
}

int main(void) {
  int m = 3;
  int n = 3;

  std::vector<float> A = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};

  qrFactorization(A, m, n);

  return 0;
}
