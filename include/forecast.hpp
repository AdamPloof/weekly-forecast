#ifndef FORECAST_H
#define FORECAST_H

#include <string>
#include <vector>
#include <memory>
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
    };

    class Forecast {
        public:
            Forecast();
            Forecast(int days);
            void setTempScale(TempScale);
            void fetchForecast();
            // ~Forecast();
        
        private:
            int m_days;
            std::vector<DailyForecast> m_forecastDays;
            TempScale m_tempScale;
            std::string m_office;
            int m_gridX;
            int m_gridY;
    };
}

#endif
