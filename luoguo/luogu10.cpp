#include <iostream>

bool check(int num) {
  int _4 = num % 10;
  num /= 10;
  int _3 = num % 10;
  num /= 10;
  int _2 = num % 10;
  num /= 10;
  int _1 = num % 10;
  if (_1 == _3 && _4 == _2 + 1) {
    return true;
  }
  return false;
}

int main() {
  int year;
  int count = 0;
  for (int i = 0; i < 5; i++) {
    std::cin >> year;
    if (check(year)) {
      count++;
    }
  }
  std::cout << count;
}