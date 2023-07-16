#include "row_printer.hpp"

namespace forecast {
    RowPrinter::RowPrinter() {}

    void RowPrinter::render(Forecast* forecast, int days) {
        makeHeader(forecast->getStartDate(), forecast->getEndDate(), forecast->getLocation());

        // Remember, there's two periods per day.
        int periodNum = 1;
        int maxPeriods = days * 2;
        for (const json& period : *forecast->getPeriods()) {
            if (periodNum >= maxPeriods && period["isDaytime"] == true) {
                break;
            }

            makePeriod(period);

            // Separate dates by long dividers, day and night by short dividers.
            if (period["isDaytime"] == false) {
                output << makeDivider(true);
            } else {
                output << makeDivider();
            }

            periodNum++;
        }

        std::cout << output.str() << std::endl;
    }

    void RowPrinter::makeHeader(const std::string startDate, const std::string endDate, const Location* loc) {
        output << makeTitle() << "\n";
        output << startDate << " - " << endDate << "\n";
        output << loc->city << ", " << loc->state;
        output << makeDivider(true);
    }

    void RowPrinter::makePeriod(const json& period) {
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

        output << period["name"] << "\n";
        output << period["shortForecast"] 
            << " | " << temperature << " | " << humidity << " | " << precip << "\n";
        output << period["detailedForecast"];
    }

    // TODO: make a cool title
    std::string RowPrinter::makeTitle() {
        std::string title = "Forecast";

        return title;
    }

    std::string RowPrinter::makeDivider(bool isLong) {
        std::string divider;
        if (isLong) {
            divider = "\n\n*** --- ***\n\n";
        } else {
            divider = "\n\n---\n\n";
        }

        return divider;
    }
}
