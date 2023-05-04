//All necessary includes from MINGW and Arduino
#include <iostream>
#include <math.h>
#include <time.h>
#include <ctime>
#include "SolarTracker.h"
#include "TimeLib.h"
// #include <Arduino.h>
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105

#define  tmYearToCalendar(Y) ((Y) + 1970)  // full four digit year

//
//Computing Functions for Solar position
//
//Local Standard Time Meridian
double LSTM(double delta) {
    double position = 15 * delta;
    return position;
}

// Equation of Time
double EOT(int day) {
    double B = 360 / 365 * (day - 81);
    double position = 9.87 * sin(2 * B * M_PI / 180) - 7.53 * cos(B * M_PI / 180) - 1.5 * sin(B * M_PI / 180);
    return position;
}

// Time Correction Factor
double TC(double longitude, double LSTM, double EOT) {
    double position = 4 * (longitude - LSTM) + EOT;
    return position;
}

// Local Solar Time | LC in hours
double LST(double LT, double TC) {
    double position = LT + TC / 60;
    return position;
}

// Hour Angle
double HRA(double LST) {
    double position = 15 * (LST - 12);
    return position;
}

// Declination Angle
double DA(int day) {
    double B = 360 / 365 * (day - 81);
    double position = asin((sin(23.45 * M_PI / 180) * sin(B * M_PI / 180))) * 180 / M_PI;
    return position;
}

// Elevation Angle
double EA(double latitude, double DA, double HRA) {
    double position = asin(sin(DA * M_PI / 180) * sin(latitude * M_PI / 180) +
                            cos(DA * M_PI / 180) * cos(latitude * M_PI / 180) * cos(HRA * M_PI / 180)) * 180 / M_PI;
    return position;
}

// Azimuth Angle
double AA(double latitude, double EA, double DA, double LST, double HRA) {
    double Azimuth = acos((sin(DA * M_PI / 180) * cos(latitude * M_PI / 180) -
                            cos(DA * M_PI / 180) * sin(latitude * M_PI / 180) * cos(HRA * M_PI / 180)) / cos(EA * M_PI / 180)) * 180 / M_PI;
    if (LST < 12 || HRA < 0) {
        return Azimuth;
    } else {
        return 360 - Azimuth;
    }
}

//
//Class Methods for SolarTracker Class
//
// constructor
SolarTracker::SolarTracker(const float Lat, const float Lon) {
	Latitude = Lat * DEG_TO_RAD;
	Longitude = Lon * DEG_TO_RAD;
}

// Get Position for specified time
SolarTrackerStruct SolarTracker::getSolarPosition(time_t t) {
	SolarTrackerStruct pos;
	position = calculateSolarPosition(t, Latitude, Longitude);
	pos.elevation = position.elevation * RAD_TO_DEG;
	pos.azimuth = position.azimuth * RAD_TO_DEG;
	pos.time = t;
	return pos;
}

// Get Elevation for specified time
float SolarTracker::getSolarElevation(time_t t) {
	position = calculateSolarPosition(t, Latitude, Longitude);
	return position.elevation * RAD_TO_DEG;
}

// Get Azimuth for specified time
//
float SolarTracker::getSolarAzimuth(time_t t) {
	position = calculateSolarPosition(t, Latitude, Longitude);
	return position.azimuth * RAD_TO_DEG;
}

//Compute function for calculating solar position angles: Azimuth and Elevation
SolarTrackerStruct calculateSolarPosition(time_t hardwareTime, float Latitude, float Longitude) {

	tmElements_t dividedTime;
	static time_t timePrevious = 0;
	static float latPrevious;
	static float lonPrevious;
	static SolarTrackerStruct position;

    int dayNumber;
    double delta;
	double Declination;
	double hourAngle;
    double lstm;
    double eot;
    double tc;
    double lst;

	if ((hardwareTime != timePrevious) || (Latitude != latPrevious) || (Longitude != lonPrevious)) // only calculate if time or location has changed
	{
		breakTime(hardwareTime, dividedTime);
        dayNumber = day(hardwareTime);
        time_t current = time(NULL);
        tm *tm_gmt = gmtime(&current);
        double diff_seconds = difftime(current, mktime(tm_gmt));
        delta = diff_seconds/3600;

        lstm = LSTM(delta);
        eot = EOT(dayNumber);
        tc = TC(Longitude, lstm, eot);

        double hoursNow = dividedTime.Hour;
        lst = LST(hoursNow, tc);
        hourAngle = HRA(lst);
        Declination = DA(dayNumber);

		// elevation from the horizon
		position.elevation = EA(Latitude, Declination, hourAngle); 
        
		// Azimuth measured eastward from north.
		position.azimuth = AA(Latitude, position.elevation, Declination, lst, hourAngle);
        
		// copy the time
		position.time = hardwareTime;

		// remember the parameters
		timePrevious = hardwareTime;
		latPrevious = Latitude;
		lonPrevious = Longitude;
	}
	return position;
}