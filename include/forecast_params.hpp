#ifndef FORECAST_PARAMS_H
#define FORECAST_PARAMS_H

#include "zip.hpp"

namespace weekly_forecast {
    struct ForecastParams {
        Zip* zip;
        int days;

        ForecastParams();
    };   
}

#endif