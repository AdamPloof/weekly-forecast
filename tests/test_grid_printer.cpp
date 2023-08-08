#include <iostream>
#include <sstream>
#include <string>
#include <nlohmann/json.hpp>
#include "grid_printer.hpp"
#include "location.hpp"
#include "forecast.hpp"

using json = nlohmann::json;
using namespace forecast;

#define ASSERT_TRUE(exp, msg) if (!(exp)) { \
    std::cout << __FUNCTION__ << " failed on line " << __LINE__ \
    << ": " << msg << std::endl; \
    m_failCnt++; \
} else { \
    m_passCnt++; \
}

namespace test::grid_printer {
    class TestGridPrinter {
        public:
            TestGridPrinter(Forecast* forecast) :
                m_forecast(forecast),
                m_printer(GridPrinter()) {
            }

            void run() {
                testLineWidthIsFixed();
                reportResults();
            }

            void reportResults() {
                int testCnt = m_passCnt + m_failCnt;
                std::cout << "Finshed running " << testCnt << " tests." << std::endl;
                std::cout << "Passed: " << m_passCnt << std::endl;
                std::cout << "Failed: " << m_failCnt << std::endl;
            }

            void testLineWidthIsFixed() {
                m_printer.render(m_forecast, 7);
                const std::ostringstream& output = m_printer.getOutput();
                std::istringstream input;
                input.str(output.str());

                for (std::string line; std::getline(input, line, '\n'); ) {
                    std::string errMsg = "line does not match fixed width. Expected: ";
                    errMsg.append(std::to_string(m_lineWidth) + ", got: ");
                    errMsg.append(std::to_string(line.length()));

                    ASSERT_TRUE((line.length() == m_lineWidth), errMsg);
                }
            }
        private:
            Forecast* m_forecast;
            GridPrinter m_printer;
            size_t m_lineWidth = 129;
            int m_passCnt = 0;
            int m_failCnt = 0;
    };
}

json getMockForecastData() {
    return json::parse(R"({
        "@context": [
            "https://geojson.org/geojson-ld/geojson-context.jsonld",
            {
                "@version": "1.1",
                "wx": "https://api.weather.gov/ontology#",
                "geo": "http://www.opengis.net/ont/geosparql#",
                "unit": "http://codes.wmo.int/common/unit/",
                "@vocab": "https://api.weather.gov/ontology#"
            }
        ],
        "type": "Feature",
        "geometry": {
            "type": "Polygon",
            "coordinates": [
                [
                    [
                        -72.8986994,
                        44.3939433
                    ],
                    [
                        -72.9035729,
                        44.372765099999995
                    ],
                    [
                        -72.8739411,
                        44.3692796
                    ],
                    [
                        -72.86906119999999,
                        44.390457399999995
                    ],
                    [
                        -72.8986994,
                        44.3939433
                    ]
                ]
            ]
        },
        "properties": {
            "updated": "2023-05-11T02:02:50+00:00",
            "units": "us",
            "forecastGenerator": "BaselineForecastGenerator",
            "generatedAt": "2023-05-11T04:00:24+00:00",
            "updateTime": "2023-05-11T02:02:50+00:00",
            "validTimes": "2023-05-10T20:00:00+00:00/P7DT17H",
            "elevation": {
                "unitCode": "wmoUnit:m",
                "value": 225.8568
            },
            "periods": [
                {
                    "number": 1,
                    "name": "Overnight",
                    "startTime": "2023-05-11T00:00:00-04:00",
                    "endTime": "2023-05-11T06:00:00-04:00",
                    "isDaytime": false,
                    "temperature": 46,
                    "temperatureUnit": "F",
                    "temperatureTrend": null,
                    "probabilityOfPrecipitation": {
                        "unitCode": "wmoUnit:percent",
                        "value": null
                    },
                    "dewpoint": {
                        "unitCode": "wmoUnit:degC",
                        "value": 5
                    },
                    "relativeHumidity": {
                        "unitCode": "wmoUnit:percent",
                        "value": 79
                    },
                    "windSpeed": "3 mph",
                    "windDirection": "W",
                    "icon": "https://api.weather.gov/icons/land/night/sct?size=medium",
                    "shortForecast": "Partly Cloudy",
                    "detailedForecast": "Partly cloudy, with a low around 46. West wind around 3 mph."
                },
                {
                    "number": 2,
                    "name": "Thursday",
                    "startTime": "2023-05-11T06:00:00-04:00",
                    "endTime": "2023-05-11T18:00:00-04:00",
                    "isDaytime": true,
                    "temperature": 73,
                    "temperatureUnit": "F",
                    "temperatureTrend": null,
                    "probabilityOfPrecipitation": {
                        "unitCode": "wmoUnit:percent",
                        "value": null
                    },
                    "dewpoint": {
                        "unitCode": "wmoUnit:degC",
                        "value": 7.777777777777778
                    },
                    "relativeHumidity": {
                        "unitCode": "wmoUnit:percent",
                        "value": 86
                    },
                    "windSpeed": "3 to 12 mph",
                    "windDirection": "W",
                    "icon": "https://api.weather.gov/icons/land/day/few?size=medium",
                    "shortForecast": "Sunny",
                    "detailedForecast": "Sunny, with a high near 73. West wind 3 to 12 mph."
                },
                {
                    "number": 3,
                    "name": "Thursday Night",
                    "startTime": "2023-05-11T18:00:00-04:00",
                    "endTime": "2023-05-12T06:00:00-04:00",
                    "isDaytime": false,
                    "temperature": 52,
                    "temperatureUnit": "F",
                    "temperatureTrend": null,
                    "probabilityOfPrecipitation": {
                        "unitCode": "wmoUnit:percent",
                        "value": 30
                    },
                    "dewpoint": {
                        "unitCode": "wmoUnit:degC",
                        "value": 8.333333333333334
                    },
                    "relativeHumidity": {
                        "unitCode": "wmoUnit:percent",
                        "value": 83
                    },
                    "windSpeed": "3 to 8 mph",
                    "windDirection": "W",
                    "icon": "https://api.weather.gov/icons/land/night/rain_showers,20/rain_showers,30?size=medium",
                    "shortForecast": "Chance Rain Showers",
                    "detailedForecast": "A chance of rain showers after 10pm. Partly cloudy, with a low around 52. West wind 3 to 8 mph. Chance of precipitation is 30%."
                },
                {
                    "number": 4,
                    "name": "Friday",
                    "startTime": "2023-05-12T06:00:00-04:00",
                    "endTime": "2023-05-12T18:00:00-04:00",
                    "isDaytime": true,
                    "temperature": 75,
                    "temperatureUnit": "F",
                    "temperatureTrend": null,
                    "probabilityOfPrecipitation": {
                        "unitCode": "wmoUnit:percent",
                        "value": 40
                    },
                    "dewpoint": {
                        "unitCode": "wmoUnit:degC",
                        "value": 10
                    },
                    "relativeHumidity": {
                        "unitCode": "wmoUnit:percent",
                        "value": 80
                    },
                    "windSpeed": "6 to 15 mph",
                    "windDirection": "W",
                    "icon": "https://api.weather.gov/icons/land/day/tsra_hi,30/tsra_hi,40?size=medium",
                    "shortForecast": "Chance Showers And Thunderstorms",
                    "detailedForecast": "A chance of rain showers before 11am, then a chance of showers and thunderstorms. Partly sunny, with a high near 75. West wind 6 to 15 mph. Chance of precipitation is 40%."
                },
                {
                    "number": 5,
                    "name": "Friday Night",
                    "startTime": "2023-05-12T18:00:00-04:00",
                    "endTime": "2023-05-13T06:00:00-04:00",
                    "isDaytime": false,
                    "temperature": 52,
                    "temperatureUnit": "F",
                    "temperatureTrend": null,
                    "probabilityOfPrecipitation": {
                        "unitCode": "wmoUnit:percent",
                        "value": 30
                    },
                    "dewpoint": {
                        "unitCode": "wmoUnit:degC",
                        "value": 11.11111111111111
                    },
                    "relativeHumidity": {
                        "unitCode": "wmoUnit:percent",
                        "value": 89
                    },
                    "windSpeed": "7 to 13 mph",
                    "windDirection": "NW",
                    "icon": "https://api.weather.gov/icons/land/night/tsra_hi,30?size=medium",
                    "shortForecast": "Chance Showers And Thunderstorms",
                    "detailedForecast": "A chance of showers and thunderstorms before 1am, then a slight chance of showers and thunderstorms. Mostly cloudy, with a low around 52. Northwest wind 7 to 13 mph. Chance of precipitation is 30%. New rainfall amounts less than a tenth of an inch possible."
                },
                {
                    "number": 6,
                    "name": "Saturday",
                    "startTime": "2023-05-13T06:00:00-04:00",
                    "endTime": "2023-05-13T18:00:00-04:00",
                    "isDaytime": true,
                    "temperature": 65,
                    "temperatureUnit": "F",
                    "temperatureTrend": null,
                    "probabilityOfPrecipitation": {
                        "unitCode": "wmoUnit:percent",
                        "value": null
                    },
                    "dewpoint": {
                        "unitCode": "wmoUnit:degC",
                        "value": 9.444444444444445
                    },
                    "relativeHumidity": {
                        "unitCode": "wmoUnit:percent",
                        "value": 89
                    },
                    "windSpeed": "6 to 14 mph",
                    "windDirection": "NW",
                    "icon": "https://api.weather.gov/icons/land/day/rain_showers/few?size=medium",
                    "shortForecast": "Slight Chance Rain Showers then Sunny",
                    "detailedForecast": "A slight chance of rain showers before 7am. Sunny, with a high near 65. Northwest wind 6 to 14 mph. New rainfall amounts less than a tenth of an inch possible."
                },
                {
                    "number": 7,
                    "name": "Saturday Night",
                    "startTime": "2023-05-13T18:00:00-04:00",
                    "endTime": "2023-05-14T06:00:00-04:00",
                    "isDaytime": false,
                    "temperature": 39,
                    "temperatureUnit": "F",
                    "temperatureTrend": null,
                    "probabilityOfPrecipitation": {
                        "unitCode": "wmoUnit:percent",
                        "value": null
                    },
                    "dewpoint": {
                        "unitCode": "wmoUnit:degC",
                        "value": 1.6666666666666667
                    },
                    "relativeHumidity": {
                        "unitCode": "wmoUnit:percent",
                        "value": 70
                    },
                    "windSpeed": "5 to 13 mph",
                    "windDirection": "NW",
                    "icon": "https://api.weather.gov/icons/land/night/few?size=medium",
                    "shortForecast": "Mostly Clear",
                    "detailedForecast": "Mostly clear, with a low around 39. Northwest wind 5 to 13 mph."
                },
                {
                    "number": 8,
                    "name": "Sunday",
                    "startTime": "2023-05-14T06:00:00-04:00",
                    "endTime": "2023-05-14T18:00:00-04:00",
                    "isDaytime": true,
                    "temperature": 57,
                    "temperatureUnit": "F",
                    "temperatureTrend": null,
                    "probabilityOfPrecipitation": {
                        "unitCode": "wmoUnit:percent",
                        "value": null
                    },
                    "dewpoint": {
                        "unitCode": "wmoUnit:degC",
                        "value": -1.1111111111111112
                    },
                    "relativeHumidity": {
                        "unitCode": "wmoUnit:percent",
                        "value": 70
                    },
                    "windSpeed": "5 to 8 mph",
                    "windDirection": "N",
                    "icon": "https://api.weather.gov/icons/land/day/sct?size=medium",
                    "shortForecast": "Mostly Sunny",
                    "detailedForecast": "Mostly sunny, with a high near 57. North wind 5 to 8 mph."
                },
                {
                    "number": 9,
                    "name": "Sunday Night",
                    "startTime": "2023-05-14T18:00:00-04:00",
                    "endTime": "2023-05-15T06:00:00-04:00",
                    "isDaytime": false,
                    "temperature": 37,
                    "temperatureUnit": "F",
                    "temperatureTrend": null,
                    "probabilityOfPrecipitation": {
                        "unitCode": "wmoUnit:percent",
                        "value": null
                    },
                    "dewpoint": {
                        "unitCode": "wmoUnit:degC",
                        "value": 1.6666666666666667
                    },
                    "relativeHumidity": {
                        "unitCode": "wmoUnit:percent",
                        "value": 89
                    },
                    "windSpeed": "5 mph",
                    "windDirection": "E",
                    "icon": "https://api.weather.gov/icons/land/night/sct?size=medium",
                    "shortForecast": "Partly Cloudy",
                    "detailedForecast": "Partly cloudy, with a low around 37. East wind around 5 mph."
                },
                {
                    "number": 10,
                    "name": "Monday",
                    "startTime": "2023-05-15T06:00:00-04:00",
                    "endTime": "2023-05-15T18:00:00-04:00",
                    "isDaytime": true,
                    "temperature": 67,
                    "temperatureUnit": "F",
                    "temperatureTrend": null,
                    "probabilityOfPrecipitation": {
                        "unitCode": "wmoUnit:percent",
                        "value": null
                    },
                    "dewpoint": {
                        "unitCode": "wmoUnit:degC",
                        "value": 2.2222222222222223
                    },
                    "relativeHumidity": {
                        "unitCode": "wmoUnit:percent",
                        "value": 82
                    },
                    "windSpeed": "2 to 7 mph",
                    "windDirection": "SW",
                    "icon": "https://api.weather.gov/icons/land/day/sct/rain_showers?size=medium",
                    "shortForecast": "Mostly Sunny then Slight Chance Rain Showers",
                    "detailedForecast": "A slight chance of rain showers after 2pm. Mostly sunny, with a high near 67. Southwest wind 2 to 7 mph."
                },
                {
                    "number": 11,
                    "name": "Monday Night",
                    "startTime": "2023-05-15T18:00:00-04:00",
                    "endTime": "2023-05-16T06:00:00-04:00",
                    "isDaytime": false,
                    "temperature": 46,
                    "temperatureUnit": "F",
                    "temperatureTrend": null,
                    "probabilityOfPrecipitation": {
                        "unitCode": "wmoUnit:percent",
                        "value": 30
                    },
                    "dewpoint": {
                        "unitCode": "wmoUnit:degC",
                        "value": 4.444444444444445
                    },
                    "relativeHumidity": {
                        "unitCode": "wmoUnit:percent",
                        "value": 76
                    },
                    "windSpeed": "5 to 10 mph",
                    "windDirection": "W",
                    "icon": "https://api.weather.gov/icons/land/night/rain_showers,30?size=medium",
                    "shortForecast": "Chance Rain Showers",
                    "detailedForecast": "A chance of rain showers. Mostly cloudy, with a low around 46. West wind 5 to 10 mph. Chance of precipitation is 30%."
                },
                {
                    "number": 12,
                    "name": "Tuesday",
                    "startTime": "2023-05-16T06:00:00-04:00",
                    "endTime": "2023-05-16T18:00:00-04:00",
                    "isDaytime": true,
                    "temperature": 61,
                    "temperatureUnit": "F",
                    "temperatureTrend": null,
                    "probabilityOfPrecipitation": {
                        "unitCode": "wmoUnit:percent",
                        "value": 30
                    },
                    "dewpoint": {
                        "unitCode": "wmoUnit:degC",
                        "value": 3.888888888888889
                    },
                    "relativeHumidity": {
                        "unitCode": "wmoUnit:percent",
                        "value": 74
                    },
                    "windSpeed": "10 to 15 mph",
                    "windDirection": "NW",
                    "icon": "https://api.weather.gov/icons/land/day/rain_showers/rain_showers,30?size=medium",
                    "shortForecast": "Chance Rain Showers",
                    "detailedForecast": "A chance of rain showers. Partly sunny, with a high near 61. Northwest wind 10 to 15 mph. Chance of precipitation is 30%."
                },
                {
                    "number": 13,
                    "name": "Tuesday Night",
                    "startTime": "2023-05-16T18:00:00-04:00",
                    "endTime": "2023-05-17T06:00:00-04:00",
                    "isDaytime": false,
                    "temperature": 39,
                    "temperatureUnit": "F",
                    "temperatureTrend": null,
                    "probabilityOfPrecipitation": {
                        "unitCode": "wmoUnit:percent",
                        "value": 30
                    },
                    "dewpoint": {
                        "unitCode": "wmoUnit:degC",
                        "value": 4.444444444444445
                    },
                    "relativeHumidity": {
                        "unitCode": "wmoUnit:percent",
                        "value": 76
                    },
                    "windSpeed": "2 to 10 mph",
                    "windDirection": "W",
                    "icon": "https://api.weather.gov/icons/land/night/rain_showers,30/sct?size=medium",
                    "shortForecast": "Chance Rain Showers then Partly Cloudy",
                    "detailedForecast": "A chance of rain showers before 8pm. Partly cloudy, with a low around 39. West wind 2 to 10 mph. Chance of precipitation is 30%."
                },
                {
                    "number": 14,
                    "name": "Wednesday",
                    "startTime": "2023-05-17T06:00:00-04:00",
                    "endTime": "2023-05-17T18:00:00-04:00",
                    "isDaytime": true,
                    "temperature": 60,
                    "temperatureUnit": "F",
                    "temperatureTrend": null,
                    "probabilityOfPrecipitation": {
                        "unitCode": "wmoUnit:percent",
                        "value": null
                    },
                    "dewpoint": {
                        "unitCode": "wmoUnit:degC",
                        "value": 2.7777777777777777
                    },
                    "relativeHumidity": {
                        "unitCode": "wmoUnit:percent",
                        "value": 70
                    },
                    "windSpeed": "2 to 6 mph",
                    "windDirection": "SW",
                    "icon": "https://api.weather.gov/icons/land/day/sct?size=medium",
                    "shortForecast": "Mostly Sunny",
                    "detailedForecast": "Mostly sunny, with a high near 60. Southwest wind 2 to 6 mph."
                }
            ]
        }
    })");
}

using namespace test::grid_printer;

int main() {
    std::cout << "##### Running GridPrinter Tests #####\n" << std::endl;

    json data = getMockForecastData();
    Location loc = Location("test_loc");
    Forecast forecast = Forecast(data, &loc);
    TestGridPrinter testCase = TestGridPrinter(&forecast);
    testCase.run();

    return 0;
}
