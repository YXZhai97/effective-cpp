#include <string>

/**
 * 给定字符串 s 和 t ，判断 s 是否为 t 的子序列。
 * 字符串的一个子序列是原始字符串删除一些（也可以不删除）
 * 字符而不改变剩余字符相对位置形成的新字符串。
 * （例如，"ace"是"abcde"的一个子序列，而"aec"不是）。
 * 解题思路： 双指针
*/
class Solution {
public:
    bool isSubsequence(std::string s, std::string t) {
        int s_len = s.length();
        int t_len = t.length();
        int i = 0, j = 0;
        if (s_len == 0) {
            return true;
        }
        if (t_len == 0 || s_len > t_len) {
            return false;
        }
        while (i<s_len && j<t_len) {
            if (s[i] == t[j]) {
                if (i == s_len -1) {
                    return true;
                }
                i++;
                j++;
            } else {
                j++;
            }
        }
        return false;
    }
};