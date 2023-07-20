#include <iostream>
#include <sstream>
#include <string>

#include "grid_printer.hpp"

#define ASSERT_TRUE(exp, msg) if (!(exp)) { \
    std::cout << __FUNCTION__ << " failed on line " << __LINE__ \
    << ": " << msg << std::endl; \
}

using namespace forecast;

namespace test::grid_printer {
    class TestGridPrinter {
        public:
            void testLineWidthIsFixed(GridPrinter* printer, int width) {
                const std::ostringstream& output = printer->getOutput();
                std::istringstream input;
                input.str(output.str());

                for (std::string line; std::getline(input, line, '\n'); ) {
                    ASSERT_TRUE((line.length() == m_lineWidth), "Line width is not 129");
                }
            }
        private:
            size_t m_lineWidth = 129;
    };
}


int main(int argc, char* argv[]) {
    GridPrinter printer;

    return 0;
}
