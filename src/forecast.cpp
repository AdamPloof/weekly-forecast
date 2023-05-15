#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include "forecast.hpp"
#include "zip_code.hpp"


namespace wf {
    Forecast::Forecast(int days) : m_days {days}, m_tempScale {TempScale::FAHRENHEIT} {}
    Forecast::Forecast() : m_days {5}, m_tempScale {TempScale::FAHRENHEIT} {}

    void Forecast::setTempScale(TempScale scale) {
        m_tempScale = scale;
    }

    void Forecast::fetchForecast() {
        std::cout << "Forecast fetched...";
    }
}
