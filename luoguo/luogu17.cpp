#include <iostream>
#include <map>
const int maxn = 200010;
std::map<long long, int> hashtable;
int n, c;
int main() {
  long long count = 0;
  std::cin >> n >> c;
  long long temp;
  for (long long i = 0; i < n; i++) {
    std::cin >> temp;
    auto it = hashtable.find(temp - c);
    if (it != hashtable.end()) {
      count += hashtable[temp - c];
    }
    it = hashtable.find(temp + c);
    if (it != hashtable.end()) {
      count += hashtable[temp + c];
    }
    if (it == hashtable.find(temp)) {
      hashtable.insert(std::make_pair(temp, 1));
    } else {
      hashtable[temp]++;
    }
  }
  std::cout << count;
}