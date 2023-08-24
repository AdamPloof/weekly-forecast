#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unordered_map>
#include <memory>
#include "http_request.hpp"
#include "options.hpp"
#include "forecast.hpp"
#include "location.hpp"
#include "config_manager.hpp"
#include "output_interface.hpp"
#include "row_printer.hpp"
#include "grid_printer.hpp"

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

    OutputMode getMode(const char* outputMode) {
        std::unordered_map<std::string, OutputMode> modeMap = {
            {"grid", OutputMode::GRID},
            {"row", OutputMode::ROW},
        };
        std::string strMode = std::string(outputMode);
        OutputMode mode;
        if (auto lookup = modeMap.find(strMode); lookup != modeMap.end()) {
            mode = lookup->second;
        } else {
            // TODO: set error flag for unreckognized mode? Default to grid for now.
            mode = OutputMode::GRID;
        }
        
        return mode;
    }

    Options getOptions(int argc, char* argv[]) {
        int verbosity = 0;
        const char* const shortOpts = "c:d:vsl:a:r:h:m:u:";
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
            {"mode", required_argument, nullptr, 'm'},
            {"user-agent", required_argument, nullptr, 'u'},
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
                    opts.locationName = optarg;
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
                case 'm':
                    opts.mode = getMode(optarg);
                    break;
                case 'u':
                    opts.userAgent = optarg;
                    break;
                case '?':
                    // Note that getopt_long already prints an error msg when this happens
                    // just putting something here as a placeholder in case we want to handle this
                    // in some special way.
                    // Maybe print help by default?
                    std::cout << "Did not recognize argument: " << arg << std::endl;
                    break;
                default:
                    help();
                    abort();
            }
        }

        if (opts.coords.isValid() && !opts.locationName.empty()) {
            // TODO: error: must provide either coordinates or location name, not both.
        } else if (!opts.coords.isValid() && opts.locationName.empty()) {
            // TODO: error: must provide either coordinates or location name, none set.
        }

        if (verbosity == 1) {
            opts.verbosity = Verbosity::LOW;
        } else if (verbosity == 2) {
            opts.verbosity = Verbosity::HIGH;
        } else {
            opts.verbosity = Verbosity::STD;
        }

        return opts;
    }

    json getForecastData(HttpRequest* request, Location* location, std::string userAgent) {
        request->appendHeader("User-Agent", userAgent);
        std::ostringstream url;
        url << "https://api.weather.gov/gridpoints/" 
            << location->gridId << "/"
            << location->gridX << "," << location->gridY 
            << "/forecast";

        request->send(url.str());
        json forecastData = request->getJsonResponse();

        return forecastData;
    }
}

/**
 * Load default config
 * Get command line options
 * Check if command line options alter config
 * Update config if needed
 * Override default config with any command line options provided.
 * 
 * Note: if user provides coordinates, then use that for the location.
 * If coordinates are provided, then prompt the user to see if they would like
 * to save the location for future use. Maybe suggest a name based on the city or
 * something like that.
 * 
 * TODO: handle 404s and other bad requests
 *  
*/
int main(int argc, char* argv[]) {
    HttpRequest request = HttpRequest();
    request.init();
    if (request.getStatus() == clientStatus::ERROR) {
        // log error
    }

    Options opts = getOptions(argc, argv);
    ConfigManager configManager = ConfigManager(&opts, &request);
    const Config* config = configManager.getConfig();
    configManager.saveConfig();

    json forecastData = getForecastData(&request, config->location, config->userAgent);
    Forecast forecast = Forecast(forecastData, config->location);
    forecast.setRenderer(config->renderer);
    forecast.render(config->days);

    return 0;
}
