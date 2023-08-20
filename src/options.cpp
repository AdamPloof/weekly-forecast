#include <options.hpp>

namespace forecast {
    Options::Options() : 
        coords {0, 0},
        verbosity(Verbosity::STD),
        days(7),
        mode(OutputMode::GRID) {};
}
