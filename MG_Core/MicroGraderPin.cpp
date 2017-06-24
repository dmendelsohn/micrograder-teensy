#include <Arduino.h>
#include "MicroGrader.h"
#include "MicroGraderPin.h"

#undef pinMode

#undef digitalRead
#undef digitalWrite

#undef analogReadResolution
#undef analogReadRes
#undef analogRead
#undef analogWriteResolution
#undef analogWriteRes
#undef analogWrite

MicroGraderPin PinWrapper; // definition of MicroGraderPin instance

MicroGraderPin::MicroGraderPin() {
    analog_read_res = ANALOG_READ_RES_DEFAULT;
    analog_write_res = ANALOG_WRITE_RES_DEFAULT;
}

void MicroGraderPin::enablePins(uint8_t pins[]) {
    num_pins = sizeof(pins)/sizeof(uint8_t);
    for (uint8_t i = 0; i < num_pins; i++) {
        enabled_pins[i] = pins[i];
    }
}

// The functions below are substitutes for GPIO functions
void MicroGraderPin::pinMode_(uint8_t pin, uint8_t mode) {
    pinMode(pin, mode);
    // Do nothing else, perhaps later we'll track this
}

int MicroGraderPin::digitalRead_(uint8_t pin) {
    if (MicroGrader.mg_mode == INACTIVE) {
        return digitalRead(pin);
    } else if (!isEnabled(pin)) {
        return digitalRead(pin); // Do real read if pin is not enabled in wrapper
    } else { // Either in TESTING or RECORDING mode
        req_buffer[0] = pin;
        if (MicroGrader.mg_mode == RECORDING) {
            req_buffer[1] = 1; // Flags
            uint8_t real_val = digitalRead(pin);
            req_buffer[2] = real_val;
            MicroGrader.sendMessage(MG_DIGITAL_READ,
                                    req_buffer, 3);
            return real_val;
        } else { // mg_mode == TESTING
            req_buffer[1] = 0; // Flags
            int data_bytes = MicroGrader.sendMessage(MG_DIGITAL_READ, 
                                                    req_buffer, 2,
                                                    resp_buffer, 1);
            if (data_bytes < 1) {
                MicroGrader.error(DATA_ERROR); // Hang forever, something broke
            }
            return resp_buffer[0];
        }       

    }

    req_buffer[0] = pin;
    int data_bytes = MicroGrader.sendMessage(MG_DIGITAL_READ, req_buffer, 1,
                                             resp_buffer, 1);
    if (data_bytes < 1) {
        MicroGrader.error(DATA_ERROR); // Hang forever, something broke
    } 
    return resp_buffer[0];
}

void MicroGraderPin::digitalWrite_(uint8_t pin, uint8_t val) {
    digitalWrite(pin, val); // Do actual write
    if (MicroGrader.mg_mode == INACTIVE) return; // No further action
    if (!isEnabled(pin)) return; // Return if this pin is not test-enabled

    req_buffer[0] = pin;
    req_buffer[1] = val;
    MicroGrader.sendMessage(MG_DIGITAL_WRITE, req_buffer, 2);
}


void MicroGraderPin::analogReadResolution_(uint32_t bits) {
    analogReadResolution(bits);
    analog_read_res = bits;
}

int MicroGraderPin::analogRead_(uint8_t pin) {
    if (MicroGrader.mg_mode == INACTIVE) {
        return analogRead(pin); // Regular read
    } else if (!isEnabled(pin)) {
        return analogRead(pin); // Do real read if pin is not enabled in wrapper
    } else { // Either in TESTING or RECORDING mode
        int32_t max_bin = pow(2,analog_read_res) - 1;
        req_buffer[0] = pin;
        *((int32_t *)(req_buffer + 2 + 0*sizeof(int32_t))) = 0; // Min bin is 0
        *((int32_t *)(req_buffer + 2 + 1*sizeof(int32_t))) = max_bin;
        *((int32_t *)(req_buffer + 2 + 2*sizeof(int32_t))) = 0; // Min is 0V
        *((int32_t *)(req_buffer + 2 + 3*sizeof(int32_t))) = 3300; // Max is 3.3V
        if (MicroGrader.mg_mode == RECORDING) {
            req_buffer[1] = 1; // Flags
            int real_val = analogRead(pin);
            *((int32_t *)(req_buffer + 2 + 4*sizeof(int32_t))) = real_val;
            MicroGrader.sendMessage(MG_ANALOG_READ,
                                    req_buffer, 2+5*sizeof(int32_t));
            return real_val;
        } else { // mg_mode == TESTING
            req_buffer[1] = 0; // Flags
            int data_bytes = MicroGrader.sendMessage(MG_ANALOG_READ, 
                                            req_buffer, 2+4*sizeof(int32_t),
                                            resp_buffer, sizeof(int32_t));
            if (data_bytes < 4) {
                MicroGrader.error(DATA_ERROR); // Hang forever, something broke
            }
            return *((int *)(resp_buffer)); // Return first four bytes as int
        }
    }
}

void MicroGraderPin::analogWriteResolution_(uint32_t bits) {
    analogWriteResolution(bits); // Do action for real
    analog_write_res = bits;
}

void MicroGraderPin::analogWrite_(uint8_t pin, int val) {
    analogWrite(pin, val); // Do actual write
    if (MicroGrader.mg_mode == INACTIVE) return; // No further action
    if (!isEnabled(pin)) return; // End early if pin is not enabled

    int32_t max_bin = pow(2,analog_write_res) - 1;
    req_buffer[0] = pin;
    *((int32_t *)(req_buffer + 1 + 0*sizeof(int32_t))) = 0; // Min bin is 0
    *((int32_t *)(req_buffer + 1 + 1*sizeof(int32_t))) = max_bin;
    *((int32_t *)(req_buffer + 1 + 2*sizeof(int32_t))) = 0; // Min is 0V
    *((int32_t *)(req_buffer + 1 + 3*sizeof(int32_t))) = 3300; // Max is 3.3V
    *((int32_t *)(req_buffer + 1 + 4*sizeof(int32_t))) = val;
    MicroGrader.sendMessage(MG_ANALOG_WRITE, 
                            req_buffer, 1+5*sizeof(int32_t));
}

bool MicroGraderPin::isEnabled(uint8_t pin) {
    for (uint8_t i = 0; i < num_pins; i++) {
        if (enabled_pins[i] == pin) {
            return true;
        }
    }
    return false;
}

#define pinMode PinWrapper.pinMode_

#define digitalRead PinWrapper.digitalRead_
#define digitalWrite PinWrapper.digitalWrite_

#define analogReadResolution PinWrapper.analogReadResolution_
#define analogReadRes PinWrapper.analogReadResolution_
#define analogRead PinWrapper.analogRead_
#define analogWriteResolution PinWrapper.analogWriteResolution_
#define analogWriteRes PinWrapper.analogWriteResolution_
#define analogWrite PinWrapper.analogWrite_