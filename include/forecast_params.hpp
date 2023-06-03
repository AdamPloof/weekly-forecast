#ifndef FORECAST_PARAMS_H
#define FORECAST_PARAMS_H

#include <string>

namespace forecast {
    struct ForecastParams {
        std::string zip;
        int days;

        ForecastParams();
    };   
}

#endif