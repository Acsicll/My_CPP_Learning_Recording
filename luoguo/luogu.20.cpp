#include <iostream>
const long long maxn = 1000005;
long long arr[maxn] = {0};
long long brr[maxn] = {0};
int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);
  long long N, K;
  std::cin >> N >> K;
  for (long long i = 1, temp; i <= N; i++) {
    std::cin >> temp;
    arr[i] = arr[i - 1] + temp;
  }
  long long ans = 0;
  for (long long i = 0; i <= N; i++) {
    ans += brr[arr[i] % K]++;
  }
  printf("%d", ans);
  return 0;
}