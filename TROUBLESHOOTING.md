# ESPToon Troubleshooting Guide

## Arduino IDE Compilation Errors

### Error: `GLIBC_2.30 not found` or Python shared library errors

**Cause:** This is a known issue with Arduino IDE installed via Snap on Ubuntu/Debian systems. It's **not** an ESPToon library problem.

**Solutions:**

#### Option 1: Use PlatformIO (Recommended)

PlatformIO is more stable and offers better ESP32/ESP8266 support:

```bash
# Install PlatformIO
pip install platformio

# Navigate to ESPToon directory
cd /path/to/ESPToon

# Test compile an example
pio ci --lib="." --board=esp32doit-devkit-v1 examples/BasicUsage/BasicUsage.ino

# Or use the platformio.ini file
pio run -e esp32
pio run -t upload -e esp32
```

#### Option 2: Reinstall Arduino IDE (Not Snap)

**Remove Snap version:**
```bash
sudo snap remove arduino
```

**Install from official source:**
```bash
# Download latest AppImage from arduino.cc
wget https://downloads.arduino.cc/arduino-ide/arduino-ide_latest_Linux_64bit.AppImage
chmod +x arduino-ide_latest_Linux_64bit.AppImage
./arduino-ide_latest_Linux_64bit.AppImage
```

#### Option 3: Fix Snap Arduino IDE

If you must use Snap version:

```bash
# Update snap
sudo snap refresh arduino

# Try with classic confinement
sudo snap install arduino --classic
```

---

## ArduinoJson Related Issues

### Compile Error: `JsonDocument` not defined

**Cause:** You're trying to use JSON features without ArduinoJson installed.

**Solution:**

**Arduino IDE:**
1. Open Library Manager (Sketch → Include Library → Manage Libraries)
2. Search for "ArduinoJson"
3. Install version 6.21.0 or later

**PlatformIO:**
```ini
lib_deps =
    ESPToon
    ArduinoJson@^6.21.0
```

### I don't need JSON - how do I use ESPToon standalone?

**Good news:** ESPToon works perfectly without ArduinoJson!

Just don't install ArduinoJson, and avoid:
- `serializeToon(doc, JsonDocument)`
- `deserializeToon(doc, JsonDocument)`
- The `ArduinoJson_Interop` example

Everything else works:
- ✅ Text serialization (`Serial`, `String`)
- ✅ Binary serialization (`EEPROM`, `SPIFFS`)
- ✅ All other examples

---

## ESP32/ESP8266 Board Issues

### Board not found in Arduino IDE

**Install ESP32 boards:**
1. File → Preferences
2. Additional Board Manager URLs: `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
3. Tools → Board → Boards Manager
4. Search "esp32" and install

**Install ESP8266 boards:**
1. Additional Board Manager URLs: `http://arduino.esp8266.com/stable/package_esp8266com_index.json`
2. Tools → Board → Boards Manager
3. Search "esp8266" and install

---

## Memory Issues

### Sketch too large / Out of memory

**Check your variant:**
- ESP32: Should have plenty of flash (4-16MB typical)
- ESP8266: Limited flash (512KB-4MB)

**Solutions:**
1. Use binary serialization instead of text (smaller)
2. Clear documents when done: `doc.clear()`
3. Use heap monitoring:
   ```cpp
   Serial.println(ESP.getFreeHeap());
   ```

---

## File System Issues

### SPIFFS not mounting

**ESP32:**
```cpp
if (!SPIFFS.begin(true)) {  // true = format on fail
    Serial.println("SPIFFS mount failed");
}
```

**ESP8266:**
```cpp
if (!SPIFFS.begin()) {
    Serial.println("SPIFFS mount failed");
    SPIFFS.format();  // Format if needed
    SPIFFS.begin();
}
```

### LittleFS vs SPIFFS

**Use LittleFS for new projects** (more reliable):

```cpp
#include <LittleFS.h>
#define FILESYSTEM LittleFS

if (!FILESYSTEM.begin(true)) {
    Serial.println("FS mount failed");
}
```

---

## CRC Validation Failures

### deserializeToon returns false

**Possible causes:**
1. **Corrupted data** - EEPROM/Flash may be damaged
2. **Wrong address** - Reading from uninitialized memory
3. **Different data** - File wasn't created by ESPToon

**Debug steps:**

```cpp
EEPROMStream eeprom(0, 512);
ToonDocument doc;

if (!deserializeToon(doc, eeprom)) {
    Serial.println("CRC validation failed!");
    Serial.println("Possible causes:");
    Serial.println("  1. Data corrupted");
    Serial.println("  2. EEPROM not initialized");
    Serial.println("  3. Reading wrong address");

    // Try writing fresh data
    doc.clear();
    doc["test"] = "hello";
    serializeToon(doc, eeprom);
    eeprom.commit();
}
```

---

## Common Code Issues

### Cannot convert ToonObject to ToonArray (or vice versa)

**Wrong:**
```cpp
ToonArray arr = doc["items"];  // Compile error if items is Object
```

**Right:**
```cpp
ToonArray arr = doc["items"].to<ToonArray>();  // Converts or creates
```

### Values not persisting

**Missing commit:**
```cpp
EEPROMStream eeprom(0, 512);
serializeToon(doc, eeprom);
eeprom.commit();  // ← Don't forget this!
```

**Not closing file:**
```cpp
File f = SPIFFS.open("/data.toon", "w");
serializeToon(doc, f);
f.close();  // ← Must close!
```

---

## Getting Help

If you're still having issues:

1. **Check the examples** - Start with `BasicUsage`
2. **Run test script** - `./test_syntax.sh` to verify library structure
3. **Enable debug output:**
   ```cpp
   Serial.begin(115200);
   Serial.setDebugOutput(true);
   ```
4. **Report issue:**
   - GitHub: https://github.com/nagarChinmay/ESPToon/issues
   - Email: chinmay@chipnexa.in

Include in your report:
- Board type (ESP32/ESP8266/variant)
- Arduino IDE or PlatformIO version
- Full error message
- Minimal code example that reproduces the issue
