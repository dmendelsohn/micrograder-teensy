// Author: Daniel Mendelsohn
// Written for MicroGrader thesis, Summer 2017
//
// This library provides functions for interacting with the x86 MicroGrader application via USB

#ifndef MICROGRADER_H
#define MICROGRADER_H

// Constants
#define MG_VERSION "0.1"
#define LED_PIN 13

// Byte codes for system events
#define MG_INIT 0x00
#define MG_PRINT 0x01
#define MG_END 0x02

// Byte codes for GPIO events
#define MG_DIGITAL_READ 0x20
#define MG_DIGITAL_WRITE 0x21
#define MG_ANALOG_READ 0x22
#define MG_ANALOG_WRITE 0x23

// Byte codes for IMU events
#define MG_IMU_ACC 0x30
#define MG_IMU_GYRO 0x31
#define MG_IMU_MAG 0x32

// Byte codes for OLED events
#define MG_OLED_INIT 0x40
#define MG_OLED_FULL 0x41
#define MG_OLED_TILE 0x42

// Byte codes for GPS events
#define MG_GPS_FIX 0x50

// Byte codes for Wifi events
#define MG_WIFI_REQ 0x60
#define MG_WIFI_RESP 0x61

// Byte codes for responses
#define MG_ACK 0x80
#define MG_ERR 0x81


#include <WString.h>
#include <Arduino.h>
#include "MicroGraderSerial.h"
#include "MicroGraderPin.h"

typedef uint8_t code_t;
typedef uint32_t timestamp_t;
typedef uint16_t msg_size_t;

#define REQ_HEADER_SIZE (sizeof(code_t)+sizeof(timestamp_t)+sizeof(msg_size_t))
#define RESP_HEADER_SIZE (sizeof(code_t)+sizeof(msg_size_t))

enum MG_ErrorType {TIMEOUT_ERROR=0, RESP_ERROR=1, DATA_ERROR=2};

class MicroGraderCore { // Essentially a static class to wrap all communication
  public:
  #if TEST
    void begin() { begin(true); }
    void debug(String str) {debug(str, true);}
  #else
    void begin() { begin(false); }
    void debug(String str) {debug(str, false);}
  #endif

    uint16_t sendMessage(code_t, uint8_t *, msg_size_t);
    uint16_t sendMessage(code_t, uint8_t *, msg_size_t, uint8_t *, msg_size_t);
    void error(MG_ErrorType);

  private:
    void begin(bool);
    void debug(String, bool);

    uint8_t header_buffer[RESP_HEADER_SIZE];

    static uint32_t const HI_MILLIS[]; // blink times for error processing
    static uint32_t const LO_MILLIS[]; // blink times for error processing

};
extern MicroGraderCore MicroGrader; // declaration of MicroGraderCore instance


#if TEST
    #define Serial MGSerial  // Replace user Serial with SerialDummy
#endif

#endif // MICROGRADER_H