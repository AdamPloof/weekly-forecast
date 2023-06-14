#ifndef FORECAST_H
#define FORECAST_H

#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <nlohmann/json.hpp>
#include "location.hpp"

using json = nlohmann::json;

namespace forecast {
    class Forecast {
        public:
            Forecast(const json forecastData, const Location location);
            void printForecast();
            // ~Forecast();
        
        private:
            Location m_location;
            std::string m_startDate;
            std::string m_endDate;
            std::vector<json> m_periods;

            void makeHeader(std::ostringstream* output);
            void makePeriod(std::ostringstream* output, json period);
            std::string makeTitle();
            std::string makeDivider(bool isLong = false);
    };
}

#endif
