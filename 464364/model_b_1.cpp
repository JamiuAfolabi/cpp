
#include <iostream>

using namespace std;

inline float mySqrt(float x) {
  if (x < 0)
    return 0;

  float result = x;
  for (int i = 0; i < 10; i++) {  // Assuming 10 iterations for convergence
    float temp = (result + x / result) / 2;
    result = temp;
  }

  return result;
}

int main() {
  float num = 16.0f;
  float root = mySqrt(num);
  cout << "Square root of " << num << " is: " << root << endl;
  return 0;
}
