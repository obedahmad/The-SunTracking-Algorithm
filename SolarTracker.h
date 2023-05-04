// Struct to store the Tracked Solar Position in terms of the angles: Elevation & Azimuth
struct SolarTrackerStruct
{
  double elevation = 0;
  double azimuth = 0;
  time_t time = 0;
};

// Computing Function Prototype
SolarTrackerStruct calculateSolarPosition(time_t tParam, float Latitude, float Longitude);

// Solar Tracker Class
class SolarTracker
{
  private:

    // Location:
    float Latitude;
    float Longitude;

    // Position Result:
    SolarTrackerStruct position;

  public:

    SolarTracker(float Latitude, float Longitude);

    static void setTimeProvider(getExternalTime getTimeFunction);

    SolarTrackerStruct getSolarPosition(time_t t);

    float getSolarElevation(time_t t);

    float getSolarAzimuth(time_t t);
};