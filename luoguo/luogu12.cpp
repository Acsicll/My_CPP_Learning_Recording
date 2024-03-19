#include <iostream>

int main() {
  long long a, b, n;
  std::cin >> a >> b >> n;
  long long sum = 0;
  int i = 1;
  for (;; i++) {
    auto the_day = i % 7;
    if (the_day >= 1 && the_day <= 5) {
      sum += a;
    } else if (the_day == 0 || the_day == 6) {
      sum += b;
    }
    if (sum >= n) {
      break;
    }
  }
  std::cout << i;
}