#include "grid_printer.hpp"

namespace forecast {
    GridPrinter::GridPrinter() {}

    void GridPrinter::render(Forecast* forecast, int days) {
        // Placeholders using member vars
        if (forecast->getEndDate().empty()) {

        }

        if (days > 0) {

        }


        xBorderMain();
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
}
