#include <sstream>
#include "config_manager.hpp"
#include "utils.hpp"
#include "grid_printer.hpp"
#include "row_printer.hpp"

using json = nlohmann::json;

namespace forecast {
    const std::string ConfigManager::CONFIG_FILENAME = "config.json";
    const std::string ConfigManager::DEFAULT_LOC_NAME = "app_default_x2340";
    const std::string ConfigManager::TEMP_LOC_NAME = "temp_abc234";

    Config::Config() : 
        location(nullptr),
        days(7),
        verbosity(Verbosity::STD),
        renderer(nullptr),
        userAgent("") {};

    ConfigManager::ConfigManager(Options* opts, HttpRequest* request) {
        loadConfig();

        if (!opts->setHome.empty()) {
            setHomeLocation(opts->setHome);

            // Use the new home location as the location is one is not explicitly provided.
            if (opts->locationName.empty()) {
                setLocation(opts->setHome);
            }
        }

        if (opts->mode == OutputMode::GRID) {
            std::shared_ptr<OutputInterface> renderer = std::make_shared<GridPrinter>();
            setRenderer(renderer);
        } else {
            std::shared_ptr<OutputInterface> renderer = std::make_shared<RowPrinter>();
            setRenderer(renderer);
        }

        if (!opts->userAgent.empty()) {
            m_userAgent = opts->userAgent;
            m_activeConfig.userAgent = opts->userAgent;
        } else if (m_userAgent.empty()) {
            // TODO: come up with a better prompt and validate User-Agent is valid
            std::string userAgent;
            std::cout << "This app uses the NOAA weather service which requires a User-Agent.\n";
            std::cout << "Please enter an email used to identify who is requesting weather forecasts:" << std::endl;
            std::cin >> userAgent;
            m_userAgent = userAgent;
            m_activeConfig.userAgent = m_userAgent;
        }

        // TODO: if both coordinates and a name are provided warn that the name is ignored and coords are used.
        if (opts->coords.isValid()) {
            // Fetch location for coordinates.
            // Coordinates coords = {44.389243, -72.887906};
            Location location = fetchLocation(request, &opts->coords);
            if (!opts->addLocation.empty()) {
                // TODO: warn user that they need to provide coordinates if they try to add a loc without them.
                location.name = opts->addLocation;
            }

            setLocation(location);
        } else if (!opts->locationName.empty()) {
            setLocation(opts->locationName);
        }

        if (m_activeConfig.verbosity != opts->verbosity) {
            m_activeConfig.verbosity = opts->verbosity;
        }

        if (m_activeConfig.days != opts->days) {
            setDays(opts->days);
        }
    }

    const Config* ConfigManager::getConfig() {
        return &m_activeConfig;
    }

    void ConfigManager::loadConfig() {
        std::ifstream handle;
        handle.open(ConfigManager::CONFIG_FILENAME);
        if (handle.is_open()) {
            json configData = json::parse(handle);
            parseConfig(configData);
            handle.close();
        } else {
            loadFallbackConfig();
        }

        // Set the active config to defaults
        m_activeConfig.days = m_defaultDays;
        m_activeConfig.verbosity = m_defaultVerbosity;
        m_activeConfig.location = getLocationByName(m_homeName);
        m_activeConfig.userAgent = m_userAgent;
    }

    void ConfigManager::saveConfig() {
        std::ofstream configFile = std::ofstream(ConfigManager::CONFIG_FILENAME, std::ios::trunc);
        configFile << std::setw(4) << serializeConfig() << std::endl;
        configFile.close();
    }

    void ConfigManager::setDays(int days) {
        if (days <= 7 && days > 0) {
            m_activeConfig.days = days;
        } else {
            // Set an error flag for invalid number of days.
        }
    }

    void ConfigManager::setLocation(std::string locName) {
        if (inLocations(locName)) {
            m_activeConfig.location = getLocationByName(locName);
        } else {
            // Set an error flag.
        }
    }

    void ConfigManager::setLocation(Location loc) {
        m_locations.push_back(loc);
        m_activeConfig.location = &m_locations.back();
    }

    bool ConfigManager::configIsValid(json configData) {
        bool isValid = true;
        if (!configData.contains("locations")) {
            isValid = false;
        }
        if (!configData.contains("defaultDays")) {
            isValid = false;
        }
        if (!configData.contains("defaultVerbosity")) {
            isValid = false;
        }
        if (!configData.contains("homeLocation")) {
            isValid = false;
        }

        return isValid;                
    }

    void ConfigManager::parseConfig(json configData) {
        if (!configIsValid(configData)) {
            // TODO: Should probably signal that something is wrong with the config file.
            // TODO: what if the user provides *some* options on the first run. We want
            // don't want to throw the baby out with the bathwater here.
            loadFallbackConfig();
            return;
        }

        for (json& loc : configData["locations"]) {
            addLocation(loc);
        }

        // make sure there's at least one location
        if (m_locations.size() == 0) {
            Location location = Location(ConfigManager::DEFAULT_LOC_NAME);
            m_locations.push_back(location);                    
        }

        int configVerbosity = configData["defaultVerbosity"];
        switch (configVerbosity) {
            case 1:
                m_defaultVerbosity = Verbosity::LOW;
                break;
            case 2:
                m_defaultVerbosity = Verbosity::HIGH;
                break;
            default:
                m_defaultVerbosity = Verbosity::STD;
        }

        // Note: make sure to set the homeLocation *after* adding all locations!
        m_homeName = configData["homeLocation"];
        m_defaultDays = configData["defaultDays"];

        if (configData.contains("userAgent")) {
            m_userAgent = configData["userAgent"];
        }
    }

    // Sets default config options in case there's an invalid config file
    void ConfigManager::loadFallbackConfig() {
        if (m_locations.size() == 0) {
            Location location = Location(ConfigManager::DEFAULT_LOC_NAME);
            m_locations.push_back(location);
        }

        m_homeName = m_locations.front().name;
        m_defaultDays = 7;
        m_defaultVerbosity = Verbosity::STD;
        m_userAgent = "";
    }

    void ConfigManager::addLocation(json& locationData) {
        std::string locName = locationData["name"];
        Location location = Location(locName);

        location.gridId = locationData["gridId"];
        location.gridX = locationData["gridX"];
        location.gridY = locationData["gridY"];
        location.city = locationData["city"];
        location.state = locationData["state"];

        Coordinates coords = {locationData["coords"]["latitude"], locationData["coords"]["longitude"]};
        location.coords = coords;

        m_locations.push_back(location);
    }

    // TODO: return error if locName not in locations
    void ConfigManager::setHomeLocation(std::string locName) {
        locName = strToLower(locName);
        for (Location& loc : m_locations) {
            if (strToLower(loc.name) == locName) {
                m_homeName = locName;
                break;
            }
        }

        // return // some kind of error here;
    }

    // TODO: return error if locName not in locations
    void ConfigManager::removeLocation(std::string locName) {
        if (locName == ConfigManager::DEFAULT_LOC_NAME) {
            // TODO: Warn the user that they shouldn't try this.
            return;
        }

        if (locName == m_activeConfig.location->name) {
            // Make sure we don't remove the active location without swapping it for something else.
            Location location = Location(ConfigManager::DEFAULT_LOC_NAME);
            m_locations.push_back(location);
            m_activeConfig.location = &m_locations.back();
            // TODO: Warn the user that they just removed the active location.
        }

        for (std::vector<int>::size_type i = 0; i < m_locations.size(); i++) {
            if (m_locations[i].name == locName) {
                m_locations.erase(m_locations.begin() + i);
                break;
            }
        }

        // return // some kind of error here;
    }

    // TODO: should probably normalize names: remove whitespace, lower case, etc.
    bool ConfigManager::inLocations(std::string locName) {
        bool found = false;
        locName = strToLower(locName);
        for (Location& loc : m_locations) {
            if (strToLower(loc.name) == locName) {
                found = true;
                break;
            }
        }

        return found;
    }

    json ConfigManager::serializeConfig() {
        json config;
        config["defaultDays"] = m_defaultDays;
        config["defaultVerbosity"] = m_defaultVerbosity;
        config["homeLocation"] = m_homeName;
        config["userAgent"] = m_userAgent;

        json locations = json::array();
        for (const Location& loc : m_locations) {
            if (loc.name != ConfigManager::DEFAULT_LOC_NAME && loc.name != ConfigManager::TEMP_LOC_NAME) {
                locations.push_back(locationToJson(&loc));
            }
        }

        config["locations"] = locations;

        return config;
    }

    json ConfigManager::locationToJson(const Location* loc) {
        json j;
        j["name"] = loc->name;
        j["gridId"] = loc->gridId;
        j["gridX"] = loc->gridX;
        j["gridY"] = loc->gridY;
        j["city"] = loc->city;
        j["state"] = loc->state;
        j["coords"]["latitude"] = loc->coords.latitude;
        j["coords"]["longitude"] = loc->coords.longitude;

        return j;
    }

    void ConfigManager::setRenderer(std::shared_ptr<OutputInterface> renderer) {
        m_activeConfig.renderer = renderer;
    }

    Location ConfigManager::fetchLocation(HttpRequest* request, const Coordinates* coords) {
        std::ostringstream url;
        url << "https://api.weather.gov/points/" 
            << coords->latitude << "," << coords->longitude;

        request->send(url.str());

        json gridPointsData = request->getJsonResponse();
        Location location(gridPointsData, ConfigManager::TEMP_LOC_NAME);

        return location;
    }

    // TODO: check that the userAgent is an email.
    bool ConfigManager::userAgentIsValid(std::string userAgent) {
        if (userAgent.length() > 2) {
            return true;
        } else {
            return true;
        }
    }

    // If no home location is set, then just return the first location in the list.
    // By this point, there should always be at least one in the locations list.
    Location* ConfigManager::getLocationByName(std::string locName) {
        locName = strToLower(locName);
        for (Location& loc : m_locations) {
            if (strToLower(loc.name) == locName) {
                return &loc;
            }
        }

        return &m_locations.front();
    }
}
