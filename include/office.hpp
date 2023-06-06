#ifndef OFFICE_H
#define OFFICE_H

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace forecast {
    struct Coordinates {
        float latitude;
        float longitude;
    };

    class Office {
        public: 
            Office(json gridPointsData);
            std::string gridId;
            int gridX;
            int gridY;
            Coordinates coords;
            std::string city;
            std::string state;
    };
}


#endif
