#include <iostream>
#include <ctime>
#include <fstream>
#include <sstream>
#include <map>
#include <string>

#ifdef _WIN32
#include <windows.h>
#include <shlwapi.h>
#else
#include <unistd.h>
#include <limits.h>
#endif

#include "sunrise.h"

bool readConfig(double &latitude, double &longitude, double &elevation, int &timezone);

int main() {

    //Belgrade
    //const double latitude = 44.787197;
    //const double longitude = 20.457273;
    //const double elevation = 117; //ovo moze da bude int
    //const int timezone = 1;

    double latitude, longitude, elevation;
    int timezone;
    if(!readConfig(latitude, longitude, elevation, timezone)) {
        return -1;
    }

    std::cout << '\n';
    std::cout << (latitude>=0?'N':'S') << std::abs(latitude) << ' ' << (longitude>=0?'E':'W') << std::abs(longitude) << '\n';
    std::cout << elevation << "m elevation\n";
    std::cout << "GMT" << (timezone>=0?'+':'-') << timezone << '\n';

    time_t currentTime = std::time(NULL);
    std::cout << '\n' <<  std::ctime(&currentTime) << '\n';

    //izgleda da ctime pazi o vremenskim zonama :)
    //posle 12:00 pokazuje zalazak sunca za sutra uvece
    time_t sunrise = getSunrise(latitude, longitude, elevation, timezone);
    std::cout << "Sunrise: " << std::ctime(&sunrise);

    time_t sunset = getSunset(latitude, longitude, elevation, timezone);
    std::cout << "Sunset: " << std::ctime(&sunset);

    return 0;
}

//still needs handling of missing or duplicate keys
bool readConfig(double &latitude, double &longitude, double &elevation, int &timezone) {

    const unsigned PATH_LEN = 4096;
    char buffer[PATH_LEN];

#ifdef _WIN32
    GetModuleFileName(NULL, buffer, sizeof(buffer));
    PathRemoveFileSpec(buffer);
#else
    //not tested on linux
    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer)-1);
    if (len != -1) {
        buffer[len] = '\0';
    } else {
        buffer[0] = '\0';
    }
#endif

    std::string filename = std::string(buffer) + "/coordinates.ini";
    std::fstream configFile(filename);
    if (!configFile.is_open()) {
        std::cerr << "Error opening file " << filename << '\n';
        return false;
    }

    std::map<std::string, std::string> config;
    std::string line;
    while(std::getline(configFile, line)) {
        std::istringstream lineStream(line);
        std::string key, value;
        if(!std::getline(lineStream, key, '=')) {
            std::cerr << "Error reading key from line " << line << '\n';
            return false;
        }
        if(!std::getline(lineStream, value)) {
            std::cerr << "Error reading value from line " << line << '\n';
            return false;
        }

        if(config.find(key) != config.end()) {
            std::cerr << "Duplicate key " << key << '\n';
            return false;
        }
        config[key] = value;
    }
    configFile.close();

    if(config.find("LATITUDE") != config.end()) {
        latitude = std::stod(config["LATITUDE"]);
    } else {
        std::cerr << "Missing key LATITUDE\n";
        return false;
    }

    if(config.find("LONGITUDE") != config.end()) {
        longitude = std::stod(config["LONGITUDE"]);
    } else {
        std::cerr << "Missing key LONGITUDE\n";
        return false;
    }

    if(config.find("ELEVATION") != config.end()) {
        elevation = std::stod(config["ELEVATION"]);
    } else {
        std::cerr << "Missing key ELEVATION\n";
        return false;
    }

    if(config.find("TIMEZONE") != config.end()) {
        timezone = std::stoi(config["TIMEZONE"]);
    } else {
        std::cerr << "Missing key TIMEZONE\n";
        return false;
    }

    return true;
}