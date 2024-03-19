#include <iostream>
#include <map>
std::map<std::string, int> month_map = {
    {"Jan", 1}, {"Feb", 2}, {"Mar", 3}, {"Apr", 4},  {"May", 5},  {"Jun", 6},
    {"Jul", 7}, {"Aug", 8}, {"Sep", 9}, {"Oct", 10}, {"Nov", 11}, {"Dec", 12}};
int main() {
  std::string str;
  std::cin >> str;
  int len = str.length();

  std::cout << month_map[str.substr(0, len - 2)] << ' '
            << ((str[len - 2] - '0') * 10 + (str[len - 1] - '0'));
}