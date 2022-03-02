#include <iostream>
#include <cmath>
#include <chrono>
#include <immintrin.h>


/**
 * sum a and b vector and store the result in result vector
 * TODO: constness with C style pointers ?
 */
void simple_add(std::size_t numElements, float* a, float* b, float* result) {
  for (std::size_t i = 0; i < numElements; i++) {
    result[i] = a[i] + b[i]; 
  }
}

int main() {
  // as I am not used to it: left shift
  // a << b gives a * 2^b
  const int N = 1<<20; // around 1M elements
  //const int N = 4; // for testing

  float* vec1 = new float[N];
  float* vec2 = new float[N];
  float* vec3 = new float[N];
  
  // initialize arrays on the host
  for (std::size_t i = 0; i < N; i++) {
    vec1[i] = 1.0f;
    vec2[i] = 2.0f;
    vec3[i] = 0.0f;
  }

  auto start = std::chrono::system_clock::now();
  simple_add(N, vec1, vec2, vec3);
  auto end = std::chrono::system_clock::now();
  auto elapsed = end - start;

  std::cout << "standard add duration: " << elapsed.count() << " nanoseconds" << std::endl;

  // Check for errors (all values should be 3.0f)
  float maxError = 0.0f;
  for (std::size_t i = 0; i < N; i++) {
    maxError = fmax(maxError, fabs(vec3[i] - 3.0f));
  }
  std::cout << "Standard Max error: " << maxError << std::endl;

  // Free memory
  delete [] vec1;
  delete [] vec2;
  delete [] vec3;

  return 0;
}