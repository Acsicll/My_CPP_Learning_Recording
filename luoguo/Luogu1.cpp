#include <iostream>
#include <string>

void filp(std::string& str, std::size_t pos) {
  if (pos + 1 < str.length() && pos >= 0) {
    str[pos] = str[pos] == '*' ? 'o' : '*';
    str[pos + 1] = str[pos + 1] == '*' ? 'o' : '*';
  }
}

int main() {
  std::string strA;
  std::string strB;

  std::cin >> strA >> strB;
  int count = 0;
  for (std::size_t i = 0; i < strA.length(); i++) {
    if (strA[i] != strB[i]) {
      filp(strA, i);
      count++;
    }
  }
  if (strA == strB) {
    std::cout << count;
  }
}