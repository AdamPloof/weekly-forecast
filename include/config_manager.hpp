#ifndef CONFIG_MAN_H
#define CONFIG_MAN_H

#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>
#include <fstream>
#include <memory>
#include <regex>
#include "options.hpp"
#include "http_request.hpp"
#include "location.hpp"
#include "options.hpp"
#include "output_interface.hpp"

using namespace forecast::http_request;

/**
 * Reads and writes config from/to json file.
 * 
 * Config file structure:
 * {
 *   "locations": [
 *     {<Location obj 1>},
 * 	   {<Location obj 2>},
 * 	   ...
 * 	 ],
 * 	 "defaultDays": <int>,
 * 	 "defaultVerbosity": <int : 0 = STD, 1 = LOW, 2 = HIGH>,
 *   "homeLocation": <string : name of location>,
 *   "user_agent": <std::string : user's email>
 * }
 * 
 * Some assumptions/guarantees about the config file:
 *   - If any of those keys are missing (or the file is empty), the config file is
 *     considered empty and will not be loaded.
 *   - The locations array will always contain at least one location.
 *   - The home location will always be set, if there's only one location in the location array
 *     then it will use that one's name.
 * 
*/
namespace forecast {
    struct Config {
        Location* location;
        int days;
        Verbosity verbosity;
        std::shared_ptr<OutputInterface> renderer;
        std::string userAgent;

        Config();
    };

    enum class ConfigErrorLvl {
        WARNING,
        ERROR
    };

    struct ConfigError {
        std::string field;
        std::string msg;
        ConfigErrorLvl lvl;
    };

    class ConfigManager {
        public:
            static const std::string CONFIG_FILENAME;
            static const std::string DEFAULT_LOC_NAME;
            static const std::string TEMP_LOC_NAME;
            static const std::regex VALID_EMAIL_REGEX;

            ConfigManager(Options* opts, HttpRequest* request, std::string configPath);
            // ~ConfigManager();

            const Config* getConfig();
            void loadConfig();
            void saveConfig();
            void setOptions(Options* opts, HttpRequest* request);
            void setDays(int days);
            void setLocation(std::string locName);
            void setLocation(Location loc);
            std::string getHomeLocation();
            void setHomeLocation(std::string locName);
            void removeLocation(std::string locName);
            std::string getUserAgent();
            void setRenderer(std::shared_ptr<OutputInterface> renderer);
            Location fetchLocation(HttpRequest* request, const Coordinates* coords, std::string userAgent);
            bool userAgentIsValid(std::string userAgent);
            bool configIsValid();
            std::vector<Location>* getLocations();
            std::vector<ConfigError>* getErrors();
            std::vector<ConfigError>* getWarnings();

        private:
            bool dataIsValid(json configData);
            void parseConfig(json configData);
            void loadFallbackConfig();
            void addLocation(json& locationData);
            bool inLocations(std::string locName);            
            Location* getLocationByName(std::string locName);
            json serializeConfig();
            json locationToJson(const Location* loc);
            std::string promptForUserAgent(bool isRetry = false);
            void addError(std::string field, std::string msg, ConfigErrorLvl lvl);

            std::string m_configPath;
            Config m_activeConfig;
            std::vector<Location> m_locations;
            int m_defaultDays;
            Verbosity m_defaultVerbosity;
            std::string m_homeName;
            std::string m_userAgent;
            std::vector<ConfigError> m_errors;
            std::vector<ConfigError> m_warnings;
    };
}

#endif
