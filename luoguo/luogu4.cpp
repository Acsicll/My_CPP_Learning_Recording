#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

int main() {
  int N;
  std::cin >> N;
  std::vector<long long> P;
  long long temp = 0;
  for (int i = 0; i < N; i++) {
    std::cin >> temp;
    P.push_back(temp);
  }
  std::sort(P.begin(), P.end());
  auto count = N;
  if (N == 1) {
    std::cout << count;
  } else {
    int d = P[1] - P[0];
    for (int i = 1; i < N; i++) {
      if ((P[0] + i * d) == P[i]) {
        count++;
      }
    }
  }
  std::cout << count;
  return 0;
}