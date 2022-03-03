#include <iostream>
#include <cmath>
#include <chrono>
#include <immintrin.h>


/**
 * sum a and b vector and store the result in result vector
 * using sse
 */
void sse_add(std::size_t numElements, float* a, float* b, float* result) {
  // TODO: how to check alignement of the 3 vectors ?
  __m128* a128bits = (__m128*) a;
  __m128* b128bits = (__m128*) b;

  // We assume numElements % 4 == 0.                                                                                                                                                                                        
  std::size_t numIters = numElements / 4;

  // we are iterating on arrays of 128 bits elements
  for (std::size_t i = 0; i < numIters; i++) {
    // the first argument is a pointer of float (32 bits), but stores 4 elements
    _mm_store_ps(result + (i * 4), _mm_add_ps(*(a128bits + i), *(b128bits + i)));
  }
}

int main() {
  // as I am not used to it: left shift
  // a << b gives a * 2^b
  const int N = 1<<20; // around 1M elements
  //const int N = 32; // for testing

  // we use variable attribute of gcc
  // to force alignement
  float* __attribute__ ((aligned (16))) vec1 = new float[N];
  float* __attribute__ ((aligned (16))) vec2 = new float[N];
  float* __attribute__ ((aligned (16))) vec3 = new float[N];
  
  // initialize arrays on the host
  for (std::size_t i = 0; i < N; i++) {
    vec1[i] = 1.0f;
    vec2[i] = 2.0f;
    vec3[i] = 0.0f;
  }

  auto start = std::chrono::system_clock::now();
  sse_add(N, vec1, vec2, vec3);
  auto end = std::chrono::system_clock::now();
  auto elapsed = end - start;

  std::cout << "SSE add duration: " << elapsed.count() << " nanoseconds" << std::endl;

  // Check for errors (all values should be 3.0f)
  float maxError = 0.0f;
  for (std::size_t i = 0; i < N; i++) {
    maxError = fmax(maxError, fabs(vec3[i] - 3.0f));
    auto mark = vec3[i];
  }
  std::cout << "SSE add max error: " << maxError << std::endl;

  // Free memory
  delete [] vec1;
  delete [] vec2;
  delete [] vec3;

  return 0;
}