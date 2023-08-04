#include "grid_printer.hpp"

namespace forecast {
    const int GridPrinter::lineWidth = 129;

    GridPrinter::GridPrinter() {}

    void GridPrinter::render(Forecast* forecast, int days) {
        // Placeholders using member vars

        if (days > 0) {

        }


        xBorderMain();

        // Remember, there's two periods per day. But it's possible the first period is night time.
        int periodNum = 1;
        int maxPeriods = days * 2;
        const std::vector<json>* periods = forecast->getPeriods();
        for (size_t i = 0; i < periods->size(); i += 2) {
            const json& period = periods[i];
            if (period["isDaytime"] == true) {
                makeFullDay(periods[i], periods[i + 1]);
            } else {
                makeFullDay(periods[i]);
            }
        }

        xBorderSecondary();
    }

    const std::ostringstream& GridPrinter::getOutput() const {
        return m_output;
    }

    // *---------------*---------------*---------------*---------------*
    void GridPrinter::xBorderMain() {
        for (int i = 0; i < 8; i++) {
            m_output << "*---------------";
        }

        m_output << "*\n";
    }

    // | ... -- ... -- = ... -- ... -- = ... -- ... -- = ... -- ... -- |
    void GridPrinter::xBorderSecondary() {
        m_output << "|";
        for (int i = 0; i < 16; i++) {
            m_output << " ... --";

            if ((i + 1) % 8 == 0) {
                m_output << " |";
            } else if ((i + 1) % 2 == 0) {
                m_output << " =";
            }
        }

        m_output << "\n";
    }

    void GridPrinter::makeFullDay(const json& day, const json& night) {
        // Name
        std::string nameD = padLine(day["name"]);
        std::string nameN = padLine(night["name"]);
        m_output << borderY(true) << nameD << borderY(true) << nameN << borderY();
    }

    void GridPrinter::makeFullDay(const json& night) {
        borderY();
    }

    std::string GridPrinter::borderY(bool trailingSpace) {
        if (trailingSpace == true) {
            return "| ";
        } else {
            return "|";
        }
    }

    // Note technically a full line, but the line of a single period. Basically,
    // half a line.
    std::string GridPrinter::padLine(std::string line) {
        int padAmt = ((GridPrinter::lineWidth - 1) / 2) - (3 + line.length());
        std::string padding = std::string(padAmt, ' ');

        return line.append(padding);
    }
}
