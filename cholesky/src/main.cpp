#include <iostream> 
#include <random>
#include <cblas.h>
#include <lapack.h>

int N = 10;

void print_matrix(float* A){

  for (size_t i = 0; i < N; i++){
    for (size_t j = 0; j < N; j++){
      std::cout << A[i * N + j] << "";
    }
    std::cout << "\n";
  }

}

void fill_matrix(float* A){
  std::mt19937 rand; 
  std::normal_distribution<float> gaussian(0, 1);

  for (size_t i = 0; i < N; i++){
    for (size_t j = 0; j < N; j++){
      A[i * N + j]= gaussian(rand);
    }
  }
  

}

int main(int argc, char** argv){
  float* A = new float[N*N];
  float* C = new float[N*N];
  float* D = new float[N*N];
  int alpha = 1;
  int beta = 1;
  int info = 0;
  fill_matrix(A);
  print_matrix(A);
  //cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasTrans, N, N, N, alpha, A, N, A, N, beta, C, N);
  //LAPACK_spotrf_base("L", &N, D, &N, &info, 0);
  
  delete[] A;
  delete[] C;
  return 0;
}
