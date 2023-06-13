#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "http_request.hpp"
#include "options.hpp"
#include "forecast.hpp"
#include "office.hpp"

using namespace forecast;
using namespace forecast::http_request;

namespace {
    void help() {
        std::cout << "Help func TODO.";

        exit(1);
    }

    bool isFloat(std::string str) {
        std::istringstream iss(str);
        float f;

        iss >> std::noskipws >> f;

        return iss.eof() && !iss.fail();
    }

    Coordinates parseCoords(char* optarg) {
        std::stringstream ss;
        ss << optarg;
        std::string coord;
        Coordinates coords;

        int idx = 0;
        while (std::getline(ss, coord, ',')) {
            if (isFloat(coord)) {
                if (idx == 0) {
                    coords.latitude = stof(coord);
                } else if (idx == 1) {
                    coords.longitude = stof(coord);
                } else {
                    // TODO: Too many coordinates. Throw invalid arguments error.
                }
                std::stof(coord);
            } else {
                // TODO: Throw some kind of an invalid argument error.
            }

            idx++;
        }

        return coords;
    }

    Options getOptions(int argc, char* argv[]) {
        int verbosity = 0;
        const char* const shortOpts = "c:d:vsl:a:r:h:";
        static struct option longOpts[] = {
            // Flags
            {"verbose", no_argument, &verbosity, 2},
            {"short", no_argument, &verbosity, 1},

            // Args
            {"coords", required_argument, nullptr, 'c'},
            {"days", required_argument, nullptr, 'd'},
            {"location", required_argument, nullptr, 'l'},
            {"add-location", required_argument, nullptr, 'a'},
            {"remove-location", required_argument, nullptr, 'r'},
            {"set-home", required_argument, nullptr, 'h'},
            {nullptr, 0, nullptr, 0}
        };

        int arg;
        int argIdx;
        Options opts;
        while (true) {
            arg = getopt_long(argc, argv, shortOpts, longOpts, &argIdx);
            if (arg == -1) {
                break;
            }
            switch (arg) {
                case 0:
                    // Flag set
                    break;
                case 'c':
                    opts.coords = parseCoords(optarg);
                    break;
                case 'd':
                    opts.days = std::stoi(optarg);
                    break;
                case 'l':
                    opts.location = optarg;
                    break;
                case 'a':
                    opts.addLocation = optarg;
                    break;
                case 'r':
                    opts.removeLocation = optarg;
                    break;
                case 'h':
                    opts.setHome = optarg;
                    break;
                case '?':
                    // Note that getopt_long already prints an error msg when this happens
                    // just putting something here as a placeholder in case we want to handle this
                    // in some special way.
                    // Maybe print help by default?
                    std::cout << "Did not recognize argument: " << arg << std::endl;
                    break;
                default:
                    abort();
            }
        }

        if (verbosity == 1) {
            opts.verbosityLvl = Verbosity::LOW;
        } else if (verbosity == 2) {
            opts.verbosityLvl = Verbosity::HIGH;
        } else {
            opts.verbosityLvl = Verbosity::STD;
        }

        return opts;
    }
}

json getForecastData(HttpRequest* request, Office* office) {
    std::ostringstream url;
    url << "https://api.weather.gov/gridpoints/" 
        << office->gridId << "/"
         << office->gridX << "," << office->gridY 
         << "/forecast";

    request->send(url.str());
    json forecastData = request->getJsonResponse();

    return forecastData;
}

Office getOffice(HttpRequest* request, const Coordinates* coords){
    std::ostringstream url;
    url << "https://api.weather.gov/points/" 
         << coords->latitude << "," << coords->longitude;

    request->send(url.str());

    json gridPointsData = request->getJsonResponse();
    Office office(gridPointsData);

    return office;
}

// TODO: handle 404s and other bad requests
int main(int argc, char* argv[]) {
    Options opts = getOptions(argc, argv);

    // Get the grid points for the zip/address provided.

    // Fetch the forecast for the grid points
    HttpRequest request = HttpRequest();
    request.init();

    if (request.getStatus() == clientStatus::ERROR) {
        // log error
    }
    request.appendHeader("User-Agent", "adamploof@hotmail.com");

    // Coordinates coords = {44.389243, -72.887906};
    Office office = getOffice(&request, &opts.coords);

    json forecastData = getForecastData(&request, &office);
    Forecast forecast = Forecast(forecastData, office);
    forecast.printForecast();

    return 0;
}
