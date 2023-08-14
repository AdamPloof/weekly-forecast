#ifndef LINE_FORMATTER_H
#define LINE_FORMATTER_H

#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <unordered_map>

namespace forecast::utils {
    struct WordBreakPair {
        std::string before;
        std::string after;
    };

    class LineFormatter {

        public:
            LineFormatter();
            WordBreakPair breakWord(const std::string word, const size_t maxLength);
            std::vector<std::string> breakLines(const std::string content, const size_t lineWidth);

        private:
            std::unordered_map<char, bool> m_consonantMap;
    };
}

#endif
