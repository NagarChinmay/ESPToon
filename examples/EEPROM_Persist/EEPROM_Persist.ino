/**
 * Example: EEPROM Persistence with Binary TOON + CRC
 *
 * Demonstrates:
 * - Saving TOON document to EEPROM in binary format with CRC
 * - Loading TOON document from EEPROM with CRC validation
 * - Graceful handling of corrupted data
 * - Safe persistence for embedded systems
 *
 * Supports: ESP32 (all variants), ESP8266
 */

#include <ESPToon.h>

const size_t EEPROM_ADDR = 0;
const size_t EEPROM_SIZE = 512;

void saveToEEPROM() {
  Serial.println("\n=== Saving to EEPROM ===");

  ToonDocument doc;

  // Create some device configuration
  doc["device"]["id"] = "ESP32-SENSOR-01";
  doc["device"]["type"] = "temperature";
  doc["device"]["version"] = 1;

  doc["network"]["ssid"] = "MyWiFi";
  doc["network"]["retry"] = 3;
  doc["network"]["timeout"] = 10000;

  doc["sensors"]["temp"]["enabled"] = true;
  doc["sensors"]["temp"]["interval"] = 60;
  doc["sensors"]["temp"]["calibration"] = 0.5;

  doc["sensors"]["humid"]["enabled"] = false;

  Serial.println("Document created:");
  serializeToon(doc, Serial);
  Serial.println();

  // Save to EEPROM (binary + CRC)
  EEPROMStream eeprom(EEPROM_ADDR, EEPROM_SIZE);
  size_t written = serializeToon(doc, eeprom);
  eeprom.commit();

  Serial.print("Written to EEPROM: ");
  Serial.print(written);
  Serial.println(" bytes");
}

void loadFromEEPROM() {
  Serial.println("\n=== Loading from EEPROM ===");

  ToonDocument doc;

  // Load from EEPROM
  EEPROMStream eeprom(EEPROM_ADDR, EEPROM_SIZE);

  if (deserializeToon(doc, eeprom)) {
    Serial.println("✓ Successfully loaded and CRC validated");
    Serial.println("\nLoaded document:");
    serializeToon(doc, Serial);
    Serial.println();

    // Read back values
    String deviceId = doc["device"]["id"];
    String deviceType = doc["device"]["type"];
    int deviceVersion = doc["device"]["version"];

    String ssid = doc["network"]["ssid"];
    int retry = doc["network"]["retry"];
    int timeout = doc["network"]["timeout"];

    bool tempEnabled = doc["sensors"]["temp"]["enabled"];
    int tempInterval = doc["sensors"]["temp"]["interval"];
    float tempCal = doc["sensors"]["temp"]["calibration"];

    Serial.println("Parsed values:");
    Serial.print("  Device: "); Serial.print(deviceId);
    Serial.print(" ("); Serial.print(deviceType);
    Serial.print(" v"); Serial.print(deviceVersion); Serial.println(")");

    Serial.print("  Network: "); Serial.print(ssid);
    Serial.print(" (retry="); Serial.print(retry);
    Serial.print(", timeout="); Serial.print(timeout); Serial.println("ms)");

    Serial.print("  Temp sensor: ");
    Serial.print(tempEnabled ? "enabled" : "disabled");
    Serial.print(" (interval="); Serial.print(tempInterval);
    Serial.print("s, cal="); Serial.print(tempCal); Serial.println(")");

  } else {
    Serial.println("✗ Failed to load or CRC validation failed!");
    Serial.println("  Data may be corrupted or not initialized.");
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n=== ESPToon EEPROM Persistence Example ===");

  // First run: save data
  saveToEEPROM();

  // Second run: load data
  loadFromEEPROM();

  Serial.println("\n=== Example Complete ===");
  Serial.println("Power cycle the device to test persistence!");
}

void loop() {
  // Nothing to do
}
