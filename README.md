# Overview
TODO

# Setup
* Add MG_Core, MG_MPU9250, MG_U8g2, and MG_Wifi to your Arduino libraries folder
* Delete MPU9250, U8g2 and Wifi_S08_v2, if applicable
* Add the following lines at the top of .ino sketch

```c
#include "MicroGrader.h"
MG_Mode mode = INACTIVE; // INACTIVE, TESTING, or RECORDING
uint8_t test_pins[] = {0,1,2,3}; // array of ints representing pins numbers
```

* Add the following line at the beginning of `setup()`:

```c
MicroGrader.begin(mode, test_pins);
```

* Add the following line at the end of `setup()`:

```c
MicroGrader.debug("Start"); // Helpful, but not required, to mark end of setup()
```

# Usage
* INACTIVE mode
    * In this mode, the system should function as if the MicroGrader module
    weren't there.  There will be no interaction with the host at all.
* RECORDING mode
    * In this mode, the system will report all input readings and output
    commands to the host, as well as various system events (such as Wifi
    requests and responses).  The `MicroGrader.begin()` function will block
    until the host connects.  With regards to the `analogRead`, `analogWrite`,
    `digitalRead`, and `digitalWrite` functions, the system will only report if
    the pin number is in the `test_pins` array.  The functions of the `Serial`
    object won't do anything.  Using the `MicroGrader.debug(String)` function
    is helpful to send plain text messages to the host.
* TESTING mode
    * Same as RECORDING mode, except input readings work differently.  Instead
    of taking a real sensor measurement and reporting it to the host, the
    system queries the host for a virtual measurement.