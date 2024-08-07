#include <vector>
#include <math>
/**
 * 给你一个整数数组 nums ，判断这个数组中是否存在长度为 3 的递增子序列。
 * 如果存在这样的三元组下标 (i, j, k) 且满足 i < j < k ，
 * 使得 nums[i] < nums[j] < nums[k] ，返回 true ；
 * 否则，返回 false 。
*/
class Solution {
public:
    bool increasingTriplet(std::vector<int>& nums) {
        int len = nums.size();
        if (len < 3) {
            return false;
        }
        std::vector<int> left_min(len);
        left_min[0] = nums[0];
        std::vector<int> right_max(len);
        right_max[len-1] = nums[len-1];
        // 每个位置左侧最小的数字
        for (int i= 1; i<len; i++) {
            left_min[i] = min(left_min[i-1], nums[i]);
        }
        // 每个位置右侧最大的数字
        for (int j = len-2; j>=0; j--) {
            right_max[j] = max(right_max[j+1], nums[j]);
        }
        
        for (int n = 1; n< len-1; n++) {
            if (left_min[n-1] < nums[n] && nums[n] < right_max[n+1]) {
                return true;
            }
        }
        return false;
    }
};