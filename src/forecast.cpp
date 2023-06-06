#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include "forecast.hpp"
#include "zip_code.hpp"

using json = nlohmann::json;

namespace forecast {
    Forecast::Forecast(const json forecastData, const std::string location) : m_location(location) {
        const json periods = forecastData["properties"]["periods"];
        for (const json& period : periods) {
            m_periods.push_back(period);
        }

        m_startDate = m_periods.front()["startTime"];
        m_endDate = m_periods.back()["endTime"];
    }

    void Forecast::printForecast() {
        std::ostringstream output;
        makeHeader(&output);

        for (const json& period : m_periods) {
            makePeriod(&output, period);

            // Separate dates by long dividers, day and night by short dividers.
            if (period["isDaytime"] == false) {
                output << makeDivider(true);
            } else {
                output << makeDivider();
            }
        }

        std::cout << output.str() << std::endl;
    }

    void Forecast::makeHeader(std::ostringstream* output) {
        *output << makeTitle() << "\n";
        *output << m_startDate << " - " << m_endDate << "\n";
        *output << m_location;

        *output << makeDivider(true);
    }

    void Forecast::makePeriod(std::ostringstream* output, const json period) {
        std::string temperature;
        int tempVal = period["temperature"];
        if (period["isDaytime"] == true) {
            temperature = "high: " + std::to_string(tempVal);
        } else {
            temperature = "low: " + std::to_string(tempVal);
        }
        temperature.append(period["temperatureUnit"]);

        int precipVal;
        if (period["probabilityOfPrecipitation"]["value"] == nullptr) {
            precipVal = 0;
        } else {
            precipVal = period["probabilityOfPrecipitation"]["value"];
        }
        std::string precip = "precipitation: " + std::to_string(precipVal) + "%";

        int humidityVal;
        if (period["relativeHumidity"]["value"] == nullptr) {
            humidityVal = 0;
        } else {
            humidityVal = period["relativeHumidity"]["value"];
        }
        std::string humidity = "humidty: " + std::to_string(humidityVal) + "%";

        *output << period["name"] << "\n";
        *output << period["shortForecast"] 
            << " | " << temperature << " | " << humidity << " | " << precip << "\n";
        *output << period["detailedForecast"];
    }

    // TODO: make a cool title
    std::string Forecast::makeTitle() {
        std::string title = "Forecast";

        return title;
    }

    // Includes a new line before an after the divider.
    std::string Forecast::makeDivider(bool isLong) {
        std::string divider;
        if (isLong) {
            divider = "\n\n*** --- ***\n\n";
        } else {
            divider = "\n\n---\n\n";
        }

        return divider;
    }
}
