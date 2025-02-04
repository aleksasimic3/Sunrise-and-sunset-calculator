#include <iostream>
#include <ctime>

#include "sunrise.h"

int main() {

    //Belgrade
    const double latitude = 44.787197;
    const double longitude = 20.457273;
    const double elevation = 117;
    const int timezone = 1;

    std::cout << '\n';
    std::cout << (latitude>=0?'N':'S') << std::abs(latitude) << ' ' << (longitude>=0?'E':'W') << std::abs(longitude) << '\n';
    std::cout << elevation << "m elevation\n";
    std::cout << "GMT" << (timezone>=0?'+':'-') << timezone << '\n';

    time_t currentTime = std::time(NULL);
    std::cout << '\n' <<  std::ctime(&currentTime) << '\n';

    //izgleda da ctime pazi o vremenskim zonama :)
    time_t sunrise = getSunrise(latitude, longitude, elevation, timezone);
    std::cout << "Sunrise: " << std::ctime(&sunrise);

    time_t sunset = getSunset(latitude, longitude, elevation, timezone);
    std::cout << "Sunset: " << std::ctime(&sunset);

    return 0;
}