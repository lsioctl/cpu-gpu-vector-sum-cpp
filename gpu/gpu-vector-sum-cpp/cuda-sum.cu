#include <iostream>
#include <cmath>
#include <chrono>

#include "cuda.h"

// function to retrieve cuda errors
// inspired from:
// https://stackoverflow.com/questions/14038589/what-is-the-canonical-way-to-check-for-errors-using-the-cuda-runtime-api
void gpuCheck(cudaError_t code, const char *file, int line, bool abort=true)
{
   if (code != cudaSuccess) 
   {
      fprintf(stderr,"GPUassert: %s %s %d\n", cudaGetErrorString(code), file, line);
      if (abort) exit(code);
   }
}

// Kernel function to add the elements of two arrays
__global__
void add(std::size_t numElements, float* a, float* b, float* result) {
  for (std::size_t i = 0; i < numElements; i++) {
    result[i] = a[i] + b[i]; 
  }
} 

int main(void)
{
  int N = 1<<20;

  // Allocate memory on the host
  float* vec1 = new float[N];
  float* vec2 = new float[N];
  float* vec3 = new float[N];

  // initialize x and y arrays on the host
  for (std::size_t i = 0; i < N; i++) {
    vec1[i] = 1.0f;
    vec2[i] = 2.0f;
    vec3[i] = 0.0f;
  }

  // Allocate device memory
  float* vec1_d;
  float* vec2_d;
  float* vec3_d;

  auto start = std::chrono::system_clock::now();

  gpuCheck(cudaMallocManaged(&vec1_d, N * sizeof(float)), __FILE__, __LINE__);
  gpuCheck(cudaMallocManaged(&vec2_d, N * sizeof(float)), __FILE__, __LINE__);
  gpuCheck(cudaMallocManaged(&vec3_d, N * sizeof(float)), __FILE__, __LINE__);

  // Transfer data from host to device memory
  gpuCheck(cudaMemcpy(vec1_d, vec1, N * sizeof(float), cudaMemcpyHostToDevice), __FILE__, __LINE__);
  gpuCheck(cudaMemcpy(vec2_d, vec2, N * sizeof(float), cudaMemcpyHostToDevice), __FILE__, __LINE__);
  gpuCheck(cudaMemcpy(vec3_d, vec3, N * sizeof(float), cudaMemcpyHostToDevice), __FILE__, __LINE__);

  // Run kernel on 1M elements on the GPU
  add<<<1, 1>>>(N, vec1_d, vec2_d, vec3_d);

  // Wait for GPU to finish before accessing on host
  gpuCheck(cudaDeviceSynchronize(), __FILE__, __LINE__);

  // Copy the result to host memory
  gpuCheck(cudaMemcpy(vec3, vec3_d, N * sizeof(float), cudaMemcpyDeviceToHost), __FILE__, __LINE__);

  auto end = std::chrono::system_clock::now();
  auto elapsed = end - start;

  std::cout << "cuda single thread add duration: " << elapsed.count() << " nanoseconds" << std::endl;


  // Check for errors (all values should be 3.0f)
  float maxError = 0.0f;
  for (int i = 0; i < N; i++) {
    maxError = fmax(maxError, fabs(vec3[i] - 3.0f));
  }
  std::cout << "Max error: " << maxError << std::endl;

  // Free host memory
  delete [] vec1;
  delete [] vec2;
  delete [] vec3;

  // Free device memory
  cudaFree(vec1_d);
  cudaFree(vec2_d);
  cudaFree(vec3_d);
  
  return 0;
}