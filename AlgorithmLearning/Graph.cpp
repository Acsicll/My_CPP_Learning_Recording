#include <cstring>
#include <functional>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <vector>
namespace Graph {
// const int maxn = 1000;
// const int inf = 100000000;
// int n, G[maxn][maxn];
// bool vis[maxn] = {false};

// void dfs(int u, int depth) {
//   vis[u] = true;
//   for (int v = 0; v < u; v++) {
//     dfs(v, depth + 1);
//   }
// }
// void dfsTrave() {
//   for (int u = 0; u < n; u++) {
//     if (vis[u] == false) {
//       dfs(u, 1);
//     }
//   }
// }
// namespace Graph

// std::vector<int> Adj[maxn];
// int n;
// bool vis2[maxn] = {false};
// void dfs(int u, int depth) {
//   vis2[u] = true;
//   for (int i = 0; i < Adj[u].size(); i++) {
//     int v = Adj[u][i];
//     if (vis[v] == false) {
//       dfs(v, depth + 1);
//     }
//   }
//   void dfstrave() {
//     for (int u = 0; u < n; u++) {
//       if (vis2[u] == false) {
//         dfs(u, 1);
//       }
//     }
//   }
// }

void testfunc() {
  const int maxn = 2010;
  const int INF = 10000000;
  struct gang {
    std::string head;
    int numMember;
  } arrayGang[maxn];
  int numGang = 0;
  auto cmp = [](gang a, gang b) { return a.head < b.head; };
  std::map<int, std::string> intToString;  // name to id
  std::map<std::string, int> stringToInt;  // id to find name
  std::map<std::string, int> Gang;
  int G[maxn][maxn] = {0};
  int weight[maxn] = {0};
  int n, k, numPersion = 0;
  bool vis[maxn] = {false};
  std::function<void(int, int&, int&, int&)> dfs =
      [&](int nowVisit, int& head, int& numMember, int& totalvalue) {
        numMember++;
        vis[nowVisit] = true;
        if (weight[nowVisit] > weight[head]) {
          head = nowVisit;
        }
        for (int i = 0; i < numPersion; i++) {
          if (G[nowVisit][i] > 0) {
            totalvalue += G[nowVisit][i];
            G[nowVisit][i] = G[i][nowVisit] = 0;
            if (vis[i] == false) {
              dfs(i, head, numMember, totalvalue);
            }
          }
        }
      };
  std::function<void()> dfsTrave = [&]() {
    for (int i = 0; i < numPersion; i++) {
      if (vis[i] == false) {
        int head = i, numMember = 0, totalvalue = 0;
        dfs(i, head, numMember, totalvalue);
        if (numMember > 2 && totalvalue > k) {
          Gang[intToString[head]] = numMember;
        }
      }
    }
  };
  auto change = [&](std::string str) {
    if (stringToInt.find(str) != stringToInt.end()) {
      return stringToInt[str];
    } else {
      stringToInt[str] = numPersion;
      intToString[numPersion] = str;
      return numPersion++;
    }
  };
  int w;
  std::string str1, str2;
  std::cin >> n >> k;
  for (int i = 0; i < n; i++) {
    std::cin >> str1 >> str2 >> w;
    int id1 = change(str1);
    int id2 = change(str2);
    weight[id1] += w;
    weight[id2] += w;
    G[id1][id2] += w;
    G[id2][id1] += w;
  }
  dfsTrave();
  std::cout << Gang.size() << std::endl;
  std::map<std::string, int>::iterator it;
  for (it = Gang.begin(); it != Gang.end(); it++) {
    std::cout << it->first << " " << it->second << std::endl;
  }
}

}  // namespace Graph

namespace dijkstra {
const int maxn = 1000;
const int inf = 1000000000;
int n, G[maxn][maxn];  // u -> v distance
int d[maxn];           // store distance that to v
bool vis[maxn] = {false};
void dijkstra(int s) {
  std::fill(d, d + maxn, inf);
  d[s] = 0;  // start distance
  for (int i = 0; i < n; i++) {
    int u = -1, min = inf;
    for (int j = 0; j < n; j++) {  // find the smallest arc
      if (vis[j] == false && d[j] < min) {
        u = j;
        min = d[j];
      }
    }
    if (u == -1)
      return;
    vis[u] == true;
    for (int v = 0; v < n; v++) {
      if (vis[v] == false && G[u][v] != inf && d[u] + G[u][v] < d[v]) {
        d[v] = d[u] + G[u][v];
      }
    }
  }
}
void testfunc() {
  const int maxn = 505;
  const int INF = 1000000000;
  int n, st, m, ed, G[maxn][maxn], c[maxn];
  int d[maxn];
  bool isv[maxn] = {false};
  int cost[maxn][maxn];
  int pre[maxn];
  auto dijkstra = [&](int s) {
    std::fill(d, d + maxn, INF);
    std::fill(c, c + maxn, INF);
    d[s] = 0;
    c[s] = 0;
    for (int i = 0; i < n; i++) {
      int u = -1, min = inf;
      for (int j = 0; j < n; j++) {
        if (vis[j] == false && d[j] < min) {
          u = j;
          min = d[j];
        }
      }
      if (u == -1)
        return;
      vis[u] = true;
      for (int v = 0; v < n; v++) {
        if (vis[v] == false && G[u][v] != INF) {
          if (d[v] > d[u] + G[u][v]) {
            d[v] = d[u] + G[u][v];
            c[v] = c[u] + cost[u][v];
            pre[v] = u;
          } else if (d[v] == d[u] + G[u][v]) {
            if (c[u] + cost[u][v] < c[v]) {
              c[v] = c[u] + cost[u][v];
              pre[v] = u;
            }
          }
        }
      }
    }
  };
  std::function<void(int)> dfs = [&](int v) {
    if (v == st) {
      printf("%d ", v);
      return;
    }
    dfs(pre[v]);
    printf("%d ", v);
  };
  std::cin >> n >> m >> st >> ed;
  int u, v;
  std::fill(G[0], G[0] + maxn * maxn, INF);
  for (int i = 0; i < m; i++) {
    std::cin >> u >> v;
    std::cin >> G[u][v] >> cost[u][v];
    G[v][u] = G[u][v];
    cost[v][u] = cost[u][v];
  }
  dijkstra(st);
  dfs(ed);
  printf("%d %d\n", d[ed], c[ed]);
}

}  // namespace dijkstra

namespace bellmanford {
const int maxv = 510;
const int inf = 0x3fffffff;
struct Node {
  int v, dis;
  Node(int _v, int _dis) : v(_v), dis(_dis) {}
};
std::vector<Node> Adj[maxv];
int n, m, st, ed, weight[maxv];
int d[maxv], w[maxv], num[maxv];
std::set<int> pre[maxv];
void bellman(int s) {
  std::fill(d, d + maxv, inf);
  memset(num, 0, sizeof(num));
  memset(w, 0, sizeof(w));
  d[s] = 0;
  w[s] = weight[s];
  num[s] = 1;
  for (int i = 0; i < n - 1; i++) {
    for (int u = 0; u < n; u++) {
      for (int j = 0; j < Adj[u].size(); j++) {
        int v = Adj[u][j].v;
        int dis = Adj[u][v].dis;
        if (d[u] + dis < d[v]) {
          d[v] = d[u] + dis;
          w[v] = w[u] + weight[v];
          num[v] = num[u];
          pre[v].clear();
          pre[v].insert(u);
        } else if (d[u] + dis == d[v]) {
          if (w[u] + weight[v] > w[v]) {
            w[v] = w[u] + weight[v];
          }
          pre[v].insert(u);
          num[v] = 0;
          std::set<int>::iterator it;
          for (it = pre[v].begin(); it != pre[v].end(); ++it) {
            num[v] += num[*it];
          }
        }
      }
    }
  }
}

void func() {
  std::cin >> n >> m >> st >> ed;
  for (int i = 0; i < n; i++) {
    std::cin >> weight[i];
  }
  int u, v, wt;
  for (int i = 0; i < m; ++i) {
    std::cin >> u >> v >> wt;
    Adj[u].push_back(Node(v, wt));
    Adj[v].push_back(Node(u, wt));
  }
  bellman(st);
  std::cout << num[ed] << w[ed];
}
}  // namespace bellmanford

namespace SPFA() {
struct Node {
  int v, dis;
  Node(int _v, int _dis) : v(_v), dis(_dis) {}
};
int maxv = 510;
std::vector<Node> Adj[maxv];
int n, d[maxv], num[maxv];
bool inq[maxv];
int inf = 0x3fffffff;
bool SPFA(int s) {
  memset(inq, false, sizeof(inq));
  memset(num, 0, sizeof(num));
  fill(d, d + maxv, inf);
  std::queue<int> q;
  q.push(s);
  inq[s] = true;
  num[s]++;
  d[s] = 0;
  while (!q.empty()) {
    int u = q.front();
    q.pop();
    inq[u] = false;
    for (int j = 0; j < Adj[u].size(); j++) {
      int v = Adj[u][j].v;
      int dis = Adj[u][j].dis;
      if (d[u] + dis < d[v]) {
        d[v] = d[u] + dis;
        if (!inq[v]) {
          q.push(v);
          inq[v] = true;
          num[v]++;
          if (num[v] >= n) {
            return false;
          }
        }
      }
    }
  }
  return true;
}

}  // namespace SPFA()
int main() {
  using namespace dijkstra;
  testfunc();
}