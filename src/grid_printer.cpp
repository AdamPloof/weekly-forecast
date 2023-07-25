#include "grid_printer.hpp"

namespace forecast {
    GridPrinter::GridPrinter() {}

    void GridPrinter::render(Forecast* forecast, int days) {
        std::cout << days << std::endl;
        std::cout << forecast->getEndDate() << std::endl;
    }

    const std::ostringstream& GridPrinter::getOutput() {
        xBorderMain();
        xBorderSecondary();

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

            m_output << "\n";
        }

    }
}
