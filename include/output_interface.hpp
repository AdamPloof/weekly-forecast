#ifndef OUTPUT_INTERFACE_H
#define OUTPUT_INTERFACE_H

#include "forecast.hpp"

namespace forecast {
    // Defining a generic interface here in case one day I want to have multiple styles of output.
    class OutputInterface {
        public:
            virtual void render(Forecast* forecast, int days) = 0;
    };
}

#endif
