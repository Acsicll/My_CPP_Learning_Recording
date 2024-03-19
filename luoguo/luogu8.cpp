#include <iostream>
int main() {
  int n = 0;
  std::cin >> n;
  int sum = 0;
  int maxnum = 0;
  int minnum = 101;
  int num = 0;
  for (int i = 0; i < n; i++) {
    std::cin >> num;
    maxnum = std::max(maxnum, num);
    minnum = std::min(minnum, num);
    sum += num;
  }
  double avage = (int((sum * 1.0 / n * 1.0) * 100 + 0.5)) * 1.00 / 100;
  std::cout << maxnum << "\n" << minnum << "\n" << avage;
}