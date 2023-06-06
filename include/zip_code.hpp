#ifndef ZIP_H
#define ZIP_H

#include <string>

namespace forecast {
    class ZipCode {
        public:
            ZipCode(std::string code);
            ZipCode(int code);
            const std::string getCode();

        private:
            bool isValid(const std::string* code);
            std::string m_code;

    };
}

#endif
