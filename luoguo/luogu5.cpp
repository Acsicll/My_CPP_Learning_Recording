#include <iostream>

int main() {
  int n;
  std::cin >> n;
  int sum = 0;
  for (int i = 1; i <= n; i++) {
    auto num = i;
    while (num) {
      auto num1 = num % 10;
      if (num1 == 2 || num1 == 0 || num1 == 1 || num1 == 9) {
        sum += i;
        break;
      }
      num /= 10;
    }
  }
  std::cout << sum;
}