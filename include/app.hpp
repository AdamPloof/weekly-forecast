#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>
#include <nlohmann/json.hpp>
#include "http_request.hpp"
#include "options.hpp"
#include "location.hpp"

using json = nlohmann::json;
using namespace forecast::http_request;

struct Coordinates;

namespace forecast {
    class App {
        public:
            App(int argc, char* argv[]);
            bool init();
            void run();
        private:
            void setOptions(int argc, char* argv[]);
            json getForecastData(Location*, std::string);
            Coordinates parseCoords(char* optArg);
            OutputMode getMode(const char* outputMode);
            bool isFloat(std::string str);
            void help();

            HttpRequest m_request;
            Options m_opts;
    };
}

#endif