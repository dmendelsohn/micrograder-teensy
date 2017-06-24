#ifndef MICROGRADER_SERIAL_DUMMY_H
#define MICROGRADER_SERIAL_DUMMY_H

#include "MicroGrader.h"
#include "Stream.h"

int mg_usb_available();
int mg_usb_read();
int mg_usb_peek();
void mg_usb_flush();
size_t mg_usb_write(uint8_t c);
size_t mg_usb_write(const uint8_t *buffer, size_t size);
bool mg_bool();

// Wrapped serial class; in TEST mode, uses of Serial will still compile, but
// will point to this dummy class instead.  We need exclusive control of
// USB serial for MicroGrader
class MicroGraderSerial : public Stream
{
  public:
    void begin(long);
    void end();
    virtual int available() { return mg_usb_available(); }
    virtual int read() { return mg_usb_read(); }
    virtual int peek() { return mg_usb_peek(); }
    virtual void flush() { mg_usb_flush(); }
    virtual size_t write(uint8_t c) { return mg_usb_write(c); }
    virtual size_t write(const uint8_t *buffer, size_t size) { 
        return mg_usb_write(buffer,size);
    }
    size_t write(unsigned long n);
    size_t write(long n);
    size_t write(unsigned int n);
    size_t write(int n);
    int availableForWrite();
    using Print::write;
    void send_now(void);
    uint32_t baud(void);
    uint8_t stopbits(void);
    uint8_t paritytype(void);
    uint8_t numbits(void);
    uint8_t dtr(void);
    uint8_t rts(void);
    operator bool() { return mg_bool(); }
};
extern MicroGraderSerial MGSerial; // declaration of SerialDummy instance

#endif