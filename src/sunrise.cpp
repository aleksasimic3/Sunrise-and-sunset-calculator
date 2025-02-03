#include "sunrise.h"

namespace {
    double radians(double deg) {
        return deg * M_PI/180.0;
    }
    double degrees(double rad) {
        return rad * 180.0/M_PI;
    }
    double jDateFromTimeStamp(double ts){
        return ts/86400.0 + 2440587.5;
    }
    double timeStampFromJDate(double jDate) {
        return (jDate - 2440587.5) * 86400.0;
    }
}

time_t getSunrise(double latitude, double longitude, double elevation, int timezone) {

    double jRise = _calculateSunEvent(1, latitude, longitude, elevation);

    time_t sunrise = timeStampFromJDate(jRise);
    return sunrise;
}

time_t getSunset(double latitude, double longitude, double elevation, int timezone) {

    double jSet = _calculateSunEvent(-1, latitude, longitude, elevation);

    time_t sunset = timeStampFromJDate(jSet);
    return sunset;
}

double _calculateSunEvent(int event, double latitude, double longitude, double elevation) {

    //calculate mean solar time
    auto timestamp = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    double jDate = jDateFromTimeStamp(timestamp);
    double n = std::ceil(jDate - 2451545.0 + 0.0008);
    double meanSolarTime = n - longitude/360.0;

    double solarMeanAnomaly = std::fmod(357.5291 + 0.98560028 * meanSolarTime, 360);

    //calculate ecliptic longitude
    double c =  1.9148*std::sin(radians(solarMeanAnomaly)) + 0.02*std::sin(2*radians(solarMeanAnomaly)) + 0.0003*std::sin(3*radians(solarMeanAnomaly));
    const double argPerihelion = 102.9372;
    double eclipticLongitude = std::fmod(solarMeanAnomaly + c + 180 + argPerihelion, 360);
    
    //solar transit (Julian date of solar noon) //~1.5 minute error
    double eqTime = 0.0053*std::sin(radians(solarMeanAnomaly)) - 0.0069*std::sin(2*radians(eclipticLongitude));
    double jTransit = 2451545.0 + meanSolarTime + eqTime;

    //declination of the sun d_sin and d_cos
    const double earthAxialTilt = radians(23.4397);
    double d_sin = std::sin(radians(eclipticLongitude)) * std::sin(earthAxialTilt);
    double d_cos = std::cos(std::asin(d_sin));
    
    //hour angle //~0.5 degree error
    const double elevationCorrection = std::sin(radians(-0.833));
    const double lat_sin = std::sin(radians(latitude));
    const double lat_cos = std::cos(radians(latitude));
    double w0 = std::acos((elevationCorrection - lat_sin*d_sin) / (lat_cos * d_cos));

    double jEvent = jTransit + (-1)*event*degrees(w0)/360; //O.o

    return jEvent;
}