#ifndef CONFIG_MAN_H
#define CONFIG_MAN_H

#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>
#include <fstream>
#include <memory>
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

    class ConfigManager {
        public:
            static const std::string CONFIG_FILENAME;
            static const std::string DEFAULT_LOC_NAME;
            static const std::string TEMP_LOC_NAME;

            ConfigManager(Options* opts, HttpRequest* request);
            // ~ConfigManager();

            const Config* getConfig();
            void loadConfig();
            void saveConfig();
            void setDays(int days);
            void setLocation(std::string locName);
            void setLocation(Location loc);
            void setHomeLocation(std::string locName);
            void removeLocation(std::string locName);
            void setRenderer(std::shared_ptr<OutputInterface> renderer);
            Location fetchLocation(HttpRequest* request, const Coordinates* coords);
            bool userAgentIsValid(std::string userAgent);

        private:
            bool configIsValid(json configData);
            void parseConfig(json configData);
            void loadFallbackConfig();
            void addLocation(json& locationData);
            bool inLocations(std::string locName);
            Location* getLocationByName(std::string locName);
            json serializeConfig();
            json locationToJson(const Location* loc);

            Config m_activeConfig;
            std::vector<Location> m_locations;
            int m_defaultDays;
            Verbosity m_defaultVerbosity;
            std::string m_homeName;
            std::string m_userAgent;
    };
}

#endif
