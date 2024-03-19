#include <algorithm>
#include <iostream>

int main() {
  long long num = 0;
  int count = 0;
  std::cin >> num;
  int arr[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  while (1) {
    int A = 0;
    for (int i = 1; i <= 7; i++) {
      A = A * 10 + arr[i];
      int B = 0;
      for (int j = i + 1; j <= 8; j++) {
        B = B * 10 + arr[j];
        int C = 0;
        for (int k = j + 1; k <= 9; k++) {
          C = C * 10 + arr[k];
        }
        if (A + (C / B) == num && C % B == 0) {
          count++;
        }
      }
    }
    std::next_permutation(arr + 1, arr + 10);
    bool flag = false;
    for (int i = 1; i <= 9; i++) {
      if (arr[i] != i) {
        flag = true;
        break;
      }
    }
    if (!flag) {
      break;
    }
  }
  std::cout << count;
}