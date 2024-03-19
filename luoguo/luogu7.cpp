#include <iostream>

int main() {
  long long n = 0;
  std::cin >> n;
  while (n) {
    std::cout << n << " ";
    n /= 2;
  }
  return 0;
}