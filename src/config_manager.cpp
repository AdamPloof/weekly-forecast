#include "config_manager.hpp"

using json = nlohmann::json;

namespace forecast {
    Config::Config() :  location(nullptr), days(7), verbosity(Verbosity::STD) {};

    ConfigManager::ConfigManager() {
        Config* m_activeConfig = new Config;
        m_homeLocation = nullptr;
    }

    Config* ConfigManager::getActiveConfig() {
        return m_activeConfig;
    }

    void ConfigManager::loadConfig() {
        std::ifstream handle;
        handle.open("config.json");
        if (handle.is_open()) {
            json configData = json::parse(handle);
            parseConfig(configData);
            handle.close();
        } else {
            loadFallbackConfig();
        }
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
            loadFallbackConfig();
            return;
        }

        for (json& loc : configData["locations"]) {
            addLocation(loc);
        }

        // make sure there's at least one location
        if (m_locations.size() == 0) {
            Location location = Location();
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
        m_homeLocation = getLocationByName(configData["homeLocation"]);
        m_defaultDays = configData["defaultDays"];
    }

    // Sets default config options in case there's an invalid config file
    void ConfigManager::loadFallbackConfig() {
        if (m_locations.size() == 0) {
            Location location = Location();
            m_locations.push_back(location);
        }

        m_homeLocation = &m_locations.front();
        m_defaultDays = 7;
        m_defaultVerbosity = Verbosity::STD;
    }

    void ConfigManager::addLocation(json& locationData) {
        Location location;
        location.name = locationData["name"];
        location.gridId = locationData["gridId"];
        location.gridX = locationData["gridX"];
        location.gridY = locationData["gridY"];
        location.city = locationData["city"];
        location.state = locationData["state"];

        Coordinates coords = {locationData["coords"]["latitude"], locationData["coords"]["longitude"]};
        location.coords = coords;

        m_locations.push_back(location);
    }

    Location* ConfigManager::getLocationByName(std::string locName) {
        for (Location& loc : m_locations) {
            if (loc.name == locName) {
                return &loc;
            }
        }

        return &m_locations.front();
    }

    ConfigManager::~ConfigManager() {
        delete m_activeConfig;
    }
}
