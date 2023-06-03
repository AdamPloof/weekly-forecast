#ifndef FORECAST_H
#define FORECAST_H

#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <nlohmann/json.hpp>
#include "zip_code.hpp"

using json = nlohmann::json;

namespace forecast {
    class Forecast {
        public:
            Forecast(const json forecastData, const std::string location);
            void printForecast();
            // ~Forecast();
        
        private:
            std::string m_startDate;
            std::string m_endDate;
            const std::string m_location;
            std::vector<json> m_periods;

            void makeHeader(std::ostringstream* output);
            void makePeriod(std::ostringstream* output, json period);
            std::string makeTitle();
            std::string makeDivider(bool isLong = false);
    };
}

#endif
