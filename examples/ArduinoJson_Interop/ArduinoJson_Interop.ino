/**
 * Example: ArduinoJson Interoperability
 *
 * Demonstrates:
 * - Converting TOON document to ArduinoJson
 * - Converting ArduinoJson to TOON document
 * - Seamless interop with existing JSON-based code
 * - Useful for REST APIs, MQTT payloads, etc.
 *
 * Supports: ESP32 (all variants), ESP8266
 */

#include <ESPToon.h>
#include <ArduinoJson.h>

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n=== ESPToon ↔ ArduinoJson Interop Example ===\n");

  // ========== Create TOON Document ==========

  ToonDocument toonDoc;

  toonDoc["sensor"]["type"] = "DHT22";
  toonDoc["sensor"]["location"] = "Living Room";

  toonDoc["reading"]["temperature"] = 23.5;
  toonDoc["reading"]["humidity"] = 65.2;
  toonDoc["reading"]["timestamp"] = 1640995200;

  ToonArray& alerts = toonDoc["alerts"].asArray();
  alerts.add("temp_high");
  alerts.add("humid_normal");

  Serial.println("Original TOON Document:");
  Serial.println("----------------------");
  serializeToon(toonDoc, Serial);
  Serial.println("----------------------\n");

  // ========== TOON → ArduinoJson ==========

  DynamicJsonDocument jsonDoc(1024);
  serializeToon(toonDoc, jsonDoc);

  Serial.println("Converted to ArduinoJson:");
  Serial.println("-------------------------");
  serializeJsonPretty(jsonDoc, Serial);
  Serial.println("\n-------------------------\n");

  // ========== ArduinoJson → TOON ==========

  // Create a different JSON document
  DynamicJsonDocument inputJson(512);
  inputJson["device"]["id"] = "ESP-001";
  inputJson["device"]["model"] = "ESP32-C3";
  inputJson["status"]["online"] = true;
  inputJson["status"]["uptime"] = 3600;

  JsonArray services = inputJson.createNestedArray("services");
  services.add("http");
  services.add("mqtt");
  services.add("websocket");

  Serial.println("Input ArduinoJson:");
  Serial.println("------------------");
  serializeJsonPretty(inputJson, Serial);
  Serial.println("\n------------------\n");

  // Convert to TOON
  ToonDocument convertedDoc;
  deserializeToon(convertedDoc, inputJson);

  Serial.println("Converted to TOON:");
  Serial.println("------------------");
  serializeToon(convertedDoc, Serial);
  Serial.println("------------------\n");

  // Read values from converted document
  String deviceId = convertedDoc["device"]["id"];
  String deviceModel = convertedDoc["device"]["model"];
  bool online = convertedDoc["status"]["online"];
  int uptime = convertedDoc["status"]["uptime"];

  Serial.println("Parsed values from converted TOON:");
  Serial.print("  Device: "); Serial.print(deviceId);
  Serial.print(" ("); Serial.print(deviceModel); Serial.println(")");
  Serial.print("  Status: "); Serial.print(online ? "online" : "offline");
  Serial.print(", uptime: "); Serial.print(uptime); Serial.println("s");

  Serial.println("\n✓ TOON ↔ ArduinoJson interop working perfectly!");
}

void loop() {
  // Nothing to do
}
