#include "config_manager.hpp"

using json = nlohmann::json;

namespace forecast {
    Config::Config() :  location(nullptr), days(7), verbosity(Verbosity::STD) {};

    ConfigManager::ConfigManager() {
        Config m_activeConfig;
        m_homeLocation = nullptr;
    }

    const Config* ConfigManager::getConfig() {
        return &m_activeConfig;
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

        // Set the active config to defaults
        m_activeConfig.days = m_defaultDays;
        m_activeConfig.verbosity = m_defaultVerbosity;
        m_activeConfig.location = m_homeLocation;
    }

    void ConfigManager::saveConfig() {
        std::ofstream configFile = std::ofstream("config.json", std::ios::trunc);
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

    // TODO: indicate that the location is temporary and shouldn't be saved with config file
    void ConfigManager::setLocation(Location loc) {
        m_locations.push_back(loc);
        m_activeConfig.location = &m_locations.back();
    }

    void ConfigManager::setVerbosity(Verbosity lvl) {
        m_activeConfig.verbosity = lvl;
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

    // TODO: should probably normalize names: remove whitespace, lower case, etc.
    bool ConfigManager::inLocations(std::string locName) {
        bool found = false;
        for (Location& loc : m_locations) {
            if (loc.name == locName) {
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
        config["homeLocation"] = locationToJson(m_homeLocation);

        json locations;
        for (const Location& loc : m_locations) {
            locations.push_back(locationToJson(&loc));
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

    Location* ConfigManager::getLocationByName(std::string locName) {
        for (Location& loc : m_locations) {
            if (loc.name == locName) {
                return &loc;
            }
        }

        return &m_locations.front();
    }
}
