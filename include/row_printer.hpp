#ifndef ROW_PRINTER_H
#define ROW_PRINTER_H

#include <sstream>
#include <string>
#include <iostream>
#include <nlohmann/json.hpp>
#include "output_interface.hpp"
#include "forecast.hpp"

namespace forecast {
    class RowPrinter : public OutputInterface {
        public:
            RowPrinter();
            ~RowPrinter() override;
            void render(Forecast* forecast, int days) override; 

        private:
            void makeHeader(const std::string startDate, const std::string endDate, const Location* loc);
            void makePeriod(const json& period);
            std::string makeTitle();
            std::string makeDivider(bool isLong = false);

            std::ostringstream output;
    };
}

#endif