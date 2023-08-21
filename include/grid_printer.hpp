#ifndef GRID_PRINTER_H
#define GRID_PRINTER_H

#include <string>
#include <iostream>
#include <sstream>
#include <nlohmann/json.hpp>
#include "forecast.hpp"
#include "line_formatter.hpp"
#include "output_interface.hpp"

using namespace forecast::utils;

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
*/
namespace forecast {
    class GridPrinter : public OutputInterface {
        public:
            static const int LINE_WIDTH;

            GridPrinter();
            ~GridPrinter() override;
            void render(Forecast* forecast, int days) override;
            const std::ostringstream& getOutput() const;

        private:
            void xBorderMain();
            void xBorderSecondary();
            std::string borderY(bool trailingSpace = false);
            void header(const std::string startDate, const std::string endDate, const Location* loc);
            void makeDay(const json& day, const json& night);
            void makeDay(const json& night);
            std::string periodSummary(const json& period);
            std::string forecastSection(const std::string dayData, const std::string nightData);
            void lineBreak();

            LineFormatter m_formatter;
            std::ostringstream m_output;
    };
}

#endif
