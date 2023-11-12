#include <iostream>
#include <sstream>
#include <getopt.h>
#include <stdio.h>
#include <filesystem>
#include <stdlib.h>
#include "app.hpp"
#include "location.hpp"
#include "config_manager.hpp"
#include "forecast.hpp"

#ifdef _WIN32
    #include <Windows.h>
#elif __linux__
    #include <libgen.h>  
    #include <unistd.h>
    #include <limits.h>
#elif __APPLE__
    #include <mach-o/dyld.h>
#endif

namespace {
    /**
     * Return the path to the config.json file relative to the executable. Create the config
     * directory if it doesn't exist. If unable to create a config directory, returns an empty string.
    */
    std::string getConfigPath() {
        std::string execDir;

#ifdef _WIN32
        char buffer[MAX_PATH]
        GetModuleFileName(null, buffer, MAX_PATH);
        execDir = buffer;
        execDir = execDir.substr(0, execDir.find_last_of("\\/"));
#elif __linux__
        char buffer[PATH_MAX];
        ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
        if (len != -1) {
            buffer[len] = '\0';
            execDir = buffer;
            execDir = execDir.substr(0, execDir.find_last_of("/"));
        }
#elif __APPLE__
        char buffer[PATH_MAX];
        uint32_t size = sizeof(buffer);
        if (_NSGetExecutablePath(buffer, &size) == 0) {
            execDir = buffer;
            execDir = execDir.substr(0, execDir.find_last_of("/"));
        }
#endif

        std::string configDir = execDir + "/config/";
        if (!std::filesystem::exists(configDir)) {
            if (!std::filesystem::create_directory(configDir)) {
                return "";
            }
        }

        return execDir + "/config/config.json";
    }
}

namespace forecast {
    App::App(int argc, char* argv[]) : 
        m_request(HttpRequest()),
        m_context(OutputContext::FORECAST)
    {
        setOptions(argc, argv);
    }

    bool App::init() {
        m_request.init();
        if (m_request.getStatus() == clientStatus::ERROR) {
            std::cout << "Unable to send forecast request. Check your internet connection and try again" << std::endl;
            return false;
        }

        return true;
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
    */
    void App::run() {
        std::string configPath = getConfigPath();
        if (configPath.empty()) {
            std::cout << "Error: could not access config directory";
        }

        ConfigManager configManager = ConfigManager(&m_opts, &m_request, configPath);
        if (!configManager.configIsValid()) {
            exitWithErrors(configManager.getErrors());
        }

        const Config* config = configManager.getConfig();
        configManager.saveConfig();

        if (config->verbosity == Verbosity::HIGH) {
            showWarnings(configManager.getWarnings());
        }

        if (m_context == OutputContext::CONFIG) {
            printConfig(config->verbosity, config->days, configManager.getHomeLocation(), configManager.getUserAgent());
        } else if (m_context == OutputContext::LOCATIONS) {
            printLocations(configManager.getLocations());
        } else {
            printForecast(config);
        }
    }

    void App::setOptions(int argc, char* argv[]) {
        const int SET_HOME = 1000;
        const int SHOW_LOCATIONS = 1001;
        const int SHOW_CONFIG = 1002;

        int verbosity = 0;
        const char* const shortOpts = "hc:d:l:a:r:m:u:";
        static struct option longOpts[] = {
            // Flags
            {"verbose", no_argument, &verbosity, 2},

            // Args
            {"help", no_argument, nullptr, 'h'},
            {"coords", required_argument, nullptr, 'c'},
            {"days", required_argument, nullptr, 'd'},
            {"location", required_argument, nullptr, 'l'},
            {"add-location", required_argument, nullptr, 'a'},
            {"remove-location", required_argument, nullptr, 'r'},
            {"set-home", required_argument, nullptr, SET_HOME},
            {"mode", required_argument, nullptr, 'm'},
            {"user-agent", required_argument, nullptr, 'u'},
            {"show-locations", no_argument, nullptr, SHOW_LOCATIONS},
            {"show-config", no_argument, nullptr, SHOW_CONFIG},
            {nullptr, 0, nullptr, 0}
        };

        int arg;
        int argIdx;
        while (true) {
            arg = getopt_long(argc, argv, shortOpts, longOpts, &argIdx);
            if (arg == -1) {
                break;
            }
            switch (arg) {
                case 0:
                    // Flag set
                    break;
                case 'h':
                    help();
                    break;
                case 'c':
                    m_opts.coords = parseCoords(optarg);
                    break;
                case 'd':
                    m_opts.days = std::stoi(optarg);
                    break;
                case 'l':
                    m_opts.locationName = optarg;
                    break;
                case 'a':
                    m_opts.addLocation = optarg;
                    break;
                case 'r':
                    m_opts.removeLocation = optarg;
                    break;
                case SET_HOME:
                    m_opts.setHome = optarg;
                    break;
                case 'm':
                    m_opts.mode = getMode(optarg);
                    break;
                case 'u':
                    m_opts.userAgent = optarg;
                    break;
                case SHOW_CONFIG:
                    m_context = OutputContext::CONFIG;
                    break;
                case SHOW_LOCATIONS:
                    m_context = OutputContext::LOCATIONS;
                    break;
                case '?':
                    std::cout << "Unrecognized argument" << std::endl;
                    std::cout << "\n";
                    help();
                    break;
                default:
                    help();
                    abort();
            }
        }

        if (m_opts.coords.isValid() && !m_opts.locationName.empty()) {
            throw std::invalid_argument("Error: ambiguous location. Must provide either coordinates or location name, not both.");
        }

        if (verbosity == 1) {
            m_opts.verbosity = Verbosity::LOW;
        } else if (verbosity == 2) {
            m_opts.verbosity = Verbosity::HIGH;
        } else {
            m_opts.verbosity = Verbosity::STD;
        }
    }

    json App::getForecastData(Location* location, std::string userAgent) {
        m_request.appendHeader("User-Agent", userAgent);
        std::ostringstream url;
        url << "https://api.weather.gov/gridpoints/" 
            << location->gridId << "/"
            << location->gridX << "," << location->gridY 
            << "/forecast";

        m_request.send(url.str());
        json forecastData = m_request.getJsonResponse();

        return forecastData;
    }

    Coordinates App::parseCoords(char* optarg) {
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
                    throw std::invalid_argument("Invalid coordinate set. Must provide longitude and latitude separated by a comma.");
                }
                std::stof(coord);
            } else {
                throw std::invalid_argument("Invalid coordinates set. Longitude and Latitude must be a number");
            }

            idx++;
        }

        return coords;
    }

    OutputMode App::getMode(const char* outputMode) {
        std::unordered_map<std::string, OutputMode> modeMap = {
            {"grid", OutputMode::GRID},
            {"row", OutputMode::ROW},
        };
        std::string strMode = std::string(outputMode);
        OutputMode mode;
        if (auto lookup = modeMap.find(strMode); lookup != modeMap.end()) {
            mode = lookup->second;
        } else {
            std::cout << "Invalid mode. Must be either 'grid' or 'row'. Defaulting to grid mode." << std::endl;
            mode = OutputMode::GRID;
        }
        
        return mode;
    }

    bool App::isFloat(std::string str) {
        std::istringstream iss(str);
        float f;

        iss >> std::noskipws >> f;

        return iss.eof() && !iss.fail();
    }

    void App::printForecast(const Config* config) {
        json forecastData = getForecastData(config->location, config->userAgent);
        Forecast forecast = Forecast(forecastData, config->location);
        forecast.setRenderer(config->renderer);
        forecast.render(config->days);
    }

    void App::printConfig(Verbosity verbosity, int days, std::string homeLocation, std::string userAgent) {
        std::stringstream ss;
        ss << "Config: \n";
        ss << "------------\n";
        std::string verbosityName;
        switch (verbosity) {
            case Verbosity::LOW:
                verbosityName = "low";
                break;
            case Verbosity::STD:
                verbosityName = "standard";
                break;
            case Verbosity::HIGH:
                verbosityName = "high";
                break;
            default:
                throw std::invalid_argument("Did not recognize verbosity level");
        }

        ss << "Verbosity: " << verbosityName << '\n';
        ss << "Days: " << days << '\n';
        ss << "Home location: " << homeLocation << '\n';
        ss << "User-Agent: " << userAgent << '\n';

        std::cout << ss.str() << std::endl;
    }

    void App::printLocations(std::vector<Location>* locations) {
        std::stringstream ss;

        ss << "Locations:\n";
        for (auto loc : *locations) {
            ss << "------------\n";
            ss << formatLocation(loc);
        }

        std::cout << ss.str() << std::endl;
    }

    std::string App::formatLocation(Location loc) {
        std::stringstream ss;
        ss << "Name: " << loc.name << '\n';
        ss << "City: " << loc.city << '\n';
        ss << "State: " << loc.state << '\n';
        ss << "Latitude: " << std::setprecision(16) << loc.coords.latitude << '\n';
        ss << "Longitude: " << std::setprecision(16) << loc.coords.longitude << '\n';
        ss << "Grid ID: " << loc.gridId << '\n';
        ss << "Grid X: " << loc.gridX << '\n';
        ss << "Grid Y: " << loc.gridY << '\n';

        return ss.str();
    }

    void App::showWarnings(std::vector<ConfigError>* warnings) {
        std::stringstream ss;
        for (ConfigError warning : *warnings) {
            ss << "Warning: " << warning.msg << "\n";
        }

        std::cout << ss.str() << std::endl;
    }

    void App::exitWithErrors(std::vector<ConfigError>* errors) {
        std::stringstream ss;
        for (ConfigError error : *errors) {
            ss << "Error: " << error.msg << "\n";
        }

        std::cout << ss.str() << std::endl;
        std::exit(EXIT_FAILURE);
    }

    void App::help() {
        const char* usage = R"(Forecast CLI provides a multi-day weather forecast for anywhere in the United States

Usage:
    forecast [options] [<arguments>]

Options:
    --help, -h                       Show help
    --verbose                        Show debugging info including warnings
    --coords, -c LONGITUDE,LATITUDE  Get the forecast for a specific longitude and latitude
                                     Separate coordinates pair with a comma (no space)
    --days, -d INTEGER               The number of days you'd like a forecast for
    --location, -l NAME              Get the forecast for a saved location
    --add-location, -a NAME          When combined with --coords, save the location by
                                     the name provided for later use
    --remove-location, -r NAME       Remove a saved location
    --set-home, -s NAME              Set the default/home location
    --mode, -m row|grid              Choose the output mode: row or grid [default: grid]
    --user-agent, -u EMAIL           Set the email address to use when requesting forecasts
                                     from the NOAA weather API
    --show--locations                Output a list of all saved locations
    --show-config                    Output the current configuration defaults for verbosity,
                                     days, home-location, and User-Agent
        )";
        std::cout << usage << std::endl;
        exit(1);
    }
}
