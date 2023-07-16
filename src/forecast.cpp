#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include "forecast.hpp"
#include "output_interface.hpp"

using json = nlohmann::json;

namespace forecast {
    Forecast::Forecast(const json forecastData, const Location* location) : m_location(location) {
        const json periods = forecastData["properties"]["periods"];
        for (const json& period : periods) {
            m_periods.push_back(period);
        }

        m_startDate = m_periods.front()["startTime"];
        m_endDate = m_periods.back()["endTime"];
    }

    const std::vector<json>* Forecast::getPeriods() {
        return &m_periods;
    }

    const Location* Forecast::getLocation() {
        return m_location;
    }

    const std::string Forecast::getStartDate() {
        return m_startDate;
    }

    const std::string Forecast::getEndDate() {
        return m_endDate;
    }

    void Forecast::render(OutputInterface* renderer, int days) {
        renderer->render(this, days);
    }
}
