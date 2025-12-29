# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

**ESPToon** is an embedded-first, typed, hierarchical data notation system called **TOON (Typed Object Oriented Notation)**.

**Author:** Chinmay Nagar (chinmay@chipnexa.in)
**Organization:** Chipnexa
**GitHub:** https://github.com/nagarChinmay

### Purpose

TOON is optimized for MCU-class systems with:
- Low memory usage
- Deterministic behavior
- Offline-first operation
- Binary-safe persistent storage
- Human-readable debugging

### Target Platforms

All code must support Arduino framework for:
- ESP32 / ESP8266
- AVR (ATmega / ATtiny)
- ARM Cortex-M (STM32, NXP, RP2040)
- RISC-V MCUs

## Core Architecture

### ToonDocument

The in-memory container for TOON data. API follows this pattern:

```cpp
ToonDocument doc;
doc["device"]["id"] = "ESP32-01";
doc["network"]["retry"] = 3;
```

### Data Types

- **ToonObject**: Key-value hierarchical objects
- **ToonArray**: Ordered collections
- **Primitives**: String, int, float, bool

### Building Nested Structures (CRITICAL)

When building nested objects and arrays, **always use reference variables** with `.asObject()` and `.asArray()`:

```cpp
// CORRECT - Using references
ToonObject& context = doc["context"].asObject();
context["task"] = "Our favorite hikes together";
context["location"] = "Boulder";

ToonArray& friends = doc["friends"].asArray();
friends.add("ana");
friends.add("luis");
friends.add("sam");

ToonObject& nested = friends.createNestedObject();
nested["name"] = "John";
nested["age"] = 30;
```

**Why references are required:**
- Without `&`, you get a **copy** of the object/array
- Changes to the copy are **lost** - they don't affect the document
- With `&`, you modify the actual object/array **in the document**

**WRONG - Don't do this:**
```cpp
// ❌ This creates a COPY - changes are lost!
ToonObject context = doc["context"].asObject();
context["task"] = "value";  // Modifies the copy, NOT the document!

// ❌ This also creates a copy
ToonArray friends = doc["friends"].asArray();
friends.add("ana");  // Added to copy, NOT the document!
```

### Canonical Example (BasicUsage)

The [examples/BasicUsage/BasicUsage.ino](examples/BasicUsage/BasicUsage.ino) example is the golden reference. See [prompt.txt](prompt.txt) lines 74-182 for the specification showing:
- Building nested objects and arrays
- Symmetric VALUE ↔ TOON conversion
- All supported data types

## Serialization Rules (CRITICAL)

### Unified API

```cpp
serializeToon(ToonDocument&, Target);
deserializeToon(ToonDocument&, Source);
```

**The Target/Source object determines the format automatically. Never use manual flags or version parameters.**

### Format Selection

| Target/Source Type | Storage Format |
|-------------------|----------------|
| `Serial`, `Stream` | Text TOON |
| `String` | Text TOON |
| `EEPROMStream` | Binary TOON + CRC |
| `File` (SPIFFS/LittleFS) | Binary TOON + CRC |
| `ArduinoJsonDocument` | JSON |

### Examples

**EEPROM (binary + CRC):**
```cpp
EEPROMStream eeprom(0, 512);
serializeToon(doc, eeprom);
eeprom.commit();  // Write to EEPROM

// CRITICAL: Reset position before reading
eeprom.reset();

ToonDocument doc2;
deserializeToon(doc2, eeprom);
```

**SPIFFS:**
```cpp
File f = SPIFFS.open("/data.toon", "w");
serializeToon(doc, f);
f.close();
```

**Serial Debug:**
```cpp
serializeToon(doc, Serial);
```

**ArduinoJson Interop:**
```cpp
DynamicJsonDocument json(1024);
serializeToon(doc, json);
```

## Design Constraints

### Memory Safety
- Size-bounded documents
- CRC check before parsing
- Graceful failure on corruption
- No crashes from invalid data

### Forbidden Patterns
- No format flags in serialization calls
- No version fields
- No CRC bypass
- Never apply corrupted data

## Implementation Principles

> **CRC protects integrity**
> **Binary protects memory**
> **TOON protects stability**

When implementing features:
1. Maintain VALUE ↔ TOON symmetry (see canonical example)
2. Let the Target/Source object determine serialization format
3. Always validate with CRC for persistent storage
4. Optimize for embedded constraints (heap/stack)
5. Ensure deterministic behavior for real-time systems

## Development Commands

### Building Examples (PlatformIO)

Create a `platformio.ini` file in the project root to build examples:

```ini
[env:esp32]
platform = espressif32
board = esp32dev
framework = arduino
lib_deps =
    ArduinoJson@^6.21.0

[env:esp32c3]
platform = espressif32
board = esp32-c3-devkitm-1
framework = arduino
lib_deps =
    ArduinoJson@^6.21.0

[env:esp32s3]
platform = espressif32
board = esp32-s3-devkitc-1
framework = arduino
lib_deps =
    ArduinoJson@^6.21.0

[env:esp8266]
platform = espressif8266
board = nodemcuv2
framework = arduino
lib_deps =
    ArduinoJson@^6.21.0
```

Build commands:
```bash
pio run -e esp32              # Build for ESP32
pio run -e esp32c3            # Build for ESP32-C3
pio run -e esp32s3            # Build for ESP32-S3
pio run -e esp8266            # Build for ESP8266
pio run -t upload -e esp32    # Upload to ESP32
```

### Testing with Arduino IDE

1. Open any example from `examples/` folder
2. Select board: Tools → Board → ESP32/ESP8266
3. Compile and upload

### Library Structure

```
ESPToon/
├── src/                      # Core library source
│   ├── ESPToon.h            # Main include file
│   ├── ToonValue.*          # Value type system
│   ├── ToonObject.*         # Object container
│   ├── ToonArray.*          # Array container
│   ├── ToonDocument.*       # Document root
│   ├── ToonSerializer.*     # Serialization
│   ├── ToonDeserializer.cpp # Deserialization
│   └── EEPROMStream.*       # EEPROM helper
├── examples/                # Example sketches
├── library.json             # PlatformIO manifest
├── library.properties       # Arduino manifest
└── keywords.txt             # Arduino syntax highlighting
```
