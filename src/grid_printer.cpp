#include "grid_printer.hpp"

namespace forecast {
    const int GridPrinter::lineWidth = 129;

    GridPrinter::GridPrinter() : m_formatter(LineFormatter()), m_output() {}

    void GridPrinter::render(Forecast* forecast, int days) {
        xBorderMain();

        // Remember, there's two periods per day. But it's possible the first period is night time.
        // Also note that we advance either by 1 period or two depending on whether the first period is
        // day or night
        int dayNum = 1;
        const std::vector<json> periods = *forecast->getPeriods();
        for (size_t i = 0; i < periods.size(); ) {
            if (dayNum > days) {
                break;
            }

            const json& period = periods[i];
            std::string temp = period.dump();
            if (period["isDaytime"] == true) {
                makeDay(periods[i], periods[i + 1]);
                i += 2;
            } else {
                makeDay(periods[i]);
                i += 1;
            }

            dayNum++;
        }

        std::cout << m_output.str() << std::endl;
    }

    const std::ostringstream& GridPrinter::getOutput() const {
        return m_output;
    }

    // *---------------*---------------*---------------*---------------*
    void GridPrinter::xBorderMain() {
        for (int i = 0; i < 8; i++) {
            m_output << "*---------------";
        }

        m_output << "*\n";
    }

    // | ... __ ... __ = ... __ ... __ = ... __ ... __ = ... __ ... __ |
    void GridPrinter::xBorderSecondary() {
        m_output << "|";
        for (int i = 0; i < 16; i++) {
            m_output << " ... __";

            if ((i + 1) % 8 == 0) {
                m_output << " |";
            } else if ((i + 1) % 2 == 0) {
                m_output << " =";
            }
        }

        m_output << "\n";
    }

    // Note: makeDay refers to a *full* day, i.e. a day and a night period.
    void GridPrinter::makeDay(const json& day, const json& night) {
        std::string name = forecastSection(day["name"], night["name"]);
        m_output << name;

        std::string shortForecast = forecastSection(day["shortForecast"], night["shortForecast"]);
        m_output << shortForecast;

        std::string summary = forecastSection(periodSummary(day), periodSummary(night));
        m_output << summary;
        xBorderSecondary();

        std::string detail = forecastSection(day["detailedForecast"], night["detailedForecast"]);
        m_output << detail;

        xBorderMain();
    }

    // TODO: consolidate this into one method.
    void GridPrinter::makeDay(const json& night) {
        std::string name = forecastSection(" ", night["name"]);
        m_output << name;

        std::string shortForecast = forecastSection(" ", night["shortForecast"]);
        m_output << shortForecast;

        std::string summary = forecastSection(" ", periodSummary(night));
        m_output << summary;
        xBorderSecondary();

        std::string detail = forecastSection(" ", night["detailedForecast"]);
        m_output << detail;

        xBorderMain();
    }

    std::string GridPrinter::periodSummary(const json& period) {
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

        std::ostringstream ss;
        ss << temperature << " | " << humidity << " | " << precip;

        return ss.str();
    }

    // Reminder, each forecast section should have the same number of lines for day and night.
    // TODO: replace the hardcoded borders with something more flexible.
    std::string GridPrinter::forecastSection(const std::string dayData, const std::string nightData) {
        constexpr const int maxWidth = ((lineWidth - 1) / 2) - 3;
        std::vector<std::string> dayLines = m_formatter.breakLines(dayData, maxWidth);
        std::vector<std::string> nightLines = m_formatter.breakLines(nightData, maxWidth);

        size_t mostLines;
        if (dayLines.size() > nightLines.size()) {
            mostLines = dayLines.size();
        } else {
            mostLines = nightLines.size();
        }

        std::ostringstream section;
        for (size_t i = 0; i < mostLines; i++) {
            section << "| ";
            if (i > dayLines.size() - 1) {
                section << m_formatter.padLine(" ", maxWidth);
            } else {
                section << m_formatter.padLine(dayLines[i], maxWidth);
            }

            section << " | ";

            if (i > nightLines.size() - 1) {
                section << m_formatter.padLine(" ", maxWidth);
            } else {
                section << m_formatter.padLine(nightLines[i], maxWidth);
            }

            section << " |\n";
        }

        return section.str();

    }

    std::string GridPrinter::borderY(bool trailingSpace) {
        if (trailingSpace == true) {
            return "| ";
        } else {
            return "|";
        }
    }
}
