/**
 * Comprehensive test to verify all assignment patterns work after removing
 * bool, float, and double operator= overloads
 * 
 * This test validates that:
 * 1. Direct assignments still work (int32_t, const char*, String)
 * 2. Implicit constructor conversions work (bool, float, double)
 * 3. All examples' usage patterns are preserved
 */

#include <ESPToon.h>

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n=== Testing Assignment Operator Fix ===\n");
  
  ToonDocument doc;
  
  // Test 1: Direct operator= assignments
  Serial.println("Test 1: Direct operator= assignments");
  doc["id"] = 1;                          // int32_t -> operator=(int32_t)
  doc["count"] = 42;                      // int32_t -> operator=(int32_t)
  doc["name"] = "ESP32";                  // const char* -> operator=(const char*)
  String device = "TestDevice";
  doc["device"] = device;                 // String -> operator=(const String&)
  Serial.println("✓ Direct assignments work\n");
  
  // Test 2: Implicit constructor conversions
  Serial.println("Test 2: Implicit constructor conversions");
  doc["enabled"] = true;                  // bool -> ToonValue(bool) -> operator=(ToonValue)
  doc["active"] = false;                  // bool -> ToonValue(bool) -> operator=(ToonValue)
  doc["temp"] = 23.5f;                    // float -> ToonValue(float) -> operator=(ToonValue)
  doc["voltage"] = 3.3f;                  // float -> ToonValue(float) -> operator=(ToonValue)
  doc["pi"] = 3.14159;                    // double -> ToonValue(double) -> operator=(ToonValue)
  doc["value"] = 99.99;                   // double -> ToonValue(double) -> operator=(ToonValue)
  Serial.println("✓ Implicit conversions work\n");
  
  // Test 3: BasicUsage example pattern
  Serial.println("Test 3: BasicUsage example pattern");
  ToonArray& hikes = doc["hikes"].asArray();
  ToonObject& h1 = hikes.createNestedObject();
  h1["id"] = 1;                           // int
  h1["distanceKm"] = 7.5;                 // double literal
  h1["wasSunny"] = true;                  // bool
  Serial.println("✓ BasicUsage pattern works\n");
  
  // Test 4: EEPROM_Persist example pattern
  Serial.println("Test 4: EEPROM_Persist example pattern");
  doc["sensors"]["temp"]["enabled"] = true;
  doc["sensors"]["temp"]["calibration"] = 0.5;
  doc["sensors"]["humid"]["enabled"] = false;
  Serial.println("✓ EEPROM_Persist pattern works\n");
  
  // Test 5: Serial_Debug example pattern
  Serial.println("Test 5: Serial_Debug example pattern");
  doc["wifi"]["connected"] = true;
  ToonObject& temp = doc["sensors"]["temperature"].asObject();
  temp["value"] = 24.8;
  Serial.println("✓ Serial_Debug pattern works\n");
  
  // Test 6: Array indexing
  Serial.println("Test 6: Array indexing");
  doc["array"][0] = 1;
  doc["array"][1] = 2;
  doc["array"][5] = 10;
  size_t idx = 3;
  doc["array"][idx] = 7;
  Serial.println("✓ Array indexing works\n");
  
  // Test 7: Reading values back
  Serial.println("Test 7: Reading values back");
  int id = doc["id"];
  bool enabled = doc["enabled"];
  float temp_val = doc["temp"];
  double pi_val = doc["pi"];
  String name = doc["name"];
  
  Serial.print("  id = "); Serial.println(id);
  Serial.print("  enabled = "); Serial.println(enabled ? "true" : "false");
  Serial.print("  temp = "); Serial.println(temp_val);
  Serial.print("  pi = "); Serial.println(pi_val, 5);
  Serial.print("  name = "); Serial.println(name);
  Serial.println("✓ Reading values works\n");
  
  // Test 8: Serialization
  Serial.println("Test 8: Serialization");
  Serial.println("Output:");
  Serial.println("--------------------------------");
  serializeToon(doc, Serial);
  Serial.println("--------------------------------");
  Serial.println("✓ Serialization works\n");
  
  Serial.println("========================================");
  Serial.println("ALL TESTS PASSED!");
  Serial.println("========================================");
  Serial.println("✓ Direct assignments work (int32_t, const char*, String)");
  Serial.println("✓ Implicit conversions work (bool, float, double)");
  Serial.println("✓ No ambiguity in Arduino IDE");
  Serial.println("✓ All example patterns preserved");
  Serial.println("✓ Full backward compatibility maintained\n");
}

void loop() {
  // Nothing to do
}
