#ifndef FORECAST_PARAMS_H
#define FORECAST_PARAMS_H

#include <string>

namespace wf {
    struct ForecastParams {
        std::string zip;
        int days;

        ForecastParams();
    };   
}

#endif