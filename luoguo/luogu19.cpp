#include <algorithm>
#include <climits>
#include <iostream>
const int maxn = 10;
int arr[maxn][maxn] = {0};
bool hashtable[maxn][maxn] = {false};
int X[4] = {0, 0, 1, -1};
int Y[4] = {1, -1, 0, 0};
int n, m;
long long sum = 0;
int min = INT_MAX;

bool check(int x, int y) {
  if (x >= n || x < 0 || y >= m || y < 0) {
    return false;
  }
  if (hashtable[x][y] == true) {
    return false;
  }
  return true;
}

void dfs(int x, int y, int count, long long now_sum) {
  if (now_sum > sum) {
    return;
  }
  if (now_sum == sum) {
    min = std::min(min, count);
    return;
  } else {
    if (check(x, y)) {
      hashtable[x][y] = true;
      for (int i = 0; i < 4; i++) {
        auto new_x = x + X[i];
        auto new_y = y + Y[i];
        dfs(new_x, new_y, count + 1, now_sum + arr[x][y]);
        hashtable[x][y] = false;
      }
    }
  }
}

int main() {
  std::cin >> m >> n;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      std::cin >> arr[i][j];
      sum += arr[i][j];
    }
  }
  sum /= 2;
  dfs(0, 0, 0, 0);
  std::cout << min;
}