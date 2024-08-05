#include<vector>

class Solution {
public:
    bool canPlaceFlowers(std::vector<int>& flowerbed, int n) {
        int len = flowerbed.size();
        for (int i = 0; i < len && n> 0 ; ++i ) {
            if( flowerbed[i] == 0 ) {
                bool leftEmpty = ( i==0 || flowerbed[i-1] == 0);
                bool rightEmpty = ( i==len-1 || flowerbed[i+1] == 0);
                if (leftEmpty && rightEmpty) {
                    --n;
                    flowerbed[i] = 1;
                }
            }
        
        }
        return n == 0;
    }
};