#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include "forecast.hpp"
#include "zip_code.hpp"


namespace wf {
    Forecast::Forecast() : m_days {5} {
        // TODO: get default zip from config file.
        std::unique_ptr<ZipCode> m_zip = std::make_unique<ZipCode>("05401");
    }

    Forecast::Forecast(std::string zip) : m_days {5} {
        std::unique_ptr<ZipCode> m_zip = std::make_unique<ZipCode>(zip);
    }

    Forecast::Forecast(int days) : m_days {days} {
        std::unique_ptr<ZipCode> m_zip = std::make_unique<ZipCode>("05401");
    }

    Forecast::Forecast(std::string zip, int days) : m_days {days} {
        std::unique_ptr<ZipCode> m_zip = std::make_unique<ZipCode>(zip);
    }

    void Forecast::setTempUnit(TempScale scale) {
        m_tempScale = scale;
    }

    void Forecast::fetchForecast() {
        std::cout << "Forecast fetched...";
    }
}
