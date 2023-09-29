#include <iostream>
#include <stdexcept>
#include "app.hpp"

using namespace forecast;

int main(int argc, char* argv[]) {
    App app = App(argc, argv);
    try {
        app.init();
        app.run();
    } catch (std::exception const& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    return 0;
}
