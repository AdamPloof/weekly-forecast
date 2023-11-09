# Weekly Forecast
Weekly forecast is a command line weather app. Forecasts are retrieved through the [NOAA forecast API](https://www.weather.gov/documentation/services-web-api) for any location in the United States.

## Features
- 7 day forecasts divided into day and night periods
- Retrieve forecast for a location by providing longitude and latitude coordinates
- Save locations by name to make fetching forecasts easier
- Set home/default location
- Format output either by row or grid styles

## Building
Right now, you'll have to build the executable yourself. There's a `Makefile` that is set up to use the GCC compiler. After building, you can place the executable wherever you'd like. The executable will be named *forecast*. Add the executable directory to your `PATH` to be able call it from anywhere.

## Usage
<pre>
Forecast CLI provides a multi-day weather forecast for anywhere in the United States

Usage:
    forecast [options] [<arguments>]

Options:
    --help, -h                       Show help
    --verbose                        Show debugging info including warnings
    --coords, -c LONGITUDE,LATITUDE  Get the forecast for a specific longitude and latitude
                                     Separate coordinates pair with a comma (no space)
    --days, -d INTEGER               The number of days you'd like a forecast for
    --location, -l NAME              Get the forecast for a saved location
    --add-location, -a NAME          When combined with --coords, save the location by
                                     the name provided for later use
    --remove-location, -r NAME       Remove a saved location
    --set-home, -s NAME              Set the default/home location
    --mode, -m row|grid              Choose the output mode: row or grid [default: grid]
    --user-agent, -u EMAIL           Set the email address to use when requesting forecasts
                                     from the NOAA weather API
    --show--locations                Output a list of all saved locations
    --show-config                    Output the current configuration defaults for verbosity,
                                     days, home-location, and User-Agent
</pre>

## Config
Weekly forecast stores configuration such as saved locations, home location, etc. in a *config.json* file. This file is saved to a *config/* directory which is located in the same directory as the exectutable.

### User-Agent
On the first run, you will be prompted to enter an email address which will be used as the `User-Agent` for calls to the NOAA API. NOAA requires that this header be present when making calls to the forecast endpoint.

From the NOAA API docs
> A User Agent is required to identify your application. This string can be anything, and the more unique to your application the less likely it will be affected by a security event. If you include contact information (website or email), we can contact you if your string is associated to a security event. This will be replaced with an API key in the future.

If you'd like to change the `User-Agent` later, you can set it with the `--user-agent` option. Use the `--show-config` option to check the current `User-Agent`.

## Limitations
- Can only provide forecasts for the United States
- No easy installer, you've got to build it yourself
- Makefile is only setup for compiling with GCC
- Has only been tested on Mac
- There's probably some bugs I haven't found yet ¯\_(ツ)_/¯
