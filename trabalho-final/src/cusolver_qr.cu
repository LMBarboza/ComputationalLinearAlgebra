#include <cstdio>
#include <cstdlib>
#include <cublas_v2.h>
#include <cuda_runtime.h>
#include <cusolverDn.h>
#include <vector>

int main(int argc, char *argv[]) {
  cusolverDnHandle_t cusolverH = NULL;
  cublasHandle_t cublasH = NULL;
  cudaStream_t stream{};

  const int m = 3;
  const int lda = m;
  const int ldb = m;
  const int nrhs = 1;

  const std::vector<double> A = {1.0, 4.0, 2.0, 2.0, 5.0, 1.0, 3.0, 6.0, 1.0};
  const std::vector<double> B = {6.0, 15.0, 4.0};
  std::vector<double> XC(ldb * nrhs, 0);

  double *d_A = nullptr;
  double *d_tau = nullptr;
  double *d_B = nullptr;
  int *d_info = nullptr;
  double *d_work = nullptr;

  int lwork_geqrf = 0;
  int lwork_ormqr = 0;
  int lwork = 0;
  int info = 0;

  const double one = 1;

  std::printf("A = (matlab base-1)\n");
  std::printf("=====\n");
  std::printf("B = (matlab base-1)\n");
  std::printf("=====\n");

  /* step 1: create cudense/cublas handle */
  cusolverDnCreate(&cusolverH);
  cublasCreate(&cublasH);

  cudaStreamCreateWithFlags(&stream, cudaStreamNonBlocking);
  cusolverDnSetStream(cusolverH, stream);
  cublasSetStream(cublasH, stream);

  cudaMalloc(reinterpret_cast<void **>(&d_A), sizeof(double) * A.size());
  cudaMalloc(reinterpret_cast<void **>(&d_tau), sizeof(double) * m);
  cudaMalloc(reinterpret_cast<void **>(&d_B), sizeof(double) * B.size());
  cudaMalloc(reinterpret_cast<void **>(&d_info), sizeof(int));

  cudaMemcpyAsync(d_A, A.data(), sizeof(double) * A.size(),
                  cudaMemcpyHostToDevice, stream);
  cudaMemcpyAsync(d_B, B.data(), sizeof(double) * B.size(),
                  cudaMemcpyHostToDevice, stream);

  cusolverDnDgeqrf_bufferSize(cusolverH, m, m, d_A, lda, &lwork_geqrf);

  cusolverDnDormqr_bufferSize(cusolverH, CUBLAS_SIDE_LEFT, CUBLAS_OP_T, m, nrhs,
                              m, d_A, lda, d_tau, d_B, ldb, &lwork_ormqr);

  lwork = std::max(lwork_geqrf, lwork_ormqr);

  cudaMalloc(reinterpret_cast<void **>(&d_work), sizeof(double) * lwork);

  cusolverDnDgeqrf(cusolverH, m, m, d_A, lda, d_tau, d_work, lwork, d_info);

  cudaMemcpyAsync(&info, d_info, sizeof(int), cudaMemcpyDeviceToHost, stream);

  cudaStreamSynchronize(stream);

  std::printf("after geqrf: info = %d\n", info);
  if (0 > info) {
    std::printf("%d-th parameter is wrong \n", -info);
    exit(1);
  }

  cusolverDnDormqr(cusolverH, CUBLAS_SIDE_LEFT, CUBLAS_OP_T, m, nrhs, m, d_A,
                   lda, d_tau, d_B, ldb, d_work, lwork, d_info);

  cudaMemcpyAsync(&info, d_info, sizeof(int), cudaMemcpyDeviceToHost, stream);

  cudaStreamSynchronize(stream);

  std::printf("after ormqr: info = %d\n", info);
  if (0 > info) {
    std::printf("%d-th parameter is wrong \n", -info);
    exit(1);
  }

  cublasDtrsm(cublasH, CUBLAS_SIDE_LEFT, CUBLAS_FILL_MODE_UPPER, CUBLAS_OP_N,
              CUBLAS_DIAG_NON_UNIT, m, nrhs, &one, d_A, lda, d_B, ldb);

  cudaMemcpyAsync(XC.data(), d_B, sizeof(double) * XC.size(),
                  cudaMemcpyDeviceToHost, stream);

  cudaStreamSynchronize(stream);

  std::printf("X = (matlab base-1)\n");

  /* free resources */
  cudaFree(d_A);
  cudaFree(d_tau);
  cudaFree(d_B);
  cudaFree(d_info);
  cudaFree(d_work);

  cublasDestroy(cublasH);
  cusolverDnDestroy(cusolverH);

  cudaStreamDestroy(stream);

  cudaDeviceReset();

  return EXIT_SUCCESS;
}
