#include <algorithm>
#include <iostream>
const long long maxn = 500000;
long long n = 0;
long long a[maxn];
long long d[maxn];
long long count = 0;
long long index = 0;
void build(long s, long t, long long p) {
  if (s == t) {
    d[p] = a[s];
    count++;
    return;
  } else if (s < t) {
    for (int i = s; i <= t; i++) {
      d[p] += a[i];
    }
  }

  index++;
  long long mid = t - 1;
  build(s, mid, p + (2 * index - 1));
  build(mid + 1, t, p + (2 * index));
  index--;
  d[p] = d[p + (2 * index - 1)] + d[p + (2 * index)];
  if (d[p] == ((a[s] + a[t]) * (t - s + 1) / 2)) {
    count++;
  }
}

int main() {
  std::cin >> n;
  for (long long i = 0; i < n; i++) {
    std::cin >> a[i];
  }
  std::sort(a, a + n);
  build(0, n - 1, 0);
  std::cout << count;
}