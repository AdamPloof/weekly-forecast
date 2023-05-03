#ifndef FORECAST_PARAMS_H
#define FORECAST_PARAMS_H

#include <string>
#include "zip.hpp"

namespace weekly_forecast {
    struct ForecastParams {
        std::string zip;
        int days;

        ForecastParams();
    };   
}

#endif