#ifndef _SUNRISE_H_
#define _SUNRISE_H_

//formula for sunrise equation: https://en.wikipedia.org/wiki/Sunrise_equation

#define _USE_MATH_DEFINES

#include <string> //for now, later return a time data type
#include <iostream>
#include <chrono>
#include <ctime>
#include <cmath>

//put this in a namespace

time_t getSunrise(double latitude, double longitude, double elevation, int timezone); //returns timestamp of sunrise
time_t getSunset(double latitude, double longitude, double elevation, int timezone); //returns timestamp of sunset

//handle event with an enum
double _calculateSunEvent(int event, double latitude, double longitude, double elevation); //event=1 sunrise, event=-1 sunset

#endif