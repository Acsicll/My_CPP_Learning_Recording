#include <iostream>

int main() {
  int n = 0;
  std::cin >> n;
  int add_C = 0;
  int drunk = 0;
  for (int i = 1; i <= n; i++) {
    if (!(i % 3)) {
      n += 1;
    }
    drunk++;
  }
  std::cout << drunk;
}