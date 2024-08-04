#include <iostream>

/**
 * 给你两个字符串 word1 和 word2 。
 * 请你从 word1 开始，通过交替添加字母来合并字符串。
 * 如果一个字符串比另一个字符串长，就将多出来的字母追加到合并后字符串的末尾。
 * 返回 合并后的字符串。
*/

/**
 * 单指针解法
*/
class Solution {
public:
    std::string mergeAlternately(std::string word1, std::string word2) {
        int n1 = word1.length();
        int n2 = word2.length();
        int i = 0;
        std::string res;
        while(i < n1 || i < n2){
            if(i < n1){
                res += word1[i];
            }
            if(i < n2){
                res += word2[i];
            }
            i++;
        }
        return res;
    }
};