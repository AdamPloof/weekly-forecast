#include "location.hpp"
#include <iostream>

namespace forecast {
    Location::Location(json gridPointsData) : name("") {
        gridId = gridPointsData["properties"]["gridId"];
        gridX = gridPointsData["properties"]["gridX"];
        gridY = gridPointsData["properties"]["gridY"];
        coords.latitude = gridPointsData["properties"]["relativeLocation"]["geometry"]["coordinates"][0];
        coords.longitude = gridPointsData["properties"]["relativeLocation"]["geometry"]["coordinates"][1];
        city = gridPointsData["properties"]["relativeLocation"]["properties"]["city"];
        state = gridPointsData["properties"]["relativeLocation"]["properties"]["state"];
    }
}
