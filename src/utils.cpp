#include <algorithm>
#include "utils.hpp"

namespace forecast {
    std::string strToLower(std::string s) {
        std::transform(s.cbegin(), s.cend(), s.begin(), [](unsigned char c) {
            return std::tolower(c); 
        });

        return s;
    }
}
