#include "location.hpp"
#include <iostream>

namespace forecast {
    bool Coordinates::isValid() {
        return latitude != 0 && longitude != 0;
    }

    Location::Location(json gridPointsData) : name("") {
        gridId = gridPointsData["properties"]["gridId"];
        gridX = gridPointsData["properties"]["gridX"];
        gridY = gridPointsData["properties"]["gridY"];
        coords.latitude = gridPointsData["properties"]["relativeLocation"]["geometry"]["coordinates"][0];
        coords.longitude = gridPointsData["properties"]["relativeLocation"]["geometry"]["coordinates"][1];
        city = gridPointsData["properties"]["relativeLocation"]["properties"]["city"];
        state = gridPointsData["properties"]["relativeLocation"]["properties"]["state"];
    }

    // Default, Burlington VT
    Location::Location() :
        name("app_default"),
        gridId("BTV"),
        gridX(89),
        gridY(55),
        coords {-73.182623000000007, 44.436469000000002},
        city("South Burlington"),
        state("VT") 
    {

    }
}
