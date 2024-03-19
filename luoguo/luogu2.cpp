#include <bits/stdc++.h>
#include <iostream>
#include <sstream>
const int maxn = 100001;
bool hashtable[maxn] = {false};
int arr[101] = {0};
int main() {
  std::stringstream ss;
  int N;
  std::cin >> N;
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  int count = 0;
  int res2 = 0;
  for (int i = 0; i < N; i++) {
    std::string line;
    std::getline(std::cin, line);
    ss.clear();
    ss.str(line);
    int num;
    while (ss >> num) {
      if (hashtable[num] == false) {
        hashtable[num] = true;
        arr[count++] = num;
      } else {
        res2 = num;
      }
    }
  }
  std::sort(arr, arr + count);
  int res1 = 0;
  for (unsigned i = 0; i < count - 1; i++) {
    if (arr[i] != arr[i + 1] - 1) {
      res1 = arr[i] + 1;
      break;
    }
  }
  printf("%d %d", res1, res2);
}