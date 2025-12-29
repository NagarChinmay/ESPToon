/**
 * Example: SPIFFS/LittleFS Persistence with Binary TOON + CRC
 *
 * Demonstrates:
 * - Saving TOON document to filesystem in binary format with CRC
 * - Loading TOON document from filesystem with CRC validation
 * - File-based configuration storage
 * - Works with both SPIFFS and LittleFS
 *
 * Supports: ESP32 (all variants), ESP8266
 */

#include <ESPToon.h>

#ifdef ESP32
  #include <SPIFFS.h>
  // Or use: #include <LittleFS.h>
  #define FILESYSTEM SPIFFS
  // Or use: #define FILESYSTEM LittleFS
#elif defined(ESP8266)
  #include <FS.h>
  #define FILESYSTEM SPIFFS
#endif

const char* CONFIG_FILE = "/config.toon";

void saveToFile() {
  Serial.println("\n=== Saving to File ===");

  ToonDocument doc;

  // Create application configuration
  doc["app"]["name"] = "ESPToon Demo";
  doc["app"]["version"] = "1.0.0";
  doc["app"]["bootCount"] = 1;

  doc["wifi"]["ssid"] = "MyNetwork";
  doc["wifi"]["password"] = "MyPassword123";
  doc["wifi"]["hostname"] = "esptoon-device";

  doc["mqtt"]["enabled"] = true;
  doc["mqtt"]["server"] = "mqtt.example.com";
  doc["mqtt"]["port"] = 1883;
  doc["mqtt"]["topic"] = "home/sensor/data";

  ToonArray& endpoints = doc["api"]["endpoints"].asArray();
  endpoints.add("https://api1.example.com");
  endpoints.add("https://api2.example.com");

  Serial.println("Document created:");
  serializeToon(doc, Serial);
  Serial.println();

  // Save to file (binary + CRC)
  File file = FILESYSTEM.open(CONFIG_FILE, "w");
  if (file) {
    size_t written = serializeToon(doc, file);
    file.close();

    Serial.print("✓ Written to file: ");
    Serial.print(written);
    Serial.println(" bytes");
  } else {
    Serial.println("✗ Failed to open file for writing");
  }
}

void loadFromFile() {
  Serial.println("\n=== Loading from File ===");

  ToonDocument doc;

  // Load from file
  File file = FILESYSTEM.open(CONFIG_FILE, "r");
  if (file) {
    if (deserializeToon(doc, file)) {
      file.close();

      Serial.println("✓ Successfully loaded and CRC validated");
      Serial.println("\nLoaded document:");
      serializeToon(doc, Serial);
      Serial.println();

      // Read back values
      String appName = doc["app"]["name"];
      String appVersion = doc["app"]["version"];
      int bootCount = doc["app"]["bootCount"];

      String ssid = doc["wifi"]["ssid"];
      String hostname = doc["wifi"]["hostname"];

      bool mqttEnabled = doc["mqtt"]["enabled"];
      String mqttServer = doc["mqtt"]["server"];
      int mqttPort = doc["mqtt"]["port"];

      String endpoint0 = doc["api"]["endpoints"][0];
      String endpoint1 = doc["api"]["endpoints"][1];

      Serial.println("Parsed values:");
      Serial.print("  App: "); Serial.print(appName);
      Serial.print(" v"); Serial.print(appVersion);
      Serial.print(" (boot #"); Serial.print(bootCount); Serial.println(")");

      Serial.print("  WiFi: "); Serial.print(ssid);
      Serial.print(" as "); Serial.println(hostname);

      Serial.print("  MQTT: ");
      Serial.print(mqttEnabled ? "enabled" : "disabled");
      Serial.print(" ("); Serial.print(mqttServer);
      Serial.print(":"); Serial.print(mqttPort); Serial.println(")");

      Serial.println("  API Endpoints:");
      Serial.print("    [0]: "); Serial.println(endpoint0);
      Serial.print("    [1]: "); Serial.println(endpoint1);

    } else {
      file.close();
      Serial.println("✗ Failed to deserialize or CRC validation failed!");
    }
  } else {
    Serial.println("✗ Failed to open file for reading");
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n=== ESPToon SPIFFS Persistence Example ===");

  // Initialize filesystem
  if (!FILESYSTEM.begin(true)) {
    Serial.println("✗ Failed to mount filesystem");
    return;
  }
  Serial.println("✓ Filesystem mounted");

  // First: save data
  saveToFile();

  // Second: load data
  loadFromFile();

  Serial.println("\n=== Example Complete ===");
}

void loop() {
  // Nothing to do
}
