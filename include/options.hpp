#ifndef FORECAST_OPTS_H
#define FORECAST_OPTS_H

#include <string>
#include "office.hpp"

namespace forecast {
    enum class Verbosity {
        LOW,
        STD,
        HIGH
    };

    struct Options {
        Coordinates coords;
        std::string location;
        Verbosity verbosityLvl;
        int days;
        std::string addLocation;
        std::string removeLocation;
        std::string setHome;

        Options();
    };   
}

#endif