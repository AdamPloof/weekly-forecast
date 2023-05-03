#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <zip.hpp>
#include <forecast_params.hpp>

// static int verbose_flag;

namespace {
    using namespace weekly_forecast;

    void help() {
        std::cout << "Help func TODO.";

        exit(1);
    }

    ForecastParams getParams(int argc, char* argv[]) {
        const char* const shortOpts = "z:d:";
        static struct option longOpts[] = {
            {"zip", required_argument, nullptr, 'z'},
            {"days", required_argument, nullptr, 'd'},
            {nullptr, 0, nullptr, 0}
        };

        int arg;
        int argIdx;
        ForecastParams params;
        while (true) {
            arg = getopt_long(argc, argv, shortOpts, longOpts, &argIdx);
            if (arg == -1) {
                break;
            }
            switch (arg) {
                case 0:
                    std::cout << "No flags to set, how did we get here?\n";
                    break;
                case 'z':
                    params.zip = optarg;
                    break;
                case 'd':
                    params.days = std::stoi(optarg);
                    break;
                case '?':
                    // Note that getopt_long already prints an error msg when this happens
                    // just putting something here as a placeholder in case we want to handle this
                    // in some special way.
                    // Maybe print help by default?
                    std::cout << "Did not recognize argument: " << arg << std::endl;
                    break;
                default:
                    abort();
            }
        }

        return params;
    }
}

int main(int argc, char* argv[]) {
    // TODO: move this out of main
    using namespace weekly_forecast;

    ForecastParams params = getParams(argc, argv);

    // TODO: Something is definitely wrong with the way we're intializing the params
    // struct because the value in params.zip->getCode() is way weird.
    std::cout << "Zip is: " << params.zip << std::endl;
    std::cout << "Number of days is: " << params.days << std::endl ;

    return 0;
}
