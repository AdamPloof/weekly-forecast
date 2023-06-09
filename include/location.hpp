#ifndef LOCATION_H
#define LOCATION_H

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace forecast {
    struct Coordinates {
        float latitude;
        float longitude;

        bool isValid();
    };

    class Location {
        public:
            Location(json gridPointsData, std::string locName);
            Location(std::string locName);
            std::string name;
            std::string gridId;
            int gridX;
            int gridY;
            Coordinates coords;
            std::string city;
            std::string state;
    };
}

#endif
