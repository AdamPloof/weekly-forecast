#ifndef LINE_FORMATTER_H
#define LINE_FORMATTER_H

#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <unordered_map>

namespace forecast::utils {
    class LineFormatter {
        struct WordBreakPair {
            std::string before;
            std::string after;
        };

        public:
            LineFormatter();
            WordBreakPair breakWord(const std::string word, const int maxLength);
            std::vector<std::string> breakLines(const std::string content, const int lineWidth);

        private:
            std::unordered_map<char, bool> m_consonantMap;
    };
}

#endif
