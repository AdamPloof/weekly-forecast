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
    class OutputInterface;

    class Forecast {
        public:
            Forecast(const json forecastData, const Location* location);
            void setRenderer(std::shared_ptr<OutputInterface> renderer);
            const std::vector<json>* getPeriods();
            const Location* getLocation();
            const std::string getStartDate();
            const std::string getEndDate();
            void render(int days);
            // ~Forecast();
        
        private:
            const Location* m_location;
            std::string m_startDate;
            std::string m_endDate;
            std::vector<json> m_periods;
            std::shared_ptr<OutputInterface> m_renderer;
    };
}

#endif
