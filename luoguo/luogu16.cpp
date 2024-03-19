#include <iostream>
const int maxn = 100010;
int w[maxn], h[maxn];
int n, k;
int check(int x) {
  int ans = 0;
  for (int i = 0; i < n; i++) {
    ans += ((w[i] / x) * (h[i] / x));
  }
  return ans >= k;
}

int main() {
  std::cin >> n >> k;
  for (int i = 0; i < n; i++) {
    std::cin >> h[i] >> w[i];
  }
  int l = 1, r = 100000;
  while (l < r) {
    int mid = (l + r + 1) >> 1;
    if (check(mid))
      l = mid;
    else
      r = mid - 1;
  }
  std::cout << l;
}