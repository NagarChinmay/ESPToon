/**
 * Example: Serial Debug Output
 *
 * Demonstrates:
 * - Human-readable text TOON format for debugging
 * - Direct serialization to Serial
 * - Inspecting nested structures
 * - Quick development and troubleshooting
 *
 * Supports: ESP32 (all variants), ESP8266
 */

#include <ESPToon.h>

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n=== ESPToon Serial Debug Example ===\n");

  ToonDocument doc;

  // ========== Build a complex structure ==========

  // System info
  doc["system"]["chip"] = "ESP32-S3";
  doc["system"]["cores"] = 2;
  doc["system"]["flashMB"] = 8;
  doc["system"]["psramMB"] = 2;

  // Memory status
  doc["memory"]["heap"]["free"] = 245760;
  doc["memory"]["heap"]["total"] = 327680;
  doc["memory"]["psram"]["free"] = 1966080;
  doc["memory"]["psram"]["total"] = 2097152;

  // WiFi config
  doc["wifi"]["ssid"] = "MyNetwork";
  doc["wifi"]["rssi"] = -67;
  doc["wifi"]["channel"] = 6;
  doc["wifi"]["connected"] = true;

  // Sensor readings (array of objects)
  ToonArray& sensors = doc["sensors"].asArray();

  ToonObject& temp = sensors.createNestedObject();
  temp["name"] = "Temperature";
  temp["value"] = 24.8;
  temp["unit"] = "C";
  temp["status"] = "ok";

  ToonObject& humid = sensors.createNestedObject();
  humid["name"] = "Humidity";
  humid["value"] = 58.3;
  humid["unit"] = "%";
  humid["status"] = "ok";

  ToonObject& pressure = sensors.createNestedObject();
  pressure["name"] = "Pressure";
  pressure["value"] = 1013.25;
  pressure["unit"] = "hPa";
  pressure["status"] = "ok";

  // GPIO configuration
  ToonArray& gpios = doc["gpio"]["outputs"].asArray();
  gpios.add(2);
  gpios.add(4);
  gpios.add(16);
  gpios.add(17);

  ToonArray& inputs = doc["gpio"]["inputs"].asArray();
  inputs.add(18);
  inputs.add(19);

  // ========== Display the entire structure ==========

  Serial.println("Complete System Status (TOON Format):");
  Serial.println("=====================================");
  serializeToon(doc, Serial);
  Serial.println("=====================================\n");

  // ========== Read and display specific values ==========

  Serial.println("Parsed System Information:");
  Serial.println("--------------------------");

  String chip = doc["system"]["chip"];
  int cores = doc["system"]["cores"];
  int flashMB = doc["system"]["flashMB"];

  Serial.print("Chip: "); Serial.print(chip);
  Serial.print(", Cores: "); Serial.print(cores);
  Serial.print(", Flash: "); Serial.print(flashMB); Serial.println(" MB");

  int heapFree = doc["memory"]["heap"]["free"];
  int heapTotal = doc["memory"]["heap"]["total"];
  int heapPercent = (heapFree * 100) / heapTotal;

  Serial.print("Heap: "); Serial.print(heapFree);
  Serial.print(" / "); Serial.print(heapTotal);
  Serial.print(" ("); Serial.print(heapPercent); Serial.println("% free)");

  String ssid = doc["wifi"]["ssid"];
  int rssi = doc["wifi"]["rssi"];
  bool connected = doc["wifi"]["connected"];

  Serial.print("WiFi: "); Serial.print(ssid);
  Serial.print(", RSSI: "); Serial.print(rssi);
  Serial.print(" dBm, Status: ");
  Serial.println(connected ? "Connected" : "Disconnected");

  Serial.println("\nSensor Readings:");
  for (size_t i = 0; i < 3; i++) {
    String name = doc["sensors"][i]["name"];
    float value = doc["sensors"][i]["value"];
    String unit = doc["sensors"][i]["unit"];
    String status = doc["sensors"][i]["status"];

    Serial.print("  "); Serial.print(name); Serial.print(": ");
    Serial.print(value); Serial.print(" "); Serial.print(unit);
    Serial.print(" ["); Serial.print(status); Serial.println("]");
  }

  Serial.println("\n✓ Text TOON format perfect for debugging!");
}

void loop() {
  // Nothing to do
}
