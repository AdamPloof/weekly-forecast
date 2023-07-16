#ifndef FORECAST_H
#define FORECAST_H

#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <nlohmann/json.hpp>
#include "location.hpp"
#include "output_interface.hpp"

using json = nlohmann::json;

namespace forecast {
    class Forecast {
        public:
            Forecast(const json forecastData, const Location* location);
            const std::vector<json>* getPeriods();
            const Location* getLocation();
            const std::string getStartDate();
            const std::string getEndDate();
            void render(OutputInterface* renderer, int days);
            // ~Forecast();
        
        private:
            const Location* m_location;
            std::string m_startDate;
            std::string m_endDate;
            std::vector<json> m_periods;
    };
}

#endif
