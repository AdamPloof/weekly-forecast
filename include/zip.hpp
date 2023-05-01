#ifndef ZIP_HEADER
#define ZIP_HEADER

#include <string>

namespace weekly_forecast {
    class Zip {
        public:
            Zip(std::string code);
            Zip(int code);
            const std::string getCode();

        private:
            bool isValid(const std::string* code);
            std::string m_code;

    };
} // namespace weeklyForecast

#endif
