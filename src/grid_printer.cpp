#include "grid_printer.hpp"

namespace forecast {
    const int GridPrinter::lineWidth = 129;

    GridPrinter::GridPrinter() {}

    void GridPrinter::render(Forecast* forecast, int days) {
        xBorderMain();

        // Remember, there's two periods per day. But it's possible the first period is night time.
        // Also note that we advance either by 1 period or two depending on whether the first period is
        // day or night
        int dayNum = 1;
        const std::vector<json> periods = *forecast->getPeriods();
        for (size_t i = 0; i < periods.size(); ) {
            if (dayNum > days) {
                break;
            }

            const json& period = periods[i];
            std::string temp = period.dump();
            if (period["isDaytime"] == true) {
                makeDay(periods[i], periods[i + 1]);
                i += 2;
            } else {
                makeDay(periods[i]);
                i += 1;
            }

            dayNum++;
        }

        std::cout << m_output.str() << std::endl;
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

    // Note: makeDay refers to a *full* day, i.e. a day and a night period.
    void GridPrinter::makeDay(const json& day, const json& night) {
        // Name
        std::string nameD = padLine(day["name"]);
        std::string nameN = padLine(night["name"]);
        m_output << borderY(true) << nameD << borderY(true) << nameN << borderY();
        m_output << '\n';
        xBorderSecondary();
    }

    void GridPrinter::makeDay(const json& night) {
        // Name
        std::string nameD = padLine(" ");
        std::string nameN = padLine(night["name"]);
        m_output << borderY(true) << nameD << borderY(true) << nameN << borderY();
        m_output << '\n';
        xBorderSecondary();
    }

    // Reminder, each forecast section should have the same number of lines for day and night.
    std::string GridPrinter::forecastSection(const std::string dayData, const std::string nightData) {
        constexpr const int maxWidth = ((lineWidth - 1) / 2) - 3;
        std::vector<std::string> dayLines;
        std::istringstream iss = std::istringstream(dayData);
        std::string output;
        while (iss >> output) {
            if (output.length() >= maxWidth) {
                break;
            }
        }

        std::vector<std::string> nightLines;




    }

    std::string GridPrinter::borderY(bool trailingSpace) {
        if (trailingSpace == true) {
            return "| ";
        } else {
            return "|";
        }
    }

    // Not technically a full line, but the line of a single period.
    // Basically, half a line.
    std::string GridPrinter::padLine(const std::string line) {
        int padAmt = ((GridPrinter::lineWidth - 1) / 2) - (2 + line.length());
        std::string padding = std::string(padAmt, ' ');
        std::string padded = line + padding;

        return padded;
    }
}
