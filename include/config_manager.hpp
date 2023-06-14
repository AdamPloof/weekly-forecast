#ifndef CONFIG_MAN_H
#define CONFIG_MAN_H

#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>
#include "location.hpp"
#include "options.hpp"

namespace forecast {
    struct Config {
        Location location;
        int days;
        Verbosity verbosity;
    };

    class ConfigManager {
        public:
            ConfigManager();
            ~ConfigManager();
        private:
            Config m_activeConfig;
            std::vector<Location> m_locations;
            int m_defaultDays;
            Verbosity m_defaultVerbosity;
            Location m_homeLocation;
    };
}

#endif
