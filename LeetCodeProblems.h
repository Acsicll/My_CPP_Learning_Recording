#ifndef _LEETCODEPROBLEMS_H_
#define _LEETCODEPROBLEMS_H_

#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <unordered_set>
#include <vector>

namespace ListNode {
struct ListNode
{
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}
};
class Solution {
  public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2)
    {
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
            }
            else {
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

void TestListNode()
{
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
    int lengthOfLongestSubstring(std::string s)
    {
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
void TestLengthOfLongestSubstring()
{
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
                                  std::vector<int>& nums2)
    {
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
        }
        else {
            res = (static_cast<double>(ivec[size / 2 - 1]) +
                   static_cast<double>(ivec[size / 2])) /
                  2 * 1.0f;
        }
        return res;
    }
};
void TestFindMedianSortedArrays()
{
    std::vector<int> nums1 = {1, 3};
    std::vector<int> nums2 = {2};
    Solution s;
    std::cout << s.findMedianSortedArrays(nums1, nums2) << " ";
}
}  // namespace findMedianSortedArrays

#endif