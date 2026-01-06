# Changelog

All notable changes to ESPToon will be documented in this file.

## [1.0.4] - 2026-01-07

### Fixed
- **CRITICAL: Windows 32-bit Duplicate Operator Declaration** - Fixed "cannot be overloaded" error on Windows
  - Removed explicit `operator=(int)`, `operator=(long)`, and other integer overloads
  - Replaced with SFINAE template that detects type differences at compile-time
  - Template automatically enables only for integer types that are different from int32_t
  - Resolves conflict on Windows where `int`, `long`, and `int32_t` are all the same type (4 bytes)
  - Works correctly on Linux/Mac where `long` is 8 bytes and different from `int32_t`

### Changed
- Updated library version to 1.0.4
- Moved integer type handling from explicit overloads to template-based approach
- Added `#include <type_traits>` for std::enable_if and std::is_same

### Technical Details
The root cause was that on Windows 32-bit platforms:
- `sizeof(int) == 4`
- `sizeof(long) == 4`
- `sizeof(int32_t) == 4`

All three types are identical, so declaring separate overloads caused duplicate function declarations. The template solution uses compile-time type checking to only enable the overload when the type is actually different from int32_t.

## [1.0.3] - 2026-01-07

### Fixed
- **Enhanced Cross-Platform Integer Type Support** - Added comprehensive integer type operator overloads
  - Added explicit `operator=(unsigned int value)` to handle unsigned integers
  - Added explicit `operator=(unsigned long value)` to handle unsigned long integers
  - Added explicit `operator=(short value)` to handle short integers
  - Added explicit `operator=(unsigned short value)` to handle unsigned short integers
  - Resolves ALL integer type ambiguities across Windows (MSVC), macOS (Clang), and Linux (GCC)
  - Ensures assignments like `value = 1`, `value = 1U`, `value = (short)1` all work without ambiguity

### Changed
- Updated library version to 1.0.3

## [1.0.2] - 2026-01-07

### Fixed
- **Windows Arduino IDE Ambiguous Overload** - Fixed "ambiguous overload for 'operator='" error on Windows Arduino IDE
  - Added explicit `operator=(int value)` overload that forwards to `operator=(int32_t value)`
  - Added explicit `operator=(long value)` overload that forwards to `operator=(int32_t value)`
  - Resolves compiler ambiguity between int32_t, bool, float, and double when using plain int literals like `value = 1`

### Changed
- Updated library version to 1.0.2

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
