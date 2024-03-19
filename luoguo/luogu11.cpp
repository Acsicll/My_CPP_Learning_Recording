#include <iostream>

int main() {
  int n;
  std::cin >> n;
  int count = 0;
  int thenum = int(n * 1.0 / 2 * 1.0 + 0.5);
  for (long long i = 1; i < n; i++) {
    if (i < 5 && double(i * i % n) < thenum) {
      count++;
    } else if (((i % n) * (i % n)) % n < thenum) {
      count++;
    }
  }
  printf("%d", count);
}