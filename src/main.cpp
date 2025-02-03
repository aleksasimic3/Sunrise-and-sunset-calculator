#include <iostream>

#include "sunrise.h"

int main() {

    //Belgrade
    const double latitude = 44.787197;
    const double longitude = 20.457273;
    const double elevation = 117;
    const int timezone = 1;

    time_t sunrise = getSunrise(latitude, longitude, elevation, timezone);
    std::cout << "\nSunrise: " << std::ctime(&sunrise) << '\n';

    time_t sunset = getSunset(latitude, longitude, elevation, timezone);
    std::cout << "Sunset: " << std::ctime(&sunset) << '\n';

    return 0;
}