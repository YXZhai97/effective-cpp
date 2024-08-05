#include <vector>
#include <string>
#include <iostream>

class Solution {
public:
    int compress(std::vector<char>& chars) {
        int n = chars.size();
    int i = 0, j = 0;

    while (i < n) {
        char currentChar = chars[i];
        int count = 0;

        // Count the number of occurrences of the current character
        while (i < n && chars[i] == currentChar) {
            i++;
            count++;
        }

        // Write the character to the new position
        chars[j++] = currentChar;

        // If the character count is greater than 1, write the count as well
        if (count > 1) {
            std::string count_str = std::to_string(count);
            for (char c : count_str) {
                chars[j++] = c;
            }
        }
    }

    return j;

    }
};