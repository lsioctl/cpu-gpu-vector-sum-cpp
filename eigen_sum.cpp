#include <iostream>
#include <chrono>

#include <Eigen/Dense>


int main() {
  // as I am not used to it: left shift
  // a << b gives a * 2^b
  const int N = 1<<20; // around 1M elements
  //const int N = 4; // for testing

  Eigen::VectorXf vec1(N);
  Eigen::VectorXf vec2(N);
  Eigen::VectorXf vec3(N);

  vec1.fill(1.0);
  vec2.fill(2.0);
  vec3.fill(0.0);

  auto start = std::chrono::system_clock::now();
  vec3 = vec1 + vec2;
  auto end = std::chrono::system_clock::now();
  auto elapsed = end - start;

  std::cout << "Eigen add duration: " << elapsed.count() << " nanoseconds" << std::endl;

  // Check for errors (all values should be 3.0f)
  float maxError = 0.0f;
  for (std::size_t i = 0; i < N; i++) {
    maxError = fmax(maxError, fabs(vec3[i] - 3.0f));
  }
  std::cout << "Standard Max error: " << maxError << std::endl;

  return 0;
}