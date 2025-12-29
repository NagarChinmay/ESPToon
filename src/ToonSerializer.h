#ifndef TOON_SERIALIZER_H
#define TOON_SERIALIZER_H

#include "ToonDocument.h"
#include <Stream.h>
#include <FS.h>

#ifdef ESP32
#include <SPIFFS.h>
#include <LittleFS.h>
#elif defined(ESP8266)
#include <FS.h>
#endif

// Optional ArduinoJson support - only enabled if user has installed it
#if __has_include(<ArduinoJson.h>)
#include <ArduinoJson.h>
#define ESPTOON_HAS_ARDUINOJSON 1
#endif

// CRC16 calculation
uint16_t calculateCRC16(const uint8_t* data, size_t length);

// Text TOON serialization
size_t serializeToonText(const ToonDocument& doc, Print& output);
size_t serializeToonText(const ToonDocument& doc, String& output);

// Binary TOON serialization (with CRC)
size_t serializeToonBinary(const ToonDocument& doc, Print& output);
size_t serializeToonBinary(const ToonDocument& doc, uint8_t* buffer, size_t bufferSize);

// ArduinoJson serialization (only available if ArduinoJson is installed)
#ifdef ESPTOON_HAS_ARDUINOJSON
bool serializeToonJson(const ToonDocument& doc, JsonDocument& jsonDoc);
#endif

// Unified serialization API - overloaded functions (target type determines format)
size_t serializeToon(const ToonDocument& doc, Print& target);
size_t serializeToon(const ToonDocument& doc, String& target);

#ifdef ESPTOON_HAS_ARDUINOJSON
size_t serializeToon(const ToonDocument& doc, JsonDocument& target);
#endif

#ifdef ESP32
size_t serializeToon(const ToonDocument& doc, fs::File& target);
#elif defined(ESP8266)
size_t serializeToon(const ToonDocument& doc, File& target);
#endif

// Text TOON deserialization
bool deserializeToonText(ToonDocument& doc, Stream& input);
bool deserializeToonText(ToonDocument& doc, const String& input);
bool deserializeToonText(ToonDocument& doc, const char* input);

// Binary TOON deserialization (with CRC validation)
bool deserializeToonBinary(ToonDocument& doc, Stream& input);
bool deserializeToonBinary(ToonDocument& doc, const uint8_t* buffer, size_t length);

// ArduinoJson deserialization (only available if ArduinoJson is installed)
#ifdef ESPTOON_HAS_ARDUINOJSON
bool deserializeToonJson(ToonDocument& doc, const JsonDocument& jsonDoc);
#endif

// Unified deserialization API - overloaded functions (source type determines format)
bool deserializeToon(ToonDocument& doc, String& source);
bool deserializeToon(ToonDocument& doc, const char*& source);
bool deserializeToon(ToonDocument& doc, Stream& source);

#ifdef ESPTOON_HAS_ARDUINOJSON
bool deserializeToon(ToonDocument& doc, JsonDocument& source);
#endif

#ifdef ESP32
bool deserializeToon(ToonDocument& doc, fs::File& source);
#elif defined(ESP8266)
bool deserializeToon(ToonDocument& doc, File& source);
#endif

// Internal serialization helpers
namespace ToonSerializer {
    void serializeValue(const ToonValue& value, String& output);
    void serializeObject(const ToonObject& obj, String& output, int indent = 0);
    void serializeArray(const ToonArray& arr, String& output, int indent = 0);
}

#endif // TOON_SERIALIZER_H
