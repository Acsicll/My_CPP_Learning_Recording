#include <iostream>

int main() {
  std::string str;
  std::cin >> str;
  if (!str.length()) {
    return 0;
  }
  std::string ans;
  for (int i = 0; i < str.length(); i++) {
    if (isdigit(str[i])) {
      auto num = str[i] - '0';
      while (--num) {
        ans += str[i - 1];
      }
    } else {
      ans += str[i];
    }
  }
  std::cout << ans;
}