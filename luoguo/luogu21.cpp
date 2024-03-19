#include <iostream>
#include <queue>
const int maxn = 1005;
bool inq[maxn][maxn] = {false};
int X[4] = {0, 0, 1, -1};
int Y[4] = {1, -1, 0, 0};
char sea[maxn][maxn] = {0};
int islands = 0;
int N;
struct node {
  int x, y;
} Node;
bool check(int x, int y) {
  if (x >= N || x < 0 || y >= N || y < 0) {
    return true;
  }
  return false;
}

void search(int x, int y) {
  Node.x = x, Node.y = y;
  std::queue<node> q;

  q.push(Node);
  while (!q.empty()) {
    auto top = q.front();
    q.pop();
    int i = 0;
    int count = 0;
    for (; i < 4; i++) {
      auto new_x = top.x + X[i];
      auto new_y = top.y + Y[i];
      if (check(new_x, new_y)) {
        continue;
      } else if (sea[new_x][new_y] == '#') {
        count++;
        if (!inq[new_x][new_y]) {
          inq[new_x][new_y] = 1;
          Node.x = new_x, Node.y = new_y;
          q.push(Node);
        }
      }
    }
    if (count == 4) {
      islands++;
    }
  }
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(0), std::cout.tie(0);
  std::cin >> N;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      std::cin >> sea[i][j];
    }
  }
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (sea[i][j] == '#' && !inq[i][j]) {
        inq[i][j] = true;
        search(i, j);
      }
    }
  }
  std::cout << islands;
  return 0;
}