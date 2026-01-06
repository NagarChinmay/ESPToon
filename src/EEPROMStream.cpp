#include "EEPROMStream.h"

EEPROMStream::EEPROMStream(size_t address, size_t size)
    : _address(address), _size(size), _position(0), _writeMode(false) {
    #ifdef ESP32
        EEPROM.begin(_address + _size);
    #elif defined(ESP8266)
        // ESP8266 EEPROM library requires fixed size (max 4096 bytes)
        EEPROM.begin(4096);
    #endif
}

int EEPROMStream::available() {
    return _size - _position;
}

int EEPROMStream::read() {
    if (_position >= _size) {
        return -1;
    }
    uint8_t value = EEPROM.read(_address + _position);
    _position++;
    return value;
}

int EEPROMStream::peek() {
    if (_position >= _size) {
        return -1;
    }
    return EEPROM.read(_address + _position);
}

size_t EEPROMStream::write(uint8_t byte) {
    if (_position >= _size) {
        return 0;
    }
    EEPROM.write(_address + _position, byte);
    _position++;
    return 1;
}

size_t EEPROMStream::write(const uint8_t* buffer, size_t size) {
    size_t written = 0;
    for (size_t i = 0; i < size && _position < _size; i++) {
        EEPROM.write(_address + _position, buffer[i]);
        _position++;
        written++;
    }
    return written;
}

void EEPROMStream::flush() {
    // Nothing to do - EEPROM writes are immediate
}

void EEPROMStream::reset() {
    _position = 0;
}

void EEPROMStream::setWriteMode(bool mode) {
    _writeMode = mode;
    if (mode) {
        _position = 0;
    }
}

bool EEPROMStream::commit() {
    #ifdef ESP32
        return EEPROM.commit();
    #else
        // ESP8266 auto-commits on EEPROM.write()
        EEPROM.commit();
        return true;
    #endif
}
