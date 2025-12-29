#ifndef EEPROM_STREAM_H
#define EEPROM_STREAM_H

#include <Arduino.h>
#include <Stream.h>

#ifdef ESP32
#include <EEPROM.h>
#elif defined(ESP8266)
#include <EEPROM.h>
#endif

class EEPROMStream : public Stream {
private:
    size_t _address;
    size_t _size;
    size_t _position;
    bool _writeMode;

public:
    EEPROMStream(size_t address, size_t size);

    // Stream interface
    int available() override;
    int read() override;
    int peek() override;
    size_t write(uint8_t byte) override;
    size_t write(const uint8_t* buffer, size_t size) override;

    // Additional methods
    void flush() override;
    void reset();
    void setWriteMode(bool mode);
    bool commit();
};

#endif // EEPROM_STREAM_H
