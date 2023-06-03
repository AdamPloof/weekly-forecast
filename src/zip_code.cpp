#include <string>
#include <iostream>
#include <stdio.h>
#include "zip_code.hpp"

namespace forecast {
    // Note: the correct format for a numeric ZIP+4 code is five digits, a hyphen, and four digits.
    ZipCode::ZipCode(const std::string code) : m_code {code} {
        if (!isValid(&code)) {
            // throw error
            std::cout << "Zip is not valid";
        }
    }

    ZipCode::ZipCode(const int code) {
        const std::string strCode = std::to_string(code);
        if (!isValid(&strCode)) {
            // throw error
            std::cout << "Zip is not valid";
        }

        m_code = strCode;
    }

    bool ZipCode::isValid(const std::string* code) {
        bool valid = true;
        const int zipLen = code->length();

        if (zipLen != 5 && zipLen != 10) {
            valid = false;
        }

        if (zipLen == 10 && code->at(5) != '-') {
            valid = false;
        }

        return valid;
    }

    const std::string ZipCode::getCode() {
        return m_code;
    }
} // weekly_forecast
