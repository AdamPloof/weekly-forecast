#ifndef PRINTER_H
#define PRINTER_H

#include <string>
#include <iostream>
#include <sstream>
#include <nlohmann/json.hpp>
#include "forecast.hpp"

/**
 * Process for printing a forecast in a grid format:
 * 
 * - Print Header section:
 *   - App title
 *   - Dates
 *   - Location
 * - Print days
 *   - Each day is two* periods (day & night), with each period having:
 *     - Period name (e.g. Tonight)
 *     - short description
 *     - short details (high/low, precip %)
 *     - long description
 * 
 * note: the first period might be night in which case the very first grid cell
 *       would be blank. The last period will always be night.
 * 
 * Other methods needed:
 * - top/bottom border
 * - line break divider
 * 
 * Other things to think about:
 * - left/right border & horizontal border between periods of the same day will
 *   have to be baked into the line.
 * 
 * Each line has a fixed length. Or more accurately, each pseudo line for each period
 * has a fixed length and the line length is 2x that.
 * 
 * 
*/

namespace forecast {
    class Printer {
        public:
            Printer();
            void print(Forecast forecast);

        private:
            void header();
            void title();
            void period();
            void border();
            void lineBreak();

            std::ostringstream output;
    };
}

#endif
