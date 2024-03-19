#include <algorithm>
#include <cstring>
#include <iostream>
namespace number_tower {
const int maxn = 1000;
int f[maxn][maxn] = {0}, dp[maxn][maxn] = {0};
void func() {
  int n;
  scanf("%d", &n);
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= i; j++) {
      std::cin >> f[i][j];
    }
  }
  for (int j = 1; j <= n; j++) {
    dp[n][j] = f[n][j];
  }
  for (int i = n - 1; i >= 1; i--) {
    for (int j = 1; j <= i; j++) {
      dp[i][j] = std::max(dp[i + 1][j], dp[i + 1][j + 1]) + f[i][j];
    }
  }
  printf("%d\n", dp[1][1]);
}

}  // namespace number_tower

namespace largestSubSequenceSum {
const int maxn = 10010;
int A[maxn], dp[maxn];
void func2() {
  int n;
  std::cin >> n;
  for (int i = 0; i < n; i++) {
    std::cin >> A[i];
  }
  dp[0] = A[0];
  for (int i = 1; i < n; i++) {
    dp[i] = std::max(A[i], dp[i - 1] + A[i]);
  }
  int k = 0;
  for (int i = 1; i < n; i++) {
    if (dp[i] > dp[k]) {
      k = i;
    }
  }
  std::cout << dp[k];
}
}  // namespace largestSubSequenceSum

namespace longestIncreasingSequence {
const int maxn = 100;
int A[maxn], dp[maxn];
void func3() {
  int n;
  std::cin >> n;
  for (int i = 1; i <= n; i++) {
    std::cin >> A[i];
  }
  int ans = -1;
  for (int i = 1; i <= n; i++) {
    dp[i] = 1;
    for (int j = 1; j < i; j++) {
      if (A[i] >= A[j] && dp[j] + 1 > dp[i]) {
        dp[i] = dp[j] + 1;
      }
    }
    ans = std::max(ans, dp[i]);
  }
  std::cout << ans;
}
}  // namespace longestIncreasingSequence

namespace longestCommonSubSeqence {
const int maxn = 100;
char A[maxn], B[maxn];
int dp[maxn][maxn];
void func4() {
  int n;
  gets(A + 1);
  gets(B + 1);
  int lenA = strlen(A + 1);
  int lenB = strlen(B + 1);
  for (int i = 0; i <= lenA; i++) {
    dp[i][0] = 0;
  }
  for (int j = 0; j < lenB; j++) {
    dp[0][j] = 0;
  }
  for (int i = 1; i <= lenA; i++) {
    for (int j = 1; j <= lenB; j++) {
      if (A[i] == B[j]) {
        dp[i][j] = dp[i - 1][j - 1] + 1;
      } else {
        dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
      }
    }
  }
  std::cout << dp[lenA][lenB];
}
}  // namespace longestCommonSubSeqence

namespace longestReseverSubSeqence {
const int maxn = 1010;
char S[maxn];
int dp[maxn][maxn];
void func5() {
  gets(S);
  int len = strlen(S);
  int ans = -1;
  for (int i = 0; i < len; i++) {
    dp[i][i] = 1;
    if (i < len - 1) {
      if (S[i] == S[i + 1]) {
        dp[i][i + 1] = 1;
        ans = 2;
      }
    }
  }
  for (int L = 3; L <= len; L++) {
    for (int i = 0; i + L - 1 < len; i++) {
      int j = i + L - 1;
      if (S[i] == S[j] && dp[i + 1][j - 1] == 1) {
        dp[i][j] = 1;
        ans = L;
      }
    }
  }
  std::cout << ans;
}
}  // namespace longestReseverSubSeqence

int main() {
  using namespace number_tower;
  using namespace largestSubSequenceSum;
  using namespace longestIncreasingSequence;
  using namespace longestCommonSubSeqence;
  using namespace longestReseverSubSeqence;
  func5();
}