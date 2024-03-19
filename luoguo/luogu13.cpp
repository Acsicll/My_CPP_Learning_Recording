#include <iostream>
#include <queue>
#include <vector>
const int maxn = 10010;
int martix[maxn][maxn] = {0};
bool inq[maxn][maxn] = {false};
int X[4] = {1, -1, 0, 0};
int Y[4] = {0, 0, 1, -1};
int n, w, m;
struct node {
  int x, y;
  int step;
} Node, S, T;

bool test(int x, int y) {
  if (x > w || x < 0 || y > T.y || y < 0) {
    return false;
  }
  if (martix[x][y] == 0) {
    return false;
  }
  if (inq[x][y] == true) {
    return false;
  }
  return true;
}

int bfs() {
  std::queue<node> Q;
  Q.push(S);
  while (!Q.empty()) {
    node top = Q.front();
    Q.pop();
    if (top.x == T.x && top.y == T.y) {
      return top.step;
    }
    for (int i = 0; i < 4; i++) {
      int NewX = top.x + X[i];
      int NewY = top.y + Y[i];
      if (test(NewX, NewY)) {
        Node.x = NewX;
        Node.y = NewY;
        Node.step = top.step + 1;
        Q.push(Node);
        inq[NewX][NewY] = true;
      }
    }
  }
  return -1;
}

int main() {
  std::cin >> w >> m >> n;
  if (m == n) {
    std::cout << 0;
    return 0;
  }
  auto end = std::max(m, n);
  auto start = std::min(m, n);
  if (w == 1 || w == 10000) {
    return end - start;
  }
  auto martix_size = 0;
  while (martix_size < end) {
    martix_size += w;
  }
  auto rows = martix_size / w;
  for (int i = 0, index = 1; i < rows; i++) {
    if (i & 1) {
      for (int j = w - 1; j >= 0; j--) {
        martix[i][j] = index++;
      }
    } else {
      for (int j = 0; j < w; j++) {
        martix[i][j] = index++;
      }
    }
  }
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < w; j++) {
      if (martix[i][j] == start) {
        S.x = i;
        S.y = j;
        S.step = 0;
      } else if (martix[i][j] == end) {
        T.x = i;
        T.y = j;
        break;
      }
    }
  }
  std::abs(T.y - S.y) + std::abs(S.x - T.x);
  printf("%d", bfs());
}