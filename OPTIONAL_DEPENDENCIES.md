# Optional Dependencies in ESPToon

## Overview

ESPToon is designed to work **completely standalone** with zero dependencies. ArduinoJson support is **optional** and only enabled if you choose to install it.

## How It Works

The library uses C++ preprocessor directives to detect if ArduinoJson is available:

```cpp
#if __has_include(<ArduinoJson.h>)
#include <ArduinoJson.h>
#define ESPTOON_HAS_ARDUINOJSON 1
#endif
```

## What This Means for You

### Without ArduinoJson (Standalone)

✅ **Available Features:**
- ToonDocument, ToonObject, ToonArray, ToonValue
- Text TOON serialization (Serial, String)
- Binary TOON serialization with CRC16 (EEPROM, SPIFFS, LittleFS)
- All deserialization functions
- EEPROMStream helper
- Complete TOON functionality

❌ **Not Available:**
- `serializeToon(doc, JsonDocument)`
- `deserializeToon(doc, JsonDocument)`
- ArduinoJson_Interop example won't compile

### With ArduinoJson Installed

✅ **All features above PLUS:**
- JSON serialization: `serializeToon(doc, JsonDocument)`
- JSON deserialization: `deserializeToon(doc, JsonDocument)`
- Seamless TOON ↔ JSON conversion
- ArduinoJson_Interop example works

## Installation

### Standalone (No Dependencies)

**Arduino IDE:**
```
Just install ESPToon - you're done!
```

**PlatformIO:**
```ini
lib_deps =
    ESPToon
```

### With JSON Support

**Arduino IDE:**
```
1. Install ESPToon via Library Manager
2. Install ArduinoJson via Library Manager
```

**PlatformIO:**
```ini
lib_deps =
    ESPToon
    ArduinoJson@^6.21.0
```

## Examples Compatibility

| Example | Requires ArduinoJson? |
|---------|----------------------|
| BasicUsage | ❌ No |
| EEPROM_Persist | ❌ No |
| SPIFFS_Persist | ❌ No |
| Serial_Debug | ❌ No |
| ArduinoJson_Interop | ✅ Yes |

## Compile-Time Detection

You can check if ArduinoJson is available in your code:

```cpp
#include <ESPToon.h>

void setup() {
    Serial.begin(115200);

    #ifdef ESPTOON_HAS_ARDUINOJSON
    Serial.println("ArduinoJson support: ENABLED");
    #else
    Serial.println("ArduinoJson support: DISABLED (standalone mode)");
    #endif
}
```

## Why This Design?

1. **Zero Bloat** - Users who don't need JSON won't have the overhead
2. **Flexibility** - Choose your dependencies based on your needs
3. **Embedded-First** - Minimal footprint for constrained systems
4. **No Breaking Changes** - If you install ArduinoJson later, features just work

## Migration Guide

### Already Using ArduinoJson?

No changes needed! If ArduinoJson is installed, ESPToon will automatically detect and use it.

### Want to Remove ArduinoJson?

Simply uninstall ArduinoJson:
- Arduino IDE: Remove via Library Manager
- PlatformIO: Remove from `lib_deps`

Your TOON code will continue to work (except JSON-specific functions).

## Technical Details

The optional dependency is implemented using:
- `__has_include()` - C++17 feature for compile-time header detection
- `#ifdef` guards around JSON-specific code
- Template specialization with conditional compilation

This ensures:
- ✅ No runtime overhead
- ✅ No linker errors if ArduinoJson is missing
- ✅ Clean compile with or without the dependency
