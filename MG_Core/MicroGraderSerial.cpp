#include <Arduino.h>

#include "MicroGrader.h"
#include "MicroGraderSerial.h"

#undef Serial

MicroGraderSerial MGSerial; // definition of dummy Serial instance

int mg_usb_available() {
    if (MicroGrader.mg_mode == INACTIVE) {
        return Serial.available();
    } else {
        return 0;
    }
}

int mg_usb_read() {
    if (MicroGrader.mg_mode == INACTIVE) {
        return Serial.read();
    } else {
        return -1;
    }
}

int mg_usb_peek() {
    if (MicroGrader.mg_mode == INACTIVE) {
        return Serial.peek();
    } else {
        return -1;
    }
}

void mg_usb_flush() {
    if (MicroGrader.mg_mode == INACTIVE) {
        return Serial.flush();
    }
}

size_t mg_usb_write(uint8_t c) {
    if (MicroGrader.mg_mode == INACTIVE) {
        return Serial.write(c);
    } else {
        return 1;
    }
}

size_t mg_usb_write(const uint8_t *buffer, size_t size) {
    if (MicroGrader.mg_mode == INACTIVE) {
        return Serial.write(buffer, size);
    } else {
        return size;
    }
}

void MicroGraderSerial::begin(long x) {
    if (MicroGrader.mg_mode == INACTIVE) {
        Serial.begin(x);
        //pinMode(13, OUTPUT);
        //digitalWrite(13, HIGH);
        //Serial.blah();
    }
}

void MicroGraderSerial::end() {
    if (MicroGrader.mg_mode == INACTIVE) {
        Serial.end();
    }
}

size_t MicroGraderSerial::write(unsigned long n) {
    if (MicroGrader.mg_mode == INACTIVE) {
        return Serial.write(n);
    } else {
        return 1;
    }
}

size_t MicroGraderSerial::write(long n) {
    if (MicroGrader.mg_mode == INACTIVE) {
        return Serial.write(n);
    } else {
        return 1;
    }
}

size_t MicroGraderSerial::write(unsigned int n) {
    if (MicroGrader.mg_mode == INACTIVE) {
        return Serial.write(n);
    } else {
        return 1;
    }
}

size_t MicroGraderSerial::write(int n) {
    if (MicroGrader.mg_mode == INACTIVE) {
        return Serial.write(n);
    } else {
        return 1;
    }
}

int MicroGraderSerial::availableForWrite() {
    if (MicroGrader.mg_mode == INACTIVE) {
        return Serial.availableForWrite();
    } else {
        return 0;
    }
}

void MicroGraderSerial::send_now() {
    if (MicroGrader.mg_mode == INACTIVE) {
        Serial.send_now();
    }
}

uint32_t MicroGraderSerial::baud() {
    if (MicroGrader.mg_mode == INACTIVE) {
        return Serial.baud();
    } else {
        return 0;
    }
}

uint8_t MicroGraderSerial::stopbits() {
    if (MicroGrader.mg_mode == INACTIVE) {
        return Serial.stopbits();
    } else {
        return 1;
    }
}

uint8_t MicroGraderSerial::paritytype() {
    if (MicroGrader.mg_mode == INACTIVE) {
        return Serial.paritytype();
    } else {
        return 0;
    }
}

uint8_t MicroGraderSerial::numbits() {
    if (MicroGrader.mg_mode == INACTIVE) {
        return Serial.numbits();
    } else {
        return 8;
    }
}

uint8_t MicroGraderSerial::dtr() {
    if (MicroGrader.mg_mode == INACTIVE) {
        return Serial.dtr();
    } else {
        return 1;
    }
}

uint8_t MicroGraderSerial::rts() {
    if (MicroGrader.mg_mode == INACTIVE) {
        return Serial.rts();
    } else {
        return 1;
    }
}

bool mg_bool() {
    if (MicroGrader.mg_mode == INACTIVE) {
        return Serial;
    } else {
        return true;
    }
}

#define Serial MGSerial
