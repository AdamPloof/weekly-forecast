#ifndef FORECAST_H
#define FORECAST_H

#include <string>
#include <vector>
#include "zip_code.hpp"

namespace wf {
    // TODO: move TempScale and DailyForecast out of here.
    enum TempScale {
        FAHRENHEIT,
        CELSIUS
    };

    struct DailyForecast {
        std::string date;
        std::string weekday;
        std::string summaryDay;
        std::string summaryNight;
        std::string details;
        int tempHigh;
        int tempLow;
        TempScale tempUnit;
    };

    class Forecast {
        public:
            Forecast();
            Forecast(std::string zip);
            Forecast(std::string zip, int days);
            void setTempUnit(TempScale);
            void fetchForecast();
            ~Forecast();
        
        private:
            int days;
            ZipCode* zip;
            std::vector<DailyForecast> forecastDays;
    };
}

#endif
