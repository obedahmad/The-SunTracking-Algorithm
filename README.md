# The SunTracking Algorithm

<a href="https://github.com/PaulStoffregen/Time"><img src="https://badgen.net/badge/Reference/Time/red?icon=github"></a>

The supporting library, <strong>TimeLib for Arduino</strong>, by <a href="https://github.com/PaulStoffregen">PaulStoffregen</a>


A Sun Positioning Algorithm (SPA) utilizing only time and coordinates as the external inputs.
The Sun Positioning Algorithm (SPA) makes use of the local observer time and coordinates to calculate the topocentric solar `azimuth` and `elevation` angles based on the algorithm flowchart specified in Figure 1.

## Functionality

To use the Sun Positioning Algorithm routines, import the C++ header: 
```c
#include <SolarTracker.h>
```

The `time_t` variable type is used extensively throughout the algorithm and in other various time computations as defined by ISO C. For additional context visit: https://en.cppreference.com/w/c/chrono/time_t.

The following utility functions are available for computing angles:

This returns the struct object for elevation and azimuth angles
```c
SolarTrackerStruct getSolarPosition(time_t t);
```

This returns the float value for elevation angle
```c
float getSolarElevation(time_t t); //returns a float for elevation angle
```

This returns the float value for azimuth angle
```c
float getSolarAzimuth(time_t t); //returns a float for azimuth angle
```

The main compute function for angle calculations is declared as:
```c
calculateSolarPosition(time_t tParam, float Latitude, float Longitude); //performs computation for calculating solar position
```

## Behind the Scene

The algorithm is based on various astronomical almanacs that provide a backbone to the mathematical structure of the algorithm. The process of computing angles works in accordance to the following flowchart:

<center><img src="/Algorithm%20Block%20Diagram%201.svg" width=500 height=500></center>
<!-- ![Figure 1]() -->

## Copyright and license

This codebase is copyright Obed Ahmad and licensed with the MIT license.