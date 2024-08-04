#include <vector>

class Solution {
public:
    std::vector<bool> kidsWithCandies(std::vector<int>& candies, int extraCandies) {
        int maxCandies = 0;
        for (int candy : candies) {
            if (candy > maxCandies) {
                maxCandies = candy;
            }
        }
        std::vector<bool> result;
        result.reserve(candies.size());

        for (int candy : candies) {
            result.push_back(candy + extraCandies >= maxCandies);
        }
        return result;
    }
};