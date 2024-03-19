#include <math.h>
#include <iostream>

bool checkSum(int a, int b, int c, int d, long long n) {
  if (a * a + b * b + c * c + d * d < n) {
    return true;
  }
  return false;
}

int main() {
  long long n = 0;
  std::cin >> n;
  for (int a = 0; checkSum(a, 0, 0, 0, n); a++) {
    for (int b = 0; checkSum(a, b, 0, 0, n); b++) {
      for (int c = 0; checkSum(a, b, c, 0, n); c++) {
        auto temp = n - a * a - b * b - c * c;
        int d = std::sqrt(temp);
        if (d * d == temp) {
          std::cout << a << " " << b << " " << c << " " << d;
          return 0;
        }
      }
    }
  }
}