#include <algorithm>
#include <array>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <list>
#include <sstream>
#include <stack>
#include <vector>
#include "meta_template_learning.h"

namespace question1 {
int arr[100001] = {0};
void question1() {
  int T;
  int index, max_num = 0;
  while (!(std::cin >> T))
    ;
  int temp = 0;
  while (temp < T) {
    int num, score;
    if (std::cin >> num >> score && num >= 0 && num <= 100000) {
      arr[num] += score;
    }
    if (max_num < arr[num]) {
      index = num;
      max_num = arr[num];
    }
    temp++;
  }
  std::cout << index << " " << max_num;
}
}  // namespace question1

namespace quesion2 {
void question2() {
  const int num = 200;
  int arr[num] = {0};
  int n;
  while (std::cin >> n && !(n >= 1 && n <= 200))
    ;
  int temp = 0;
  while (temp < n) {
    std::cin >> arr[temp];
    temp++;
  }
  int match = 0;
  while (!(std::cin >> match))
    ;
  for (unsigned i = 0; i < n; i++) {
    if (arr[i] == match) {
      std::cout << i << std::endl;
      return;
    }
  }
}
}  // namespace quesion2

namespace question3 {
void question3() {
  int side = 0;
  char ch;
  while (!(std::cin >> side >> ch))
    ;
  int col = side;
  int row = col % 2 == 1 ? col / 2 + 1 : col / 2;
  // row
  for (int i = 0; i < side; i++) {
    // col
    std::cout << ch;
  }
  std::cout << std::endl;
  for (int i = 2; i < row; i++) {
    std::cout << ch;
    for (int j = 0; j < col - 2; j++) {
      std::cout << ' ';
    }
    std::cout << ch << std::endl;
  }
  for (int i = 0; i < side; i++) {
    // col
    std::cout << ch;
  }
}
}  // namespace question3

namespace question4 {

void question4() {
  std::vector<std::array<int, 12>> year_vec;
  year_vec.push_back({{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}});
  year_vec.push_back({{31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}});

  std::string line;
  long long seconds;
  std::stringstream ss;
  std::tm time{};
  std::tm time_arr[2] = {};
  int index = 0;
  while (std::cin >> line) {
    ss.clear();
    ss.str(line);
    ss >> std::get_time(&time, "%Y%m%d");
    if (ss.fail()) {
      return;
    }
    time.tm_year += 1900;
    time.tm_mon += 1;
    time_arr[index] = time;
    index++;
  }

  int res = 0;
  auto is_leap_year = [](int year) -> bool {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
  };
  auto days_in_month = [&is_leap_year, &year_vec](int year, int month) -> int {
    if (is_leap_year(year)) {
      return (year_vec[1])[month - 1];
    } else {
      return (year_vec[0])[month - 1];
    }
  };
  auto daysbetweenDates = [&days_in_month, is_leap_year](int year1, int month1,
                                                         int day1, int year2,
                                                         int month2, int day2) {
    int days;
    for (int month = month1; month <= 12; ++month) {
      days += days_in_month(year1, month);
    }
    days -= day1;
    for (int year = year1 + 1; year1 < year2; ++year) {
      days += is_leap_year(year) ? 366 : 365;
    }
    for (int month = 1; month <= month2; ++month) {
      days += days_in_month(year2, month);
    }
    return days;
  };
  if (time_arr[0].tm_year > time_arr[1].tm_year) {
    std::swap(time_arr[0], time_arr[1]);
  }
  res = daysbetweenDates(time_arr[0].tm_year, time_arr[0].tm_mon,
                         time_arr[0].tm_mday, time_arr[1].tm_year,
                         time_arr[1].tm_mon, time_arr[1].tm_mday);
  std::cout << std::abs(res);
}
}  // namespace question4

namespace question5 {
void question5() {
  // A, B <=2^30 - 1,1<D<=10
  long long A, B, D;
  while (!(std::cin >> A >> B >> D))
    ;
  auto dex_res = A + B;
  std::vector<int> store_arr;
  long long index = 0;
  do {
    store_arr.push_back(dex_res % D);
    dex_res = dex_res / D;
  } while (dex_res != 0);
  std::vector<int> resever_store_arr(store_arr.rbegin(), store_arr.rend());
  for (auto val : resever_store_arr) {
    std::cout << val;
  }
}
}  // namespace question5

namespace question6 {
void question6() {
  std::string str;
  while (!(std::cin >> str))
    ;
  std::string resever_str(str.rbegin(), str.rend());
  std::cout << (str == resever_str ? "YES" : "NO");
}
}  // namespace question6

namespace question7 {
void question7() {
  std::string line;
  std::getline(std::cin, line);
  std::stack<std::string> words;
  std::stringstream ss(line);
  std::string word;
  while (ss >> word) {
    words.push(word);
  }
  while (!words.empty()) {
    std::cout << words.top();
    words.pop();
    if (!words.empty()) {
      std::cout << " ";
    }
  }
}
}  // namespace question7

namespace IncrementSort {
void SelectSort(int arr[], const std::size_t size) {
  // arr[] equal in *arr;
  for (std::size_t i = 0; i < size - 1; ++i) {
    auto selected_index = i;
    for (std::size_t j = i + 1; j < size; ++j) {
      if (arr[selected_index] > arr[j]) {
        selected_index = j;
      }
    }
    std::swap(arr[i], arr[selected_index]);
  }
}

int* SelectSort_(int* arr, const std::size_t size) {
  for (std::size_t i = 0; i < size - 1; ++i) {
    auto selected_index = i;
    for (std::size_t j = i + 1; j < size; ++j) {
      if (arr[selected_index] > arr[j]) {
        selected_index = j;
      }
    }
    std::swap(arr[i], arr[selected_index]);
  }
  return arr;
}

}  // namespace IncrementSort

int main() {
  // quesion1::question1();
  // quesion2::question2();
  // quesion3::question3();
  // question4::question4();
  // question6::question6();
  // question7::question7();
  using namespace IncrementSort;
  using namespace SomeTemplateInstance;
  int arr[] = {41, -16, 71, 123, 0,   -1, 5,  -10, 11,   256, -25,
               10, 51,  61, -1,  61,  55, 71, 87,  -124, -16, -71,
               1,  611, 3,  51,  116, 80, 2,  74,  67};
  std::list<int> ilist1(std::begin(arr), std::end(arr));
  printValue(ilist1);
  // std::for_each(ilist1.begin(), ilist1.end(),
  //               [](const int& val) { std::cout << val << " "; });
  std::cout << "\n";
  // auto ptr = SelectSort(arr, sizeof(arr) / sizeof(arr[0]));
  // auto tmp = ptr;
  // while (tmp != ptr + sizeof(arr) / sizeof(arr[0])) {
  //   std::cout << *tmp << " ";
  //   tmp++;
  // }
  SelectSort(arr, sizeof(arr) / sizeof(arr[0]));
  std::list<int> ilist2(std::begin(arr), std::end(arr));
  printValue(ilist2);
  // std::for_each(ilist2.begin(), ilist2.end(),
  //               [](const int& val) { std::cout << val << " "; });
  return 0;
}