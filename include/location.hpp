#ifndef LOCATION_H
#define LOCATION_H

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace forecast {
    struct Coordinates {
        float latitude;
        float longitude;
    };

    class Location {
        public: 
            Location(json gridPointsData);
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
