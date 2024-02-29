#ifndef _LANQIAOPROBLEMS_H_
#define _LANQIAOPROBLEMS_H_
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <tuple>
#include <vector>
namespace WorkingTime {
class Solution {
 public:
  long long workingTime() {
    std::ifstream file("./file.txt", std::ios::in);
    if (!file.is_open()) {
      std::cerr << "can not open file" << std::endl;
      return -1;
    }
    std::stringstream ss;
    std::string line;
    std::tm time = {};
    using Day_t = int;
    std::vector<std::multimap<Day_t, std::pair<int, long long>>> months_vec(12);
    // std::map<Day_t, long long> days_map;
    for (int i = 0; i < 12; i++) {
      months_vec.push_back(std::multimap<Day_t, std::pair<int, long long>>());
    }
    while (std::getline(file, line)) {
      ss << line;
      ss >> std::get_time(&time, "%Y-%m-%d %H:%M:%S");
      if (ss.fail()) {
        std::cerr << "prase fail" << std::endl;
        continue;
      }
      ss.clear();
      auto new_sum_seconds =
          time.tm_hour * 3600 + time.tm_min * 60 + time.tm_sec;
      auto it = months_vec[time.tm_mon].insert(std::make_pair(
          time.tm_mday, std::make_pair(time.tm_hour, new_sum_seconds)));
    }

    long long res = 0;

    for (auto& month : months_vec) {
      int flag = 0;
      long long day_sum_second = 0;
      long long last_day_second = 0;
      int the_day = 0;
      for (auto day : month) {
        if (the_day == 0) {
          the_day = day.first;
        }
        if (flag < 2 && the_day == day.first) {
          day_sum_second -= day.second.second;
          day_sum_second = std::abs(day_sum_second);
          flag++;
          continue;
        } else if (flag == 2 && the_day == day.first) {
          res += day_sum_second;
          day_sum_second = 0;
          last_day_second = day.second.second;
          flag = 0;
          continue;
        } else if (flag == 2 && the_day != day.first) {
          flag = 0;
          day_sum_second -= day.second.second;
          day_sum_second = std::abs(day_sum_second);
          the_day = day.first;
          flag++;
        } else if (last_day_second != 0 && the_day != day.first) {
          day_sum_second = std::abs(day.second.second - last_day_second);
          res += day_sum_second;
          day_sum_second = 0;
          last_day_second = 0;
          the_day = day.first;
          flag++;
        }
      }
    }
    file.close();
    return res;
  }
};
void TestWorkingTime() {
  Solution S;
  std::cout << S.workingTime();
}
}  // namespace WorkingTime

#endif