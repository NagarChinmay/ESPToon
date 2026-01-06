# ESPToon

**Typed Object Oriented Notation (TOON)** - A lightweight, typed, hierarchical data format designed for embedded systems.

[![PlatformIO](https://img.shields.io/badge/PlatformIO-Library-orange)](https://platformio.org/)
[![Arduino](https://img.shields.io/badge/Arduino-Library-blue)](https://www.arduino.cc/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

## Features

- **Low Memory Footprint** - Optimized for MCU-class systems
- **Deterministic Behavior** - No dynamic allocations during serialization
- **Offline-First** - Works completely offline, no cloud dependencies
- **Binary-Safe Persistence** - CRC16-validated binary storage for EEPROM/Flash
- **Human-Readable Debug** - Text format for Serial debugging
- **Type-Safe** - Strongly typed values (bool, int, float, string, object, array)
- **ArduinoJson Compatible** - Optional seamless JSON interoperability
- **Zero Dependencies** - Works standalone, ArduinoJson is optional

## Supported Platforms

- **ESP32** - All variants (ESP32, ESP32-C2, ESP32-C3, ESP32-S2, ESP32-S3)
- **ESP8266**

## Compatibility

✅ **Fully tested and compatible with:**
- Arduino IDE 1.8.19+ and 2.x
- PlatformIO
- Arduino CLI

## Installation

### Arduino IDE

1. Download this repository as ZIP
2. In Arduino IDE: `Sketch` → `Include Library` → `Add .ZIP Library`
3. Select the downloaded ZIP file

### PlatformIO

Add to your `platformio.ini`:

```ini
lib_deps =
    ESPToon
```

Or install via PlatformIO Library Manager:
```bash
pio lib install ESPToon
```

### Optional: ArduinoJson Support

ESPToon works perfectly standalone. To enable JSON interoperability, install ArduinoJson:

**Arduino IDE:** Library Manager → Search "ArduinoJson" → Install

**PlatformIO:**
```ini
lib_deps =
    ESPToon
    ArduinoJson@^6.21.0
```

## Quick Start

```cpp
#include <ESPToon.h>

ToonDocument doc;

// Set values
doc["device"]["id"] = "ESP32-01";
doc["device"]["temp"] = 23.5;
doc["device"]["enabled"] = true;

// Arrays (use reference!)
ToonArray& sensors = doc["sensors"].asArray();
sensors.add("DHT22");
sensors.add("BMP280");

// Serialize to Serial (text format for debugging)
serializeToon(doc, Serial);

// Save to EEPROM (binary + CRC)
EEPROMStream eeprom(0, 512);
serializeToon(doc, eeprom);
eeprom.commit();

// Reset position before reading
eeprom.reset();

// Load from EEPROM
ToonDocument loaded;
deserializeToon(loaded, eeprom);

// Read values
String id = loaded["device"]["id"];
float temp = loaded["device"]["temp"];
bool enabled = loaded["device"]["enabled"];
```

## Core Concepts

### ToonDocument

The main container for TOON data:

```cpp
ToonDocument doc;
doc["key"] = "value";
```

### Data Types

- **Primitives**: `bool`, `int`, `float`, `String`
- **Containers**: `ToonObject`, `ToonArray`

### Nested Objects

**IMPORTANT:** Always use reference variables (`&`) when building nested structures:

```cpp
// CORRECT - Using reference
ToonObject& config = doc["config"].asObject();
config["ssid"] = "MyWiFi";
config["password"] = "secret123";

// WRONG - Creates a copy, changes are lost!
// ToonObject config = doc["config"].asObject();  // ❌ Don't do this!
```

### Arrays

**IMPORTANT:** Always use reference variables (`&`) when building arrays:

```cpp
// CORRECT - Using reference
ToonArray& items = doc["items"].asArray();
items.add("first");
items.add("second");
items.add(123);
items.add(45.6);

// Access values (no reference needed for reading)
String first = doc["items"][0];
int number = doc["items"][2];
```

**Why references are required:**
- Without `&`, you get a **copy** that is discarded
- With `&`, you modify the **actual** object/array in the document
- Only needed when **building** structures, not when **reading** values

## Serialization

The library automatically chooses the format based on the target:

| Target Type | Format | Requires |
|------------|---------|----------|
| `Serial`, `Stream` | Text TOON | Built-in |
| `String` | Text TOON | Built-in |
| `File` (SPIFFS/LittleFS) | Binary + CRC | Built-in |
| `EEPROMStream` | Binary + CRC | Built-in |
| `JsonDocument` | JSON | ArduinoJson |

### Examples

**Text (for debugging):**
```cpp
serializeToon(doc, Serial);
```

**Binary to SPIFFS:**
```cpp
File file = SPIFFS.open("/config.toon", "w");
serializeToon(doc, file);
file.close();
```

**EEPROM:**
```cpp
EEPROMStream eeprom(0, 512);
serializeToon(doc, eeprom);
eeprom.commit();

// To read back:
eeprom.reset();  // CRITICAL: Reset position before reading
ToonDocument doc2;
deserializeToon(doc2, eeprom);
```

**ArduinoJson (requires ArduinoJson library):**
```cpp
#include <ArduinoJson.h>  // Install separately

DynamicJsonDocument json(1024);
serializeToon(doc, json);
serializeJsonPretty(json, Serial);
```

## Deserialization

**From SPIFFS:**
```cpp
File file = SPIFFS.open("/config.toon", "r");
ToonDocument doc;
if (deserializeToon(doc, file)) {
    // CRC validated - data is good
}
file.close();
```

**From EEPROM:**
```cpp
EEPROMStream eeprom(0, 512);
eeprom.reset();  // CRITICAL: Reset position if reading after writing
ToonDocument doc;
if (deserializeToon(doc, eeprom)) {
    // CRC validated
}
```

**From ArduinoJson (requires ArduinoJson library):**
```cpp
#include <ArduinoJson.h>  // Install separately

DynamicJsonDocument json(1024);
deserializeJson(json, "{\"key\":\"value\"}");

ToonDocument doc;
deserializeToon(doc, json);
```

## Examples

The library includes comprehensive examples:

- **BasicUsage** - Fundamental features and VALUE ↔ TOON symmetry (start here!)
- **EEPROM_Persist** - Saving/loading configuration from EEPROM with CRC validation
- **SPIFFS_Persist** - File-based persistence with SPIFFS/LittleFS
- **ArduinoJson_Interop** - Converting between TOON and JSON formats
- **Serial_Debug** - Human-readable debugging and inspection

## CRC Validation

All binary serialization (EEPROM, SPIFFS) includes automatic CRC16 validation:

```cpp
// Save with CRC
File file = SPIFFS.open("/data.toon", "w");
serializeToon(doc, file);
file.close();

// Load with CRC validation
File file = SPIFFS.open("/data.toon", "r");
if (deserializeToon(doc, file)) {
    // ✓ CRC validated - data integrity confirmed
} else {
    // ✗ CRC failed - data corrupted
}
file.close();
```

## Memory Management

TOON uses dynamic allocation for flexibility but provides size control:

```cpp
ToonDocument doc;  // Grows as needed

// Keep structures small for embedded systems
// Use clear() to free memory when done
doc.clear();
```

## API Reference

### ToonDocument

```cpp
ToonDocument doc;

// Access
ToonValue& doc[const String& key];
ToonValue& doc[const char* key];

// Methods
void clear();
size_t size();
bool containsKey(const String& key);
```

### ToonObject

```cpp
ToonObject obj;

// Access
ToonValue& obj[const String& key];

// Methods
bool containsKey(const String& key);
bool remove(const String& key);
void clear();
size_t size();

// Create nested
ToonObject& obj.createNestedObject(const String& key);
ToonArray& obj.createNestedArray(const String& key);
```

### ToonArray

```cpp
ToonArray arr;

// Access
ToonValue& arr[size_t index];

// Methods
ToonValue& add();
ToonValue& add(value);  // bool, int, float, String
ToonObject& createNestedObject();
ToonArray& createNestedArray();
bool remove(size_t index);
void clear();
size_t size();
```

### ToonValue

```cpp
ToonValue val;

// Type checking
bool isNull();
bool isBool();
bool isInt();
bool isFloat();
bool isString();
bool isObject();
bool isArray();

// Conversion
bool asBool();
int32_t asInt();
float asFloat();
String asString();

// Object/Array access (returns references - use with &)
ToonObject& asObject();
ToonArray& asArray();
const ToonObject& asObject() const;
const ToonArray& asArray() const;
```

## Design Principles

1. **CRC protects integrity** - Binary storage always includes CRC16
2. **Binary protects memory** - Compact storage for constrained systems
3. **TOON protects stability** - Type-safe, deterministic behavior
4. **Target determines format** - No manual flags needed

## Author

**Chinmay Nagar**
Founder & Engineer at [Chipnexa](https://chipnexa.in)
Email: chinmay@chipnexa.in
GitHub: [@nagarChinmay](https://github.com/nagarChinmay)

**Kevin Panchal**
Founder & Engineer at [Chipnexa](https://chipnexa.in)
Email: kevinpanchal2004@gmail.com
GitHub: [@KevinPanchal2444](https://github.com/KevinPanchal2444)

## License

MIT License - See [LICENSE](LICENSE) file for details

## Contributing

Contributions are welcome! Please feel free to submit issues and pull requests.

## Links

- **GitHub**: https://github.com/nagarChinmay/ESPToon
- **Issues**: https://github.com/nagarChinmay/ESPToon/issues
- **Chipnexa**: https://chipnexa.in
