#ifndef _LEETCODEPROBLEMS_H_
#define _LEETCODEPROBLEMS_H_

#include <string.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <unordered_set>
#include <vector>
#include "few_resources_use_to_test.h"

namespace ListNode {
struct ListNode {
  int val;
  ListNode* next;
  ListNode() : val(0), next(nullptr) {}
  ListNode(int x) : val(x), next(nullptr) {}
  ListNode(int x, ListNode* next) : val(x), next(next) {}
};
class Solution {
 public:
  ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
    if (l1 == nullptr && l2 == nullptr)
      return nullptr;
    ListNode* l3 = nullptr;
    auto head_l1 = l1;
    auto head_l2 = l2;
    ListNode* tail_l3 = nullptr;
    int carry = 0;
    int res = 0;
    while (head_l1 || head_l2) {
      auto value_l1 = (head_l1) ? head_l1->val : 0;
      auto value_l2 = (head_l2) ? head_l2->val : 0;
      res = (value_l1 + value_l2 + carry) % 10;
      carry = (value_l1 + value_l2 + carry) >= 10 ? 1 : 0;
      if (l3 == nullptr) {
        l3 = new ListNode(res);
        tail_l3 = l3;
      } else {
        ListNode* new_node = new ListNode(res);
        tail_l3->next = new_node;
        tail_l3 = new_node;
      }
      head_l1 = (head_l1) ? head_l1->next : nullptr;
      head_l2 = (head_l2) ? head_l2->next : nullptr;
    }
    if (carry) {
      ListNode* new_node = new ListNode(carry);
      tail_l3->next = new_node;
      tail_l3 = new_node;
    }
    return l3;
  }
};

void TestListNode() {
  ListNode* L1 = new ListNode(2);
  ListNode* L2 = new ListNode(5);
  ListNode* L1_new_node = new ListNode(4, (new ListNode(3)));
  L1->next = L1_new_node;
  ListNode* L2_new_node = new ListNode(6, (new ListNode(4)));
  L2->next = L2_new_node;
  auto l1_tail = L1;
  auto l2_tail = L2;
  while (l1_tail && l2_tail) {
    std::cout << "l1 : " << l1_tail->val << " "
              << "l2 : " << l2_tail->val << "\n";
    l1_tail = l1_tail->next;
    l2_tail = l2_tail->next;
  }
  Solution s1;
  auto res = s1.addTwoNumbers(L1, L2);
  auto tail = res;
  while (tail) {
    std::cout << tail->val << " ";
    tail = tail->next;
  }
}
}  // namespace ListNode

namespace lengthOfLongestSubstring {
class Solution {
 public:
  int lengthOfLongestSubstring(std::string s) {
    int res = 0;
    if (!s.length()) {
      return res;
    }
    for (std::size_t i = 0; i < s.length(); ++i) {
      std::string substring;
      std::unordered_set<char> char_set;
      for (std::size_t j = i; j < s.length(); ++j) {
        char ch = s[j];
        if (char_set.find(ch) != char_set.end()) {
          break;
        }
        char_set.insert(ch);
        substring += s[j];
        res = (res > substring.length()) ? res : substring.length();
      }
      char_set.clear();
    }
    return res;
    // int res = 0;
    // if (!s.length()) {
    //     return res;
    // }
    // for (std::size_t i = 0; i < s.length(); ++i) {
    //     std::string substring;
    //     for (std::size_t j = i; j < s.length(); ++j) {
    //         substring += s[j];
    //         std::unordered_set<char> char_set;
    //         bool ispass = [&]() -> bool {
    //             for (auto ch : substring) {
    //                 if (char_set.find(ch) != char_set.end()) {
    //                     return true;
    //                 }
    //                 char_set.insert(ch);
    //             }
    //             return false;
    //         }();
    //         if (!ispass) {
    //             res = (res > substring.length()) ? res :
    //             substring.length();
    //         }
    //     }
    // }
    // return res;
  }
};
void TestLengthOfLongestSubstring() {
  Solution s;
  std::vector<std::string> vecstr = {"abcabcbb", "bbbbb", "pwwkew"};
  for (auto val : vecstr) {
    // setter.insert(val);
    std::cout << s.lengthOfLongestSubstring(val) << std::endl;
  }
}
}  // namespace lengthOfLongestSubstring

namespace findMedianSortedArrays {
class Solution {
 public:
  double findMedianSortedArrays(std::vector<int>& nums1,
                                std::vector<int>& nums2) {
    double res = 0.0;
    int size = nums1.size() + nums2.size();
    std::vector<int> ivec;
    std::copy(nums1.begin(), nums1.end(), std::back_inserter(ivec));
    std::copy(nums2.begin(), nums2.end(), std::back_inserter(ivec));
    std::sort(ivec.begin(), ivec.end());
    std::for_each(ivec.begin(), ivec.end(),
                  [](const int val) { std::cout << val << " "; });
    int nums = (size % 2 == 0) ? 2 : 1;
    if (nums == 1) {
      res = ivec[size / 2];
    } else {
      res = (static_cast<double>(ivec[size / 2 - 1]) +
             static_cast<double>(ivec[size / 2])) /
            2 * 1.0f;
    }
    return res;
  }
};
void TestFindMedianSortedArrays() {
  std::vector<int> nums1 = {1, 3};
  std::vector<int> nums2 = {2};
  Solution s;
  std::cout << s.findMedianSortedArrays(nums1, nums2) << " ";
}
}  // namespace findMedianSortedArrays

namespace longestPalindrome {
class Solution {
 public:
  std::string longestPalindrome(std::string s) {
    if (s.length() == 1) {
      return s;
    } else if (s.length() == 2 && s[0] == s[1]) {
      return s;
    }
    std::string res = "";
    for (int i = 0; i < s.length(); ++i) {
      std::string substring;
      for (int j = i; j < s.length(); ++j) {
        substring += s[j];
        // std::cout << substring << std::endl;
        auto first = substring.begin();
        auto end = substring.end() - 1;
        int sublength = substring.length();
        int choose = sublength % 2;
        while (*first == *end) {
          ++first;
          if (!choose && first == end) {
            break;
          }
          --end;
          if (choose && first == end) {
            break;
          }
        }
        if (sublength == 2 && substring[0] != substring[1]) {
          if (res.length() < sublength) {
            res.clear();
            res += substring[0];
          }
        }
        if (first == end && res.length() < sublength) {
          res.clear();
          res = substring;
        }
      }
    }
    return res;
  }
};
void TestLongestPalindrome() {
  std::vector<std::string> strvec = {"ac", "babad", "cbbd"};
  Solution s;
  for (auto val : strvec) {
    std::cout << s.longestPalindrome(val) << std::endl;
  }
}

}  // namespace longestPalindrome

namespace convert {
class Solution {
 public:
  std::string convert(std::string s, int numRows) {
    std::string res = "";
    int len = s.length();
    if (len <= 1 || len <= numRows) {
      return s;
    }
    if (numRows == 1) {
      return s;
    }
    // doing transform:
    int t = numRows * 2 - 2;
    int c = (len + numRows - 1) / t * (numRows - 1);
    std::vector<std::string> mat(numRows, std::string(c, 0));
    for (int i = 0, x = 0, y = 0; i < len; ++i) {
      mat[x][y] = s[i];
      if (i % t < numRows - 1) {
        ++x;
      } else {
        --x;
        ++y;
      }
    }
    for (auto& row : mat) {
      for (char ch : row) {
        if (ch)
          res += ch;
      }
    }
    return res;
  }
};
void TestConvert() {
  Solution S;

  std::multimap<std::string, int> test_map;
  test_map.insert(std::make_pair<std::string, int>("PAYPALISHIRING", 3));
  test_map.insert(std::pair<std::string, int>("PAYPALISHIRING", 4));
  test_map.insert(std::pair<std::string, int>("A", 1));
  test_map.insert(std::pair<std::string, int>("AB", 1));
  test_map.insert(std::pair<std::string, int>("ABC", 3));
  test_map.insert(std::make_pair("ABCDE", 4));
  test_map.insert(std::make_pair("ABCDEF", 5));
  test_map.insert(std::pair<std::string, int>("ABC", 2));

  for (auto val : test_map) {
    // std::cout << val.first << std::endl;
    std::cout << S.convert(val.first, val.second) << std::endl;
  }
}
}  // namespace convert

namespace reverse {
class Solution {
 public:
  int reverse(int x) {
    if (x > INT32_MAX || x < INT32_MIN || x == 0) {
      return 0;
    }
    int x_signed = ((x >= 0) ? 1 : -1);
    auto temp = abs(x);
    int res = 0;
    do {
      if (res > INT32_MAX / 10 || res < INT32_MIN / 10) {
        return 0;
      }
      res *= 10;
      res += (temp % 10);
    } while (temp /= 10);
    res = x_signed * res;
    return res;
  }
};
void TestReverse() {
  Solution s;
  std::cout << s.reverse(1534236469) << std::endl;
  // printValue(1, 2, 3, 4, 5, 65, 78);
}
}  // namespace reverse

namespace myAtio {
class Solution {
 public:
  int myAtio(std::string s) {
    auto len = s.length();
    if (!len) {
      return 0;
    }
    std::size_t not_space_index = s.find_first_not_of(' ');
    if (not_space_index != std::string::npos) {
      if (isalpha(s[not_space_index + 1])) {
        return 0;
      }
    }
    std::size_t is_negetive = s.find_first_of('-');
    std::size_t is_positive = s.find_first_of('+');
    int res_signed = 0;
    if (is_negetive != std::string::npos && is_positive != std::string::npos) {
      return 0;
    } else {
      res_signed = is_negetive != std::string::npos
                       ? -1
                       : (is_positive != std::string::npos ? 1 : 1);
    }

    int64_t res = 0;
    if (is_negetive == 0 && is_positive == 0) {
      return res;
    }
    int is_digit_num = 0;
    auto tmp = s;
    std::reverse(tmp.begin(), tmp.end());
    for (char ch : tmp) {
      if (ch >= '0' && ch <= '9') {
        int num = ch - '0';
        if (INT32_MAX / std::pow(10, is_digit_num) <= num) {
          if (res_signed == -1) {
            return INT32_MIN;
          }
          return INT32_MAX;
        }
        num *= std::pow(10, is_digit_num);
        if (INT32_MAX - num - res <= 0) {
          if (res_signed == -1) {
            return INT32_MIN;
          }
          return INT32_MAX;
        }
        res += num;
        ++is_digit_num;
      } else if (ch == '.') {
        res = 0;
        is_digit_num = 0;
      } else if (ch != '+' && ch != '-' && ch != ' ' && is_digit_num > 0) {
        res = 0;
        return res;
      } else if (ch == '+' || ch == '-') {
        break;
      }
    }
    res = res * res_signed;
    return res;
  }
};
void TestMyAtio() {
  Solution s;
  // std::cout << ;
  printValue(s.myAtio("420"), s.myAtio("   -42"), s.myAtio("words and 987"),
             s.myAtio("-91283472332"), s.myAtio("3.14159"), s.myAtio("+-12"),
             s.myAtio("+1"), s.myAtio("00000-42a1234"),
             s.myAtio("  0000000000012345678"), s.myAtio("  -0012a42"));
}
}  // namespace myAtio
#endif