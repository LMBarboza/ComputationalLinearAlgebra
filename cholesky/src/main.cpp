#include <iostream>
//#include <cblas.h>
#include <random>

constexpr int N = 10;

void print_matrix(float* A){

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
  fill_matrix(A);

  delete[] A;
  return 0;
}
