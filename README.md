# Overview
TODO

# Setup
* Add MG_Core, MG_MPU9250, MG_U8g2, and MG_Wifi to your Arduino libraries folder
* Delete MPU9250, U8g2 and Wifi_S08_v2, if applicable
* Add the following lines at the top of .ino sketch

    #include "MicroGrader.h"
    MG_Mode mode = INACTIVE;
    uint8_t test_pins[] = {BUTTON_PIN};

* TODO