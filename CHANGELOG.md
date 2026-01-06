# Changelog

All notable changes to ESPToon will be documented in this file.

## [1.0.1] - 2026-01-06

### Fixed
- **Arduino IDE Compatibility** - Fixed compilation errors that occurred in Arduino IDE
  - Added missing `#include <cstring>` for memcpy() in ToonSerializer.cpp and ToonDeserializer.cpp
  - Added missing `#include <cctype>` for isspace() in ToonDeserializer.cpp
  - Changed `min()` to `std::min()` in ToonSerializer.cpp for proper namespace resolution
  - Fixed platform-specific header includes in ToonSerializer.h (FS.h now only included when ESP32/ESP8266 detected)
  - Added C++17 feature detection with C++11/C++14 fallback for `__has_include()` macro
  - Fixed ESP8266 EEPROM compatibility (EEPROM.begin() and EEPROM.commit() API differences)
  - Added explicit `#include <cstdint>` to ToonValue.h for better type safety

### Changed
- Updated library description to mention Arduino IDE, PlatformIO, and Arduino CLI compatibility
- Added compatibility section to README

### Tested
- ✅ PlatformIO + ESP32 (GCC 8.4.0)
- ✅ PlatformIO + ESP8266 (GCC 10.3.0)
- ✅ Arduino CLI + ESP8266 (GCC 10.3.0)

## [1.0.0] - 2025-12-XX

### Added
- Initial release
- ToonDocument, ToonObject, ToonArray, ToonValue core classes
- Binary serialization with CRC16 validation
- Text serialization for debugging
- EEPROM persistence support
- SPIFFS/LittleFS file persistence
- Optional ArduinoJson interoperability
- Comprehensive examples
- Full documentation
