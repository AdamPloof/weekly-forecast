#include "line_formatter.hpp"

namespace forecast::utils {
    LineFormatter::LineFormatter() {
        for (char c = 'a'; c <= 'z'; c++) {
            if (c == 'a' || c == 'e' || c == 'i' || c == 'o'|| c == 'u') {
                m_consonantMap[c] = false;
            } else {
                m_consonantMap[c] = true;
            }
        }

        for (char c = 'A'; c <= 'Z'; c++) {
            if (c == 'A' || c == 'E' || c == 'I' || c == 'O'|| c == 'U') {
                m_consonantMap[c] = false;
            } else {
                m_consonantMap[c] = true;
            }
        }
    }

    /**
     * Break a word at the last consonant at least 2 characters before n characters
     * not as pretty as breaking on syllables, but this will do for now.
     * 
     * Example
     * maxLength = 10
     * 
     * 0123456 // start searching for consonant here
     * ATESTWORDISSUPERLONGWORD
     * 012345  // find consonant here
     * 
     * before: ATESTW-
     * after : ORDISSUPERLONGWORD
     * 
     */ 
    LineFormatter::WordBreakPair LineFormatter::breakWord(const std::string word, const int maxLength) {
        WordBreakPair pair;
        if (word.length() <= maxLength) {
            pair.before = word;
            return pair;
        }

        for (int i = maxLength - 4; i > 0; i--) {
            if (m_consonantMap[word[i]] == true) {
                // before is everything up to this letter
                pair.before = word.substr(0, i + 1) + '-';
                pair.after = word.substr(i + 1, word.length() - i);
                break;
            }
        }

        return pair;
    }

    /**
     * Break a long string into a vector of lines at word boundries less than lineWidth.
     * If a single word is longer than the lineWidth, break the word at a consonant.
     */
    std::vector<std::string> LineFormatter::breakLines(const std::string content, const int lineWidth) {
        std::vector<std::string> lines;
        if (content.length() <= lineWidth) {
            lines.push_back(content);
            return lines;
        }

        std::string tempWord;
        std::string output;
        std::istringstream iss = std::istringstream(content);
        while (iss >> tempWord) {
            if (output == "" && tempWord.length() > lineWidth) {
                // Need to deal with the possibility that there are rreeealllly long words,
                // that might need to be broken multiple times. Probably not gonna happen :P
                while (tempWord.length() > lineWidth) {
                    WordBreakPair p = breakWord(tempWord, lineWidth);
                    lines.push_back(p.before);
                    tempWord = p.after;
                }
            }

            if ((tempWord.length() + output.length()) >= lineWidth) {
                output.pop_back();
                lines.push_back(output);
                output = "";
            }

            output += tempWord + " ";
        }

        if (output != "") {
            lines.push_back(output);
        }

        return lines;
    }
}
