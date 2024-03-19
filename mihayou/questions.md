# mihayou:

### question1:棋盘

[塔子哥](https://www.zhihu.com/search?q=%E5%A1%94%E5%AD%90%E5%93%A5&search_source=Entity&hybrid_search_source=Entity&hybrid_search_extra=%7B%22sourceType%22%3A%22answer%22%2C%22sourceId%22%3A3254036613%7D)有一个 n * m 的棋盘，一次移动可以选择上下左右四个方向移动一次，不同于普通棋盘，这个棋盘是循环的。

即 (x,m) 和 (x,1) 两个点可以一步到达，其中1≤x≤n。同样的， (n,y)和 (1,y) 两个点也可以一步到达，其中 1≤y≤m。

现在塔子哥需要从 A先走到 B 点，再从 B 点走到 C点，问最小移动次数是多少。

```c++
#include <bits/stdc++.h>
typedef pair<int,int>PII;

int main(
	int n,m;
	std::cin>>n>>m;
	PII	a,b,c;
	std::cin>>a.first>>a.second;
	std::cin>>b.first>>b.second;
	std::cin>>c.first>>c.second;

	long long ans = 0;
	ans += std::min(std::abs(a.first - b.first),n - std::abs(a.first - b.first));
	ans += std::min(std::abs(a.second - b.second),m - std::abs(a.second - b.second));
	ans += std::min(std::abs(b.first - c.first),n - std::abs(b.first - c.first));
	ans += std::min(std::abs(b.second - c.second),m - std::abs(b.second,c.second));
	std::cout<<ans<<" ";
	return 0;
)
```

---

### question2:有根树的节点个数
