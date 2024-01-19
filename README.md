# SatFindCpp

Finding the position of LEO satellites by the SGP4/SDP4

# Quick Start

```C++
#include <SatFind/Core>
using namespace satfind;

int main() {

    // Translation of TLE (3LE, 2LE) 
    auto iss_tle = Tle {
        "ISS (ZARYA)             ",
        "1 25544U 98067A   24018.43698023  .00021385  00000+0  38757-3 0  9991", 
        "2 25544  51.6427 342.3169 0004949 101.3994  45.6784 15.49554946435174"
    };

    // Create ISS Orbital Propagator
    auto iss = OrbitalPropagator{iss_tle};

    // Calculate the position of the ISS at the current time
    std::cout << iss.trackFlightObject(DateTime::now()) << std::endl;
}
```

# How to use

## 1. Import this library into your project

All functions are available by including `Core.hpp`.

```C++
#include <SatFind/Core.hpp>
```

All APIs are in the `satfind` namespace.

## 2. Time Definition

Time operations are performed in the `DateTime` class.
Only ISO-8601 Extended format (`YYYYY-MM-DDThh:mm:ss.ssss±hh:mm`) is supported as input format.
However, the following degrees of freedom are available.

1. If only `YYYYY-MM-DD` is used, it is interpreted as `YYYYY-MM-DDT00:00:00`.
1. Any single letter may be used for `T` in `<date>T<time>`.
1. There is no restriction on the length of the decimal part `.ssss` only the integer part `ss` is acceptable.
1. If time zone `±hh:mm` is not specified or `Z` is used, it is treated as UTC+0.

```C++
DateTime dt("2000-02-20T02:20:00.00+09:00");
```

### 2.1 Convert time system

Conversion of major time formats is performed as follows.
For example, UNIX time, Julian date, and modified Julian date (based on JD2000.0) are converted as follows

```C++
DateTime dt("2023-12-03T00:00:00");

std::cout << "UNIX" << dt.unixTime() << std::endl;
std::cout << "JD: " << dt.julianDay()  << std::endl;
std::cout << "MJD(JD2000.0): " << dt.j2000()  << std::endl;
```

Other conversions to Greenwich sidereal time, local sidereal time, and earth time (predicted) are also available.

### 2.2 Arithmetic evaluation

Arithmetic operations on time are performed using the dedicated `addXXX` member function or the `TimeSpan` class.
Standard arithmetic, relational, and equality operators are supported.

```C++
DateTime dt("2023-12-03T00:00:00");

std::cout << dt.addMinutes(1) << std::endl;
std::cout << dt + TimeSpan(1, TimeUnit::Minutes) << std::endl;
std::cout << dt <= DateTime("2024-12-03T00:00:00") << std::endl;
```

## 3 Angle Definition

Angles are represented by the `Angle` class.
This class holds the normalized angles and provides smooth conversion to individual angle units.

Angles can be expressed in arc radians, degrees, hours, arcminutes, and arcseconds.
HMS and DMS formats, which are commonly used in astronomy, are also supported.

```C++
Angle ang_deg = Degree(123.456);
Angle ang_rad = Radian(2.1547136813);
Angle ang_hms = Hms(8, 13, 49.44);
Angle ang_dms = Dms(123, 27, 21.6);
```

### 3.1 Convert angle unit

Angle unit conversion calls a dedicated member.

```C++
Angle ang = Degree(123.456);

std::cout << ang.radians() << std::endl;
std::cout << ang.arcmins() << std::endl;
```

### 3.2 Arithmetic evaluation

Standard arithmetic, relational, and equality operators are supported.
Trigonometric functions can be used without regard to angular units.

```C++
std::cout << Dms(124, 27, 21.6) - Degree(1) << std::endl;
std::cout << Degree(90).sin() << std::endl;
std::cout << Angle::acos(0.5).degrees() << std::endl;
```

## 4. Read TLE data

TLE are loaded using the `Tle` class, and the TLE format is as follows

```
AAAAAAAAAAAAAAAAAAAAAAAA
1 NNNNNU NNNNNAAA NNNNN.NNNNNNNN +.NNNNNNNN +NNNNN-N +NNNNN-N N NNNNN
2 NNNNN NNN.NNNN NNN.NNNN NNNNNNN NNN.NNNN NNN.NNNN NN.NNNNNNNNNNNNNN
```

Although TLE object names must be 24 characters long by default, the library is very flexible and can read TLEs that are less than 24 characters long.  
The first and second lines, which are the body of the TLE, must be exactly 69 characters long.  
For detailed TLE format, see [here](https://celestrak.org/NORAD/documentation/tle-fmt.php).

### 4.1 Read from string

When converting from a string, enter the string line by line.  
The TLE object name does not have to be present, in which case the NORAD ID becomes the object name.

```C++
std::string tle_name = "KOYOH                   ";
std::string tle_line1 = "1 39769U 14029D   23333.77377284  .00001207  00000+0  15105-3 0  9994";
std::string tle_line2 = "2 39769  97.6430  38.1027 0002146 158.2419  18.6200 15.03069831513153";

Tle tle(tle_name, tle_line1, tle_line2);

std::cout << tle << std::endl;
```

### 4.2 Read from stream

You can also input data from the stream.

```C++
std::ifstream ifs("KOYOH.tle");
Tle tle(ifs);

std::cout << tle << std::endl;
```

The latest TLE can be obtained from [CelesTrak](https://celestrak.com/NORAD/elements/).

## 5. Create SGP4/SDP4 model and propagate orbital element

The `OrbitalPropagator` class is used to create an SGP4/SDP4 model based on TLE to predict orbital elements.  
The output is a set of Cartesian orbital elements (`CartesianOrbitalElements`) with position and velocity in the ECI coordinate system.

### 5.1. instantaneous element

To predict an orbital element at a given time, use the DateTime class to specify the time.

```C++
DateTime dt("2023-12-03T00:00:00");
OrbitalPropagator op(tle);

auto r = op.trackFlightObject(dt);

std::cout << r << std::endl;
```

### 5.2 Time series element

To predict the orbital elements of a time series, use the `DateTime` class arithmetic operations for time evolution.  
Of course, members such as `addSeconds` can also be used.

```C++
DateTime start_dt("2023-12-03T00:00:00");
DateTime end_dt("2023-12-04T00:00:00");
OrbitalPropagator op(tle);

for (DateTime dt = start_dt; dt < end_dt; dt += Minutes(1)) {
    auto r = op.trackFlightObject(dt);
    std::cout << r.toString() << std::endl;
}
```

## 6. Convert Cartesian orbital elements to Keplerian orbital elements

You can convert Cartesian orbital elements to Keplerian orbital elements (`KeplerianOrbitalElements`) using `toKeplerianOrbitalElements` member of `CartesianOrbitalElements` class.

```C++
DateTime dt("2023-12-03T00:00:00");
OrbitalPropagator op(tle);

auto r = op.trackFlightObject(dt);
auto ke = r.toKeplerianOrbitalElements();

std::cout << ke << std::endl;
```

## 7. Position of the satellite as observed from the ground station

The `GroundObservation` class can be used to obtain the position of a satellite when observed from a ground station.  
The position information is obtained in the ground surface coordinate system (`Topocentric`).

```C++
DateTime dt("2023-12-03T00:00:00");
OrbitalPropagator op(tle);
GroundObservation go(Degree(35.681236), Degree(139.767125), 0.0);
auto r = op.trackFlightObject(dt);
auto p = go.lookUpPosition(r);

std::cout << p << std::endl;
```

## 8. Transform coordinate system

If time information is associated with the location information, the coordinate system can be transformed.  
The coordinate systems that can be mutually transformed are the ECI coordinate system (`Eci`), the ECEF coordinate system (`Ecef`), and the WGS84 coordinate system (`Wgs84`).  
Ecliptic (`EclipticSpherical`, `EclipticCartesian`) and equatorial (`EquatorialSpherical`, `EquatorialCartesian`) coordinate systems are also available as celestial coordinate systems.  
Celestial coordinate systems can be transformed through the Eci class.

```C++
Eci eci(DateTime("2023-12-03T00:00:00"), 3.60859e6,  4.01001e6, -4.37001e6);
std::cout << eci << std::endl;
std::cout << eci.toEcef() << std::endl;
std::cout << eci.toWgs84() << std::endl;
```

## 9. Prediction of the Sun position

The `SunPosition` class can be used to predict the position of the sun.  
The sun position can be obtained in the ecliptic and ECI coordinate systems.

```C++
DateTime dt("1998-02-26T17:29:27");
SunPosition sp(dt);

std::cout << sp.ecliptic() << std::endl;
std::cout << sp.eci() << std::endl;
```

## 10. Prediction of the Moon position

The `MoonPosition` class can be used to predict the position of the moon.  
The moon's position can be obtained in the ecliptic and ECI coordinate systems.

```C++
DateTime dt("1998-02-26T17:29:27");
MoonPosition mp(dt);

std::cout << mp.ecliptic() << std::endl;
std::cout << mp.eci() << std::endl;
```