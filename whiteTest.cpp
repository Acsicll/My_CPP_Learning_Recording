#include <algorithm>
#include <array>
#include <cassert>
#include <cctype>
#include <cmath>
#include <cstring>
#include <ctime>
#include <functionaL>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <set>
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

namespace question8 {
// input N:groups, T:person_number,
// test_registration score

// output test_registration, rank in whole(if equal:rank + 2 and same rank),rank
// in group
struct student {
  student() {}
  ~student() { test_number.clear(); }
  std::string test_number;
  int score;
  int test_room;
  int rank_in_group;
  int rank_in_whole;
};
void question8() {
  int N, T;
  std::cout << "input groups: ";
  std::cin >> N;
  std::map<std::string, student> arr[N];
  struct student the_student;
  auto temp = 1;
  while (temp <= N) {
    std::cout << "input each group info: ";
    std::cin >> T;
    auto T_times = 1;
    while (T_times <= T) {
      char buff[14];
      std::cin >> buff;
      the_student.test_number = std::string(buff, 14);
      std::cin >> the_student.score;
      the_student.test_room = temp;
      // the_student.rank_in_group = T_times;
      arr[temp - 1].insert(
          std::make_pair(the_student.test_number, the_student));
      T_times++;
    }
    temp++;
  }
  std::vector<student> whole_student_vector;
  for (auto& students : arr) {
    std::vector<student> group_student_vector;
    for (auto& val : students) {
      group_student_vector.push_back(val.second);
    }
    // std::sort(group_student_vector.begin(), group_student_vector.end(),
    //           [](const student& s1, const student& s2) {
    //             return s1.score > s2.score;
    //           });
    for (unsigned i = group_student_vector.size() - 1; i > 0; --i) {
      for (unsigned j = 0; j < i; ++j) {
        if (group_student_vector[j].score < group_student_vector[j + 1].score) {
          std::swap(group_student_vector[j], group_student_vector[j + 1]);
        }
      }
    }
    int rank = 1;
    bool flag = false;
    group_student_vector[0].rank_in_group = rank;
    for (unsigned i = 1; i < group_student_vector.size(); i++) {
      if (group_student_vector[i].score == group_student_vector[i - 1].score) {
        group_student_vector[i].rank_in_group = rank;
        flag = true;
        // rank += 2;
      } else {
        if (flag) {
          rank += 2;
        } else {
          rank++;
        }
        group_student_vector[i].rank_in_group = rank;
        // rank++;
        flag = false;
      }
    }
    std::copy(group_student_vector.begin(), group_student_vector.end(),
              std::back_inserter(whole_student_vector));
  }

  std::sort(
      whole_student_vector.begin(), whole_student_vector.end(),
      [](const student& s1, const student& s2) { return s1.score > s2.score; });
  int rank = 1;
  whole_student_vector[0].rank_in_whole = rank;
  for (unsigned i = 1; i < whole_student_vector.size(); i++) {
    if (whole_student_vector[i].score == whole_student_vector[i - 1].score) {
      whole_student_vector[i].rank_in_whole =
          whole_student_vector[i - 1].rank_in_whole;
      rank = whole_student_vector[i - 1].rank_in_whole;
      rank += 2;
    } else {
      whole_student_vector[i].rank_in_whole = rank;
      rank++;
    }
  }

  for (auto every_student : whole_student_vector) {
    std::cout << every_student.test_number << " " << every_student.rank_in_whole
              << " " << every_student.test_room << " "
              << every_student.rank_in_group << "\n";
  }
}

}  // namespace question8

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

/// @brief selecting a value,then it is used to compare with the value that
/// before itif one of the value before it less than the selected one, let the
/// sequence before it move back a sister. trying to do that constantly until
/// find a value smaller than it,then put it after that.
/// @param arr
/// @param size
void InsertSort(int arr[], const std::size_t size) {
  for (std::size_t i = 1; i < size; ++i) {
    auto temp = arr[i];
    std::size_t j = i - 1;
    while (j >= 0 && temp < arr[j]) {
      arr[j + 1] = arr[j];
      --j;
    }
    arr[j + 1] = temp;
  }
}

/// @brief confirming the times of the cycle firstly and every times of cycle
/// stand for the length of son of whole sequence. it means that we have to
/// compare the son seqence length times every cycle. from the index 0 to the
/// index length subtract 1 it need to compare current index value with next
/// index value,if the current larger than the next, swap it.
/// @param arr
/// @param size
void SwapSort(int arr[], const std::size_t size) {
  for (std::size_t i = size - 1; i > 0; --i) {
    for (std::size_t j = 0; j < i; ++j) {
      if (arr[j] > arr[j + 1]) {
        std::swap(arr[j], arr[j + 1]);
      }
    }
  }
}

}  // namespace IncrementSort

namespace Hash_ {
// string to hash key(whole conversion):
long long HashFuncByWholeConversion(const char s[], const std::size_t len) {
  assert(len >= 0);
  assert(s);
  long long key = 0;
  for (std::size_t i = 0; i < len; ++i) {
    if (std::isalpha(s[i])) {
      if (std::isupper(s[i])) {
        key = key * 26 + (s[i] - 'A');
      } else {
        key = key * 52 + (s[i] - 'a');
      }
    } else if (std::isdigit(s[i])) {
      key = key * 62 + (s[i] - '0');
    }
  }
  return key;
}

long long HashFuncBySplit(const char s[], const std::size_t len) {
  assert(len >= 0);
  assert(s);
  long long key = 0;
  int num_start_index = len - 1;
  for (std::size_t i = 0; i < len; ++i) {
    if (std::isalpha(s[i])) {
      if (std::isupper(s[i])) {
        key = key * 26 + (s[i] - 'A');
      } else {
        key = key * 52 + (s[i] - 'a');
      }
    } else if (std::isdigit(s[i])) {
      num_start_index = i;
      break;
    }
  }
  for (; num_start_index < len; ++num_start_index) {
    key = key * 10 + (s[num_start_index] - '0');
  }
  return key;
}

// giving N strings that combine with three-bit capital.then giving M searching
// strings to count the occurred times
void test() {
  char str[100][5], temp[5];
  long long hashTable[26 * 26 * 26 + 10] = {0};
  auto hashFunc = [](const char s[], const std::size_t len) -> long long {
    int key = 0;
    for (std::size_t i = 0; i < len; ++i) {
      key = key * 26 + (s[i] - 'A');
    }
    return key;
  };
  auto main = [&]() {
    int n, m;
    std::cin >> n >> m;
    for (int i = 0; i < n; ++i) {
      std::cin >> str[i];
      auto key = hashFunc(str[i], 3);
      hashTable[key]++;
    }
    for (int i = 0; i < m; ++i) {
      std::cin >> temp;
      auto key = hashFunc(temp, 3);
      std::cout << hashTable[key] << std::endl;
    }
  };
  main();
}

namespace FullPermutation {
const int maxn = 11;  // actually size is 10;
int n = 3, P[maxn], hashTable[maxn] = {false};
// actually,we divide into several groups,each group needs to confirm the
// first lettle and select the other lettles. if the next lettle belongs to
// the string's proper subset and it can be select or end this time recursion
void generateP(int index) {
  if (index == n + 1) {  // process done
    for (int i = 1; i <= n; i++) {
      std::cout << P[i];
    }
    puts("\n");
    return;
  }
  for (int x = 1; x <= n; x++) {
    if (hashTable[x] == false) {
      P[index] = x;
      hashTable[x] = true;
      // one recursion is done,try to do next time
      generateP(index + 1);
      hashTable[x] = false;
    }
  }
};

}  // namespace FullPermutation

namespace EightQueen {
const int n = 9;
int p[n + 1] = {0};
bool hashtable[n + 1] = {false};
static int count = 0;
void generateP(int index) {
  if (index == n + 1) {
    bool flag = true;
    for (int i = 1; i <= n; i++) {
      for (int j = i + 1; j <= n; j++) {
        if (std::abs(i - j) == std::abs(p[i] - p[j])) {
          flag = false;
          break;
        }
      }
    }
    if (flag) {
      count++;
    }
    return;
  }
  // create 1~n all sequence
  for (int i = 1; i <= n; i++) {
    if (hashtable[i] == false) {
      p[index] = i;
      hashtable[i] = true;
      generateP(index + 1);
      hashtable[i] = false;
    }
  }
}

void generateP2(int index) {
  if (index == n + 1) {
    count++;
    return;
  }
  for (int i = 1; i <= n; i++) {
    // checks whether the queen can be placed this row or not
    if (hashtable[i] == false) {
      bool flag = true;
      for (int pre = 1; pre < index; pre++) {
        // checks whether the difference between the current position and
        // previous row and the current row is equal to the difference between
        // the previous column and the current column. if equal,the queen can
        // not be placed here
        if (std::abs(index - pre) == std::abs(i - p[pre])) {
          flag = false;
          break;
        }
      }
      if (flag) {
        p[index] = i;  // recording last queen position
        hashtable[i] = true;
        generateP2(index + 1);
        hashtable[i] = false;
      }
    }
  }
}
}  // namespace EightQueen

}  // namespace Hash_

namespace Greedy {
void Greedy_sold_moon_cake() {
  int N, D;
  std::cin >> N >> D;
  struct moon_cake_t {
    unsigned int store_num;
    unsigned int sold_num;
    double average;
  };

  std::vector<moon_cake_t> moon_cake_vec;
  for (int i = 0; i < N; ++i) {
    moon_cake_t mc;
    moon_cake_vec.push_back(mc);
  }
  for (int i = 0; i < N; ++i) {
    std::cin >> moon_cake_vec[i].store_num;
  }
  for (int i = 0; i < N; ++i) {
    std::cin >> moon_cake_vec[i].sold_num;
  }
  for (auto& val : moon_cake_vec) {
    val.average = val.sold_num * 1.00 / val.store_num * 1.00;
  }
  std::sort(moon_cake_vec.begin(), moon_cake_vec.end(),
            [](const moon_cake_t& mc1, const moon_cake_t& mc2) {
              return mc1.average > mc2.average;
            });
  int index = 0;
  int remain = 0;
  double money = 0.00;
  for (; D != 0;) {
    if (D == moon_cake_vec[index].store_num) {
      money = moon_cake_vec[index].sold_num;
      break;
    } else if (D < moon_cake_vec[index].store_num) {
      money += D * moon_cake_vec[index].average;
      break;
    } else if (D > moon_cake_vec[index].store_num) {
      money += moon_cake_vec[index].sold_num;
      remain = D - moon_cake_vec[index].store_num;
      D = remain;
    }
    index++;
  }
  std::cout << std::fixed << std::setprecision(2) << money;
}

void Geedy_smallest_number() {
  const int N = 10;
  int arr[N] = {0};
  int whole_numbers = 0;
  for (int i = 0; i < N; i++) {
    std::cin >> arr[i];
    whole_numbers += arr[i];
  }
  int res;
  for (int i = 1; i < N; i++) {
    if (arr[i] != 0) {
      res = i;
      arr[i] -= 1;
      break;
    }
  }
  for (int i = 0; i < N; i++) {
    while (arr[i] != 0) {
      res *= 10;
      res += i;
      arr[i] -= 1;
    }
  }
  std::cout << "\n" << res;
}
struct point {
  int _x;
  int _y;
  point() {}
  point(int x, int y) : _x(x), _y(y) {}
  friend std::ostream& operator<<(std::ostream& os, point& p) {
    os << "(" << p._x << ", " << p._y << ")";
    return os;
  }
};
void interval_greedy() {
  int N;
  std::cin >> N;
  std::vector<point> arr;
  for (unsigned i = 0; i < N; i++) {
    point p;
    std::cin >> p._x >> p._y;
    arr.push_back(p);
  }
  std::sort(arr.begin(), arr.end(), [](const point& lhs, const point& rhs) {
    if (lhs._x != rhs._x) {
      return lhs._x > rhs._x;
    } else {
      return lhs._y < rhs._y;
    }
  });
  auto last_point = arr[0];
  int res = 1;
  for (int i = 1; i < N; i++) {
    if (arr[i]._y <= last_point._x) {
      last_point = arr[i];
      res++;
    }
  }
  std::cout << res;
}
}  // namespace Greedy

namespace BinarySearch {
int BinarySearch(int a[], int left, int right, int x) {
  int mid;
  while (left <= right) {
    mid = (left + right) / 2;
    if (a[mid] == x) {
      return mid;
    } else if (a[mid] > x) {
      right = mid - 1;
    } else if (a[mid] < x) {
      left = mid + 1;
    }
  }
  return -1;
}
int LowerBound(int a[], int left, int right, int x) {
  int mid;
  while (left < right) {
    mid = (left + right) / 2;
    if (a[mid] >= x) {  // condition
      right = mid;
    } else {
      left = mid + 1;
    }
  }
  return left;
}
int upperBound(int a[], int left, int right, int x) {
  int mid;
  while (left < right) {
    mid = (left + right) / 2;
    if (a[mid] > x) {
      right = mid;
    } else {
      left = mid + 1;
    }
  }
  return left;
}
const double eps = 1e-5;
double math_func(double x) {
  return x;
}
double solve(double left, double right, double num) {
  double lhs = left, rhs = right;
  double mid;
  while (rhs - lhs > eps) {
    mid = (rhs + lhs) / 2;
    if (mid > num) {
      right = mid;
    } else {
      left = mid;
    }
  }
  return mid;
}
void cut_stick() {
  auto FindK = [](int arr[], int left, int right, int K) -> int {
    int result;
    int mid;
    while (left <= right) {
      mid = (left + right) / 2;
      int count = 0;
      for (int i = 0; i < right - left; i++) {
        count += arr[i] / mid;
      }
      if (count >= K) {
        result = mid;
        left = mid + 1;
      } else {
        right = mid - 1;
      }
    }
    return result;
  };
  int N, K;
  std::cin >> N >> K;
  int stick[N];
  int max_length = 0;
  for (unsigned i = 0; i < N; i++) {
    std::cin >> stick[i];
    max_length = std::max(max_length, stick[i]);
  }
  bool arr[max_length] = {false};
  for (unsigned i = 1; i <= max_length + 1; ++i) {
    int temp = 0;
    for (unsigned j = 0; j < N; j++) {
      temp += (stick[j] / i);
    }
    if (temp >= 7) {
      arr[i] = true;
    }
  }
  int res = 0;
  for (int i = 1; i <= max_length + 1; i++) {
    if (arr[i] == true) {
      res = std::max(res, i);
    }
  }
  std::cout << res;
}

void max_radius() {
  auto checkFeasible = [](const std::vector<int>& segments,
                          double radius) -> bool {
    int n = segments.size();
    double sum = 0;
    for (int i = 0; i < n; i++) {
      double angle = asin(segments[i] / (2.0 * radius));
      sum += angle;
    }
    return sum > 2 * M_PI;
  };
  auto binarySearchMaxRadius =
      [&checkFeasible](const std::vector<int>& segments_) -> double {
    double left = 0;
    double right = 10000;
    double maxRadius = 0;
    // emumerate radius,if the radius
    while (right - left > 1e-6) {
      double mid = left + (right - left) / 2;
      if (checkFeasible(segments_, mid)) {
        maxRadius = mid;
        left = mid;
      } else {
        right = mid;
      }
    }
    return maxRadius;
  };
  int N;
  std::cin >> N;
  std::vector<int> Segments(N);
  for (int i = 0; i < N; i++) {
    std::cin >> Segments[i];
  }
  std::sort(Segments.begin(), Segments.end());
  double maxRadius = binarySearchMaxRadius(Segments);
  std::cout << maxRadius << std::endl;
}

}  // namespace BinarySearch

namespace BinaryPow {
typedef long long ll;
ll BinaryPow(ll a, ll b, ll m) {
  if (b == 0)
    return 1;
  if (b & 1)
    return a * BinaryPow(a, b - 1, m) % m;
  else {
    ll mul = BinaryPow(a, b / 2, m);
    return mul * mul % m;
  }
}
ll BinaryPow2(ll a, ll b, ll m) {
  ll ans = 1;
  while (b > 0) {
    if (b & 1) {
      ans = ans * a % m;
    }
    a = a * a % m;
    b >>= 1;
  }
  return ans;
}
}  // namespace BinaryPow

namespace twoPointers {
void findTwoNumberBySum(int increase_arr[], int len, int sum) {
  int i = 0, j = len - 1;
  while (i < j) {
    if (increase_arr[i] + increase_arr[j] == sum) {
      std::cout << i << " " << j << std::endl;
    } else if (increase_arr[i] + increase_arr[j] < sum) {
      i++;
    } else {
      j--;
    }
  }
}

int merge(int arra[], int arrb[], int arrc[], int a_len, int b_len) {
  int i = 0, j = 0, index = 0;
  while (i < a_len && j < b_len) {
    if (arra[i] <= arrb[j]) {
      arrc[index++] = arra[i++];
    } else {
      arrc[index++] = arrb[j++];
    }
  }
  while (i < a_len) {
    arrc[index++] = arra[i++];
  }
  while (j < b_len) {
    arrc[index++] = arrb[j++];
  }
  return index;
}

const int maxn = 100;
void merge(int arr[], int l1, int r1, int l2, int r2) {
  int i = l1, j = l2;
  int temp[maxn], index = 0;
  while (i <= r1 && j <= r2) {
    if (arr[i] <= arr[j]) {
      temp[index++] = arr[i++];
    } else {
      temp[index++] = arr[j++];
    }
  }
  while (i <= r1) {
    temp[index++] = arr[i++];
  }
  while (j <= r2) {
    temp[index++] = arr[j++];
  }
  for (i = 0; i < index; i++) {
    arr[l1 + i] = temp[i];
  }
}

void mergeSort(int A[], int left, int right) {
  if (left < right) {
    int mid = left + (right - left) / 2;
    mergeSort(A, left, mid);
    mergeSort(A, mid + 1, right);
    merge(A, left, mid, mid + 1, right);
  }
}

void mergSort(int A[], int n) {
  for (int step = 2; step / 2 <= n; step *= 2) {
    for (int i = 1; i <= n; i += step) {
      int mid = i + step / 2 - 1;
      if (mid + 1 <= n) {
        merge(A, i, mid, mid + 1, std::min(i + step - 1, n));
      }
    }
  }
}
}  // namespace twoPointers

namespace partition {
int Partition(int arr[], int left, int right) {
  int temp = arr[left];
  while (left < right) {
    while (left < right && arr[right] > arr[temp]) {
      right--;
    }
    arr[left] = arr[right];
    while (left < right && arr[left] >= arr[temp]) {
      left++;
    }
    arr[right] = arr[left];
  }
  arr[left] = temp;
  return left;
}

int randPartition(int arr[], int left, int right) {
  int p = (round(1.0 * rand() / RAND_MAX * (right - left) + left));
  std::swap(arr[p], arr[left]);
  int temp = arr[left];
  while (left < right) {
    while (left < right && temp < arr[right]) {
      right--;
    }
    arr[left] = arr[temp];
    while (left < right && temp >= arr[left]) {
      left++;
    }
    arr[left] = arr[right];
  }
  arr[left] = temp;
  return left;
}

void QuickSort(int arr[], int left, int right) {
  if (left < right) {
    int pos = Partition(arr, left, right);
    QuickSort(arr, left, pos);
    QuickSort(arr, pos + 1, right);
  }
}
// findTheKMax
int randSelect(int A[], int left, int right, int k) {
  if (left == right) {
    return A[right];
  }
  int p = randSelect(A, left, right, k);
  int m = p - left + 1;
  if (k == m) {
    return A[p];
  }
  if (k < m) {
    return randSelect(A, left, p - 1, k);
  } else {
    return randSelect(A, p + 1, right, k - m);
  }
}

void TheDifferenceBetweenWithSplitedArray() {
  auto randpartition = [](int arr[], int left, int right) -> int {
    int p = (round(1.0 * rand() / RAND_MAX * (right - left) + left));
    std::swap(arr[p], arr[left]);
    int temp = arr[left];
    while (left < right) {
      while (left < right && temp < arr[right]) {
        right--;
      }
      arr[left] = arr[right];
      while (left < right && temp >= arr[left]) {
        left++;
      }
      arr[right] = arr[left];
    }
    arr[left] = temp;
    return left;
  };
  std::function<void(int arr[], int, int, int)> quickSort =
      [&randpartition, &quickSort](int arr[], int left, int right, int K) {
        if (left == right) {
          return;
        }
        int p = randpartition(arr, left, right);
        int m = p - left + 1;
        if (K == m)
          return;
        if (K < m) {
          quickSort(arr, left, p - 1, K);
        } else {
          quickSort(arr, p + 1, right, K - m);
        }
      };
  srand((unsigned)(time(0)));
  int sum = 0, sum1 = 0;
  int n;
  std::cin >> n;
  int arr[n] = {0};
  for (int i = 0; i < n; i++) {
    std::cin >> arr[i];
    sum += arr[i];
  }
  quickSort(arr, 0, n - 1, n / 2);
  for (int i = 0; i < n / 2; i++) {
    sum1 += arr[i];
  }
  std::cout << (sum - sum1) - sum1;
}

}  // namespace partition

namespace CountingTheWord {
void countWord() {
  const int MAXN = 100010;
  const int MOD = 100000007;
  char str[MAXN];
  int leftNump[MAXN] = {0};
  gets(str);
  int len = strlen(str);
  for (int i = 0; i < len; i++) {
    if (i > 0) {
      leftNump[i] = leftNump[i - 1];
    }
    if (str[i] == 'P') {
      leftNump[i]++;
    }
  }
  int ans = 0, rightNumT = 0;
  for (int i = len - 1; i >= 0; i--) {
    if (str[i] == 'T') {
      rightNumT++;
    } else if (str[i] == 'A') {
      if (str[i + 1] == 'A') {
        ans = (ans + leftNump[i] * rightNumT) % MOD;
      }
    }
  }
  std::cout << ans;
}

}  // namespace CountingTheWord

int conversion(int num, int inc = 1) {
  int arr[4];
  int index = 0;
  while (index < 4) {
    arr[index] = num % 10;
    num /= 10;
    index++;
  }
  if (inc) {
    for (int i = 3; i > 0; i--) {
      for (int j = 0; j < i; j++) {
        if (arr[j] < arr[j + 1])
          std::swap(arr[j], arr[j + 1]);
      }
    }
  } else {
    for (int i = 3; i > 0; i--) {
      for (int j = 0; j < i; j++) {
        if (arr[j] > arr[j + 1])
          std::swap(arr[j], arr[j + 1]);
      }
    }
  }
  num = 0;
  for (int i = 0; i < 4; i++) {
    num *= 10;
    num += arr[i];
  }
  return num;
}

namespace numberblackhold {
void numberblackhold() {
  int num;
  std::cin >> num;
  while (num) {
    std::cout << num << std::endl;
    num = conversion(num) - conversion(num, 0);
    if (num == 6174) {
      break;
    }
  }
  std::cout << num;
}

}  // namespace numberblackhold
namespace math_ {
namespace GCD {
int gcd(int a, int b) {
  return !b ? a : gcd(b, a % b);
}
int lcm(int a, int b) {
  int a_b_gcd = gcd(a, b);
  return a * b / a_b_gcd;
}
}  // namespace GCD

namespace Fraction_ {
struct Fraction {
  int up, down;
};
Fraction reduction(Fraction result) {
  if (result.down < 0) {
    result.up = -result.up;
    result.down = -result.down;
  }
  if (result.up == 0) {
    result.down = 1;
  } else {
    int d = GCD::gcd(abs(result.up), abs(result.down));
    result.up /= d;
    result.down /= d;
  }
  return result;
}
Fraction add(Fraction f1, Fraction f2) {
  Fraction result;
  result.up = f1.up * f2.down + f2.up * f1.down;
  result.down = f1.down * f2.down;
  return reduction(result);
}
Fraction sub(Fraction f1, Fraction f2) {
  Fraction result;
  result.up = f1.up * f2.down - f2.up * f1.down;
  result.down = f1.down * f2.down;
  return reduction(result);
}
Fraction multi(Fraction f1, Fraction f2) {
  Fraction result;
  result.up = f1.up * f2.up;
  result.down = f1.down * f1.down;
  return reduction(result);
}
Fraction divide(Fraction f1, Fraction f2) {
  Fraction result;
  result.up = f1.up * f2.down;
  result.down = f1.down * f2.up;
  return reduction(result);
}
void showResult(Fraction r) {
  if (r.down == 1)
    printf("%lld", r.up);
  else if (abs(r.up) > r.down) {
    printf("%d %d/%d", r.up / r.down, abs(r.up) % r.down, r.down);
  } else {
    printf("%d/%d", r.up, r.down);
  }
}
}  // namespace Fraction_

namespace prime {
bool isPrime(int n) {
  if (n <= 1)
    return false;
  int sqr = (int)sqrt(1.0 * n);
  for (int i = 2; i <= sqr; i++) {
    if (n % i == 0)
      return false;
  }
  return true;
}

bool isPrime2(int n) {
  if (n <= 1)
    return false;
  for (int i = 2; i * i <= n; i++) {
    if (n % i == 0)
      return false;
  }
  return true;
}
void getPrimeTable(int n) {
  const int maxn = n + 1;
  int prime[maxn], pNum = 0;
  bool p[maxn] = {0};
  auto find_prime = [&]() -> void {
    for (int i = 1; i < maxn; i++) {
      if (isPrime(i) == true) {
        prime[pNum++] = i;
        p[i] = true;
      }
    }
  };
  find_prime();
  for (int i = 0; i < pNum; i++) {
    std::cout << prime[i] << " ";
  }
}

void getPrimeTable2(int n) {
  const int maxn = n + 1;
  int prime[maxn], pNum = 0;
  bool p[maxn] = {0};
  for (int i = 2; i < maxn; i++) {
    if (p[i] == false) {
      prime[pNum++] = i;
      for (int j = i + i; j < maxn; j += i) {
        p[j] = true;
      }
    }
  }
  for (int i = 0; i < pNum; i++) {
    std::cout << prime[i] << " ";
  }
}

void FindPrimeInMN(int M, int N) {
  if (M > N)
    return;
  const int maxn = 10001;
  int prime[maxn], pNum = 0;
  bool p[maxn] = {0};
  for (int i = 2; i < maxn; i++) {
    if (p[i] == false) {
      prime[pNum++] = i;
      if (pNum >= N)
        break;
      for (int j = i + i; j < maxn; j += i) {
        p[j] = true;
      }
    }
  }
  for (int i = M; i <= N; i++) {
    std::cout << prime[i - 1] << std::endl;
  }
}

struct factor {
  int x;
  int cnt;
};

void primeFactor() {
  // auto isPrime = [](int n) -> bool {
  //   if (n <= 1)
  //     return false;
  //   int sqr = (int)sqrt(1.0 * n);
  //   for (int i = 0; i < sqr; i++) {
  //     if (n % i == 0){
  //       return false;
  //     }
  //   }
  //   return true;
  // };
  int prime[10010] = {0};
  int pNum = 0;
  int p[10010] = {0};
  auto createPrimeTable = [&](int n = 10010) {
    for (int i = 2; i <= n; i++) {
      if (p[i] == false) {
        prime[pNum++] = i;
        for (int j = i + i; j <= n; j += i) {
          p[j] = true;
        }
      }
    }
  };
  factor fac[10];
  createPrimeTable();
  int n, num = 0;
  std::cin >> n;
  if (n == 1)
    printf("i=1");
  else {
    printf("%d=", n);
    int sqr = (int)sqrt(1.0 * n);
    for (int i = 0; i < pNum && prime[i] <= sqr; i++) {
      if (n % prime[i] == 0) {
        fac[num].x = prime[i];
        fac[num].cnt = 0;
        while (n % prime[i] == 0) {
          fac[num].cnt++;
          n /= prime[i];
        }
        num++;
      }
      if (n == 1)
        break;
    }
    if (n != 1) {
      fac[num].x = n;
      fac[num++].cnt = 1;
    }
    for (int i = 0; i < num; i++) {
      if (i > 0)
        printf("*");
      printf("%d", fac[i].x);
      if (fac[i].cnt > 1) {
        printf("^%d", fac[i].cnt);
      }
    }
  }
}
}  // namespace prime

namespace BigNum {
struct bign {
  int d[1000];
  int len;
  bign() {
    memset(d, 0, sizeof(d));
    len = 0;
  }
};
bign change(char str[]) {
  bign a;
  a.len = strlen(str);
  for (int i = 0; i < a.len; i++) {
    a.d[i] = str[a.len - 1 - i] - '0';
  }
  return a;
}

int compare(bign a, bign b) {
  if (a.len > b.len)
    return 1;
  else if (a.len < b.len)
    return -1;
  else {
    for (int i = a.len - 1; i >= 0; i--) {
      if (a.d[i] > b.d[i])
        return 1;
      else if (a.d[i] < b.d[i])
        return -1;
    }
    return 0;
  }
}
bign add(bign a, bign b) {
  bign c;
  int carry = 0;
  for (int i = 0; i < a.len || i < b.len; i++) {
    int temp = a.d[i] + b.d[i] + carry;
    c.d[c.len++] = temp % 10;
    carry = temp / 10;
  }
  if (carry != 0) {
    c.d[c.len++] = carry;
  }
}
bign sub(bign a, bign b) {
  bign c;
  for (int i = 0; i < a.len || i < b.len; i++) {
    if (a.d[i] < b.d[i]) {
      a.d[i + 1]--;
      a.d[i] += 10;
    }
    c.d[c.len++] = a.d[i] - b.d[i];
  }
  while (c.len - 1 >= 1 && c.d[c.len - 1] == 0) {
    c.len--;
  }
  return c;
}

bign multi(bign a, int b) {
  bign c;
  int carry = 0;
  for (int i = 0; i < a.len; i++) {
    int temp = a.d[i] * b + carry;
    c.d[c.len++] = temp % 10;
    carry = temp / 10;
  }
  while (carry != 0) {
    c.d[c.len++] = carry % 10;
    carry /= 10;
  }
  return c;
}

bign divide(bign a, int b, int& r) {
  bign c;
  c.len = a.len;
  for (int i = a.len - 1; i >= 0; i--) {
    r = r * 10 + a.d[i];
    if (r < b)
      c.d[i] = 0;
    else {
      c.d[i] = r / b;
      r = r % b;
    }
  }
  while (c.len - 1 >= 1 && c.d[c.len - 1] == 0) {
    c.len--;
  }
}
}  // namespace BigNum

namespace primefactor {
int cal(int n, int p) {
  int ans = 0;
  for (int i = 2; i <= n; i++) {
    int temp = i;
    while (temp % p == 0) {
      ans++;
      temp /= p;
    }
  }
}
int cal2(int n, int p) {
  int ans = 0;
  while (n) {
    ans += (n / p);
    n /= p;
  }
  return ans;
}
int cal3(int n, int p) {
  if (n < p)
    return 0;
  return n / p + cal(n / p, n);
}

long long C(long long n, long long m) {
  long long ans = 1;
  for (long long i = 1; i <= n; i++) {
    ans *= i;
  }
  for (long long i = 1; i <= m; i++) {
    ans /= i;
  }
  for (long long i = 1; i <= n - m; i++) {
    ans /= i;
  }
  return ans;
}

long long C2(long long n, long long m) {
  long long ans = 1;
  if (m == 0 || m == n)
    return ans;
  return C2(n - 1, m) + C2(n - 1, m - 1);
}

long long res[67][67] = {0};
long long C3(long long n, long long m) {
  if (m == 0 || m == n)
    return 1;
  if (res[n][m] != 0)
    return res[n][m];
  return res[n][m] = C3(n - 1, m) + C3(n - 1, m - 1);
}

long long res2[61][61] = {0};
void CalC(long long arr[], int n) {
  for (int i = 0; i <= n; i++) {
    res2[i][0] = res2[i][i] = 1;
  }
  for (int i = 2; i <= n; i++) {
    for (int j = 0; j <= i / 2; j++) {
      res2[i][j] = res2[i - 1][j] + res2[i - 1][j - 1];
      res2[i][i - j] = res2[i][j];
    }
  }
}

long long C4(long long n, long long m) {
  long long ans = 1;
  for (long long i = 1; i <= m; i++) {
    ans = ans * (n - m + i) / i;
  }
  return ans;
}

int res3[1010][1010] = {0};
int Cmodp(int n, int m, int p) {
  if (m == n || m == 0)
    return 1;
  if (res3[n][m] != 0)
    return res3[n][m];
  return res3[n][m] = (Cmodp(n - 1, m, p) + Cmodp(n - 1, m - 1, p)) % p;
}

void CalCmodp(int n, int p) {
  for (int i = 0; i <= n; i++) {
    res3[i][0] = res3[i][i] = 1;
  }
  for (int i = 2; i <= n; i++) {
    for (int j = 0; j <= i / 2; j++) {
      res3[i][j] = (res3[i - 1][j] + res3[i - 1][j - 1]) % p;
      res3[i][i - j] = res3[i][j];
    }
  }
}

int CalCmodp2(int n, int m, int p) {
  int prime[n + 1] = {0}, pNum = 0;
  bool isnp[n + 1] = {0};
  auto getPrimeTable = [&](int n) {
    for (int i = 2; i <= n; i++) {
      if (isnp[i] == false) {
        prime[pNum++] = i;
        for (int j = i + i; j <= n; j += i) {
          isnp[j] = true;
        }
      }
    }
  };
  auto Calfactor = [](long long n, long long p) {
    long long ans = 0;
    while (n) {
      ans += (n / p);
      n /= p;
    }
    return ans;
  };
  std::function<long long(long long, long long, long long)> binaryPow =
      [&binaryPow](long long a, long long b, long long p) -> long {
    if (b == 0)
      return 1;
    if (b & 1) {
      return a * binaryPow(a, b - 1, p) % p;
    } else {
      auto multi = binaryPow(a, b / 2, p);
      return multi * multi % p;
    }
  };
  getPrimeTable(n);
  int ans = 1;
  for (int i = 0; prime[i] <= n && i < pNum; i++) {
    int c = Calfactor(n, prime[i]) - Calfactor(m, prime[i]) -
            Calfactor(n - m, prime[i]);
    ans = (ans * binaryPow(prime[i], c, p)) % p;
  }
  return ans;
}

long long Lucas(long long n, long long m, int p) {
  auto C = [](long long n, long long m) -> long long {
    if (m == 0)
      return 1;
    else {
      long long ans = 1;
      for (int i = 1; i <= m; i++) {
        ans = (ans * (n - m + i) / i);
      }
      return ans;
    }
  };
  if (m == 0)
    return 1;
  return C(n % p, m % p) * Lucas(n / p, m / p, p) % p;
}

}  // namespace primefactor

}  // namespace math_
namespace stack_ {

std::map<char, int> opt_map = {{'*', 2}, {'/', 2}, {'+', 1}, {'-', 1}};

void experssion() {
  std::stack<double> nums;
  std::stack<char> opts;
  std::string line;
  std::getline(std::cin, line);

  for (int i = 0; i < line.size(); i++) {
    std::string is_num_temp;
    if (std::isdigit(line[i]) || line[i] == '.') {
      while ((std::isdigit(line[i]) || line[i] == '.') && i < line.size()) {
        is_num_temp += line[i];
        i++;
      }
      i--;
      nums.push(std::stod(is_num_temp));
    } else {
      char ch = line[i];
      if (!opts.empty()) {
        while (!opts.empty() && opt_map[ch] <= opt_map[opts.top()]) {
          char op = opts.top();
          opts.pop();
          double right = nums.top();
          nums.pop();
          double left = nums.top();
          nums.pop();
          switch (op) {
            case '*':
              nums.push(left * right);
              break;
            case '/':
              nums.push(left / right);
              break;
            case '-':
              nums.push(left - right);
              break;
            case '+':
              nums.push(left + right);
              break;
          }
        }
      }
      opts.push(ch);
    }
  }
  while (!opts.empty()) {
    char op = opts.top();
    opts.pop();
    double right = nums.top();
    nums.pop();
    double left = nums.top();
    nums.pop();
    switch (op) {
      case '*':
        nums.push(left * right);
        break;
      case '/':
        nums.push(left / right);
        break;
      case '-':
        nums.push(left - right);
        break;
      case '+':
        nums.push(left + right);
        break;
    }
  }
  double res = nums.top();
  std::cout << std::fixed << std::setprecision(2) << res << std::endl;
}

}  // namespace stack_

namespace dfs {
void dfs() {
  const int maxn = 30;
  int n, v, maxvalue = 0;
  int w[maxn], c[maxn];
  std::function<void(int, int, int)> DFS = [&](int index, int sumW, int sumC) {
    if (index == n) {
      if (sumW <= v && sumC > maxvalue) {
        maxvalue = sumC;
      }
      return;
    }
    DFS(index + 1, sumW, sumC);
    DFS(index + 1, sumW + w[index], sumC + c[index]);
  };
  std::cin >> n >> v;
  for (int i = 0; i < n; i++) {
    std::cin >> w[i] >> c[i];
  }
  DFS(0, 0, 0);
  std::cout << maxvalue;
}

void dfs2() {
  const int maxn = 30;
  int n, v;
  int w[maxn], c[maxn];
  int maxvalue = 0;
  std::function<void(int, int, int)> DFS = [&](int index, int sumW, int sumC) {
    if (index == n) {
      return;
    }
    DFS(index + 1, sumW, sumC);
    if (sumW + w[index] <= v) {
      if (sumC + c[index] > maxvalue) {
        maxvalue = sumC + c[index];
      }
      DFS(index + 1, sumW + w[index], sumC + c[index]);
    }
  };
  std::cin >> n >> v;
  for (int i = 0; i < n; i++) {
    std::cin >> w[i] >> c[i];
  }
  DFS(0, 0, 0);
  std::cout << maxvalue;
}

void dfs3() {
  int n, k, x, maxSumSqu = -1;
  int A[n + 1];
  std::vector<int> temp, ans;
  std::function<void(int, int, int, int)> DFS = [&](int index, int nowK,
                                                    int sum, int sumSqu) {
    if (nowK == k && sum == x) {
      if (sumSqu > maxSumSqu) {
        maxSumSqu = sumSqu;
        ans = temp;
      }
      return;
    }
    if (index == n || nowK > k || sum > x) {
      return;
    }
    temp.push_back(A[index]);
    DFS(index, nowK + 1, sum + A[index], sumSqu + (A[index] * A[index]));
    DFS(index + 1, nowK + 1, sum + A[index], sumSqu + (A[index] * A[index]));
    temp.pop_back();
    DFS(index + 1, nowK, sum, sumSqu);
  };
  std::cin >> n >> k >> x;
  for (int i = 0; i < n; i++) {
    std::cin >> A[i];
  }
  DFS(0, 0, 0, 0);
  std::cout << maxSumSqu << " ";
  for (auto val : ans) {
    std::cout << val << " ";
  }
}

}  // namespace dfs

namespace bfs {
void bfs() {
  struct node {
    int x, y;
  };
  const int maxn = 100;
  int n, m;
  std::cin >> n >> m;
  int matrix[maxn][maxn];
  bool inq[maxn][maxn] = {false};
  int X[4] = {0, 0, 1, -1};
  int Y[4] = {1, -1, 0, 0};

  auto judge = [&](int x, int y) {
    return (x >= 0 && x < n && y >= 0 && y < m && matrix[x][y] == 1 &&
            !inq[x][y]);
  };

  auto BFS = [&](int x, int y) {
    std::queue<node> Q;
    node Node;
    Node.x = x;
    Node.y = y;
    Q.push(Node);
    inq[x][y] = true;
    while (!Q.empty()) {
      node top = Q.front();
      Q.pop();
      for (int i = 0; i < 4; i++) {
        int newX = top.x + X[i];
        int newY = top.y + Y[i];
        if (judge(newX, newY)) {
          Node.x = newX;
          Node.y = newY;
          Q.push(Node);
          inq[newX][newY] = true;
        }
      }
    }
  };

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      std::cin >> matrix[i][j];
    }
  }

  int ans = 0;
  for (int x = 0; x < n; x++) {
    for (int y = 0; y < m; y++) {
      if (matrix[x][y] == 1 && !inq[x][y]) {
        ans++;
        BFS(x, y);
      }
    }
  }
  std::cout << ans;
}

void bfs2() {
  const int maxn = 100;
  struct node {
    int x, y;
    int step;
  } S, T, Node;
  int n, m;
  char maze[maxn][maxn];
  bool inqu[maxn][maxn] = {false};
  int X[4] = {0, 0, 1, -1};
  int Y[4] = {1, -1, 0, 0};
  auto test = [&](int x, int y) {
    if (x >= n || x < 0 || y >= m || y < 0)
      return false;
    if (maze[x][y] == '*')
      return false;
    if (inqu[x][y] == true)
      return false;
    return true;
  };

  std::function<int()> BFS = [&]() {
    std::queue<node> q;
    q.push(S);
    while (!q.empty()) {
      node top = q.front();
      q.pop();
      if (top.x == T.x && top.y == T.y) {
        return top.step;
      }
      for (int i = 0; i < 4; i++) {
        int newX = top.x + X[i];
        int newY = top.y + Y[i];
        if (test(newX, newY)) {
          Node.x = newX;
          Node.y = newY;
          Node.step = top.step + 1;
          q.push(Node);
          inqu[newX][newY] = true;
        }
      }
    }
    return -1;
  };
  std::cin >> n >> m;
  for (int i = 0; i < n; i++) {
    getchar();
    for (int j = 0; j < m; j++) {
      maze[i][j] = getchar();
    }
    maze[i][m + 1] = '\0';
  }
  std::cin >> S.x >> S.y >> T.x >> T.y;
  S.step = 0;
  std::cout << BFS();
}

}  // namespace bfs

namespace binaryTree {
struct node {
  int data;
  node* lchild;
  node* rchild;
};

void search(node* root, int x, int newdata) {
  if (root == nullptr) {
    return;
  }
  if (root->data == x) {
    root->data = newdata;
  }
  search(root->lchild, x, newdata);
  search(root->rchild, x, newdata);
}
void insert(node*& root, int x) {
  if (root == nullptr) {
    root = new node;
    root->data = x;
    return;
  }
  if (root->lchild) {
    insert(root->lchild, x);
  } else {
    insert(root->rchild, x);
  }
}
node* create(int data[], int n) {
  node* root = nullptr;
  for (int i = 0; i < n; i++) {
    insert(root, data[i]);
  }
  return root;
}
int pre[100], in[100];
node* create(int preL, int preR, int inL, int inR) {
  if (preL > preR) {
    return nullptr;
  }
  node* root = new node;
  root->data = pre[preL];
  int k;
  for (k = inL; k <= inR; k++) {
    if (in[k] == pre[preL]) {
      break;
    }
  }
  int numLeft = k - inL;
  root->lchild = create(preL + 1, preL + numLeft, inL, k - 1);
  root->rchild = create(preL + numLeft + 1, preR, k + 1, inR);
  return root;
}

void createTree() {
  const int maxn = 50;
  struct node {
    int data;
    node* lchild;
    node* rchild;
  };
  int pre[maxn], in[maxn], post[maxn];
  int n;
  std::function<node*(int, int, int, int)> create =
      [&](int postL, int postR, int inL, int inR) -> node* {
    if (postL > postR) {
      return nullptr;
    }
    node* root = new node;
    root->data = post[postR];
    int k;
    for (k = inL; k <= inR; k++) {
      if (post[postR] == in[k]) {
        break;
      }
    }
    int numLeft = k - inL;
    root->lchild = create(postL, postL + numLeft - 1, inL, k - 1);
    root->rchild = create(postL + numLeft, postR - 1, k + 1, inR);
    return root;
  };
  int num = 0;
  auto bfs = [&](node* root) {
    std::queue<node*> q;
    q.push(root);
    while (!q.empty()) {
      node* now = q.front();
      q.pop();
      printf("%d", now->data);
      num++;
      if (num < n)
        printf(" ");
      if (now->lchild != nullptr)
        q.push(now->lchild);
      if (now->rchild != nullptr)
        q.push(now->rchild);
    }
  };
  scanf("%d", &n);
  for (int i = 0; i < n; i++) {
    scanf("%d", &post[i]);
  }
  for (int i = 0; i < n; i++) {
    scanf("%d", &in[i]);
  }
  node* root = create(0, n - 1, 0, n - 1);
  bfs(root);
  return;
}
const int maxn = 100;
struct node_ {
  int data;
  int lchild;
  int rchild;
} Node[maxn];
int index = 0;
int newNode(int v) {
  Node[index].data = v;
  Node[index].lchild = -1;
  Node[index].rchild = -1;
}
void search(int root, int x, int newdata) {
  if (root == -1) {
    return;
  }
  if (Node[root].data == x) {
    Node[root].data = newdata;
  }
  search(Node[root].lchild, x, newdata);
  search(Node[root].rchild, x, newdata);
}
void insert(int& root, int x) {
  if (root == -1) {
    root = x;
    return;
  }
  if (Node[root].lchild != -1) {
    insert(Node[root].lchild, x);
  } else {
    insert(Node[root].rchild, x);
  }
}

int create_(int data[], int n) {
  int root = -1;
  for (int i = 0; i < n; i++) {
    insert(root, data[i]);
  }
  return root;
}

}  // namespace binaryTree

namespace tree {
const int maxn = 100;
struct node {
  int data;
  std::vector<int> child;
} Node[maxn];
int index = 0;
int newNode(int v) {
  Node[index].data = v;
  Node[index].child.clear();
  return index++;
}
void preorder(int root) {
  printf("%d", Node[root].data);
  for (int i = 0; i < Node[root].child.size(); i++) {
    preorder(Node[root].child[i]);
  }
}
void LayerOrder(int root) {
  std::queue<int> Q;
  Q.push(root);
  while (!Q.empty()) {
    int front = Q.front();
    printf("%d", Node[front].data);
    Q.pop();
    for (int i = 0; i < Node[front].child.size(); i++) {
      Q.push(Node[front].child[i]);
    }
  }
}

void pathofequalweight() {
  const int maxn = 110;
  struct node {
    int weight;
    std::vector<int> child;
  } Node[maxn];
  auto cmp = [&](int a, int b) { return Node[a].weight > Node[b].weight; };
  int n, m, s;
  int path[maxn];
  std::function<void(int, int, int)> dfs = [&](int index, int numnode,
                                               int sum) {
    if (sum > s)
      return;
    if (sum == s) {
      if (Node[index].child.size() != 0)
        return;
      for (int i = 0; i < numnode; i++) {
        printf("%d", Node[path[i]].weight);
        if (i < numnode - 1)
          printf(" ");
        else
          printf("\n");
      }
      return;
    }
    for (int i = 0; i < Node[index].child.size(); i++) {
      int child = Node[index].child[i];
      path[numnode] = child;
      dfs(child, numnode + 1, sum + Node[child].weight);
    }
  };
  scanf("%d%d%d", &n, &m, &s);
  for (int i = 0; i < n; i++) {
    scanf("%d", &Node[i].weight);
  }
  int id, k, child;
  for (int i = 0; i < m; i++) {
    scanf("%d%d", &id, &k);
    for (int j = 0; j < k; j++) {
      scanf("%d", &child);
      Node[id].child.push_back(child);
    }
    std::sort(Node[id].child.begin(), Node[id].child.end(), cmp);
  }
  path[0] = 0;
  dfs(0, 1, Node[0].weight);
}
}  // namespace tree

namespace linerTree {
std::vector<int> arr = {1, 2, 3, 4, 5};
std::vector<int> brr(std::pow(2.0, int(log(5)) + 1));
void build(int s, int t, int p) {
  if (s = t) {
    brr[p] = arr[s];
    return;
  }
  int m = s + ((t - s) >> 1);
  build(s, m, p * 2), build(m + 1, t, p * 2 + 1);
  brr[p] = brr[p * 2] + brr[(p * 2) + 1];
}

int getsum(int l, int r, int s, int t, int p) {
  if (l <= s && t <= r) {
    return brr[p];
  }
  int m = s + ((t - s) >> 1), sum = 0;
  if (l <= m)
    sum += getsum(l, r, s, m, p * 2);
  if (r > m)
    sum += getsum(l, r, m + 1, t, p * 2 + 1);
  return sum;
}
}  // namespace linerTree

int main() {}