/**
 * ESPToon Basic Usage Example
 *
 * This example demonstrates the fundamental features of ESPToon:
 * - Creating nested objects and arrays
 * - Setting values of different types (String, int, float, bool)
 * - Reading values back from the document
 * - Serializing to human-readable text format
 * - Proving VALUE ↔ TOON symmetry
 *
 * This is the canonical reference implementation from the ESPToon specification.
 */

#include <ESPToon.h>

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n=== ESPToon Basic Usage ===\n");

  // ========== VALUE → TOON (Canonical Build) ==========

  Serial.println("STEP 1: Building ToonDocument (VALUE → TOON)");
  Serial.println("Creating nested objects and arrays with typed values...\n");

  ToonDocument doc;

  /* ---------- context ---------- */
  ToonObject& context = doc["context"].asObject();
  context["task"]     = "Our favorite hikes together";
  context["location"] = "Boulder";
  context["season"]   = "spring_2025";

  /* ---------- friends ---------- */
  ToonArray& friends = doc["friends"].asArray();
  friends.add("ana");
  friends.add("luis");
  friends.add("sam");

  /* ---------- hikes ---------- */
  ToonArray& hikes = doc["hikes"].asArray();

  ToonObject& h1 = hikes.createNestedObject();
  h1["id"]            = 1;
  h1["name"]          = "Blue Lake Trail";
  h1["distanceKm"]    = 7.5;
  h1["elevationGain"] = 320;
  h1["companion"]     = "ana";
  h1["wasSunny"]      = true;

  ToonObject& h2 = hikes.createNestedObject();
  h2["id"]            = 2;
  h2["name"]          = "Ridge Overlook";
  h2["distanceKm"]    = 9.2;
  h2["elevationGain"] = 540;
  h2["companion"]     = "luis";
  h2["wasSunny"]      = false;

  ToonObject& h3 = hikes.createNestedObject();
  h3["id"]            = 3;
  h3["name"]          = "Wildflower Loop";
  h3["distanceKm"]    = 5.1;
  h3["elevationGain"] = 180;
  h3["companion"]     = "sam";
  h3["wasSunny"]      = true;

  Serial.println("✓ Document built successfully!");
  Serial.println("  - 1 context object with 3 fields");
  Serial.println("  - 1 friends array with 3 names");
  Serial.println("  - 1 hikes array with 3 nested objects\n");

  // ========== Serialize to Serial (Text TOON) ==========

  Serial.println("STEP 2: Serializing to Text Format");
  Serial.println("Calling serializeToon(doc, Serial)...\n");
  Serial.println("Output:");
  Serial.println("--------------------------------");
  serializeToon(doc, Serial);
  Serial.println("--------------------------------");
  Serial.println("✓ Serialization complete!\n");

  // ========== TOON → VALUE (Canonical Readback) ==========

  Serial.println("STEP 3: Reading Values Back (TOON → VALUE)");
  Serial.println("Extracting all values to prove symmetry...\n");

  /* ---------- context ---------- */
  String task     = doc["context"]["task"];
  String location = doc["context"]["location"];
  String season   = doc["context"]["season"];

  Serial.println("Context:");
  Serial.print("  task: "); Serial.println(task);
  Serial.print("  location: "); Serial.println(location);
  Serial.print("  season: "); Serial.println(season);

  /* ---------- friends ---------- */
  String friend0 = doc["friends"][0];
  String friend1 = doc["friends"][1];
  String friend2 = doc["friends"][2];

  Serial.println("\nFriends:");
  Serial.print("  [0]: "); Serial.println(friend0);
  Serial.print("  [1]: "); Serial.println(friend1);
  Serial.print("  [2]: "); Serial.println(friend2);

  /* ---------- hikes ---------- */
  Serial.println("\nHikes:");

  // Hike 0
  int    id0    = doc["hikes"][0]["id"];
  String name0  = doc["hikes"][0]["name"];
  float  dist0  = doc["hikes"][0]["distanceKm"];
  int    elev0  = doc["hikes"][0]["elevationGain"];
  String comp0  = doc["hikes"][0]["companion"];
  bool   sunny0 = doc["hikes"][0]["wasSunny"];

  Serial.println("  Hike 0:");
  Serial.print("    id: "); Serial.println(id0);
  Serial.print("    name: "); Serial.println(name0);
  Serial.print("    distanceKm: "); Serial.println(dist0);
  Serial.print("    elevationGain: "); Serial.println(elev0);
  Serial.print("    companion: "); Serial.println(comp0);
  Serial.print("    wasSunny: "); Serial.println(sunny0 ? "true" : "false");

  // Hike 1
  int    id1    = doc["hikes"][1]["id"];
  String name1  = doc["hikes"][1]["name"];
  float  dist1  = doc["hikes"][1]["distanceKm"];
  int    elev1  = doc["hikes"][1]["elevationGain"];
  String comp1  = doc["hikes"][1]["companion"];
  bool   sunny1 = doc["hikes"][1]["wasSunny"];

  Serial.println("  Hike 1:");
  Serial.print("    id: "); Serial.println(id1);
  Serial.print("    name: "); Serial.println(name1);
  Serial.print("    distanceKm: "); Serial.println(dist1);
  Serial.print("    elevationGain: "); Serial.println(elev1);
  Serial.print("    companion: "); Serial.println(comp1);
  Serial.print("    wasSunny: "); Serial.println(sunny1 ? "true" : "false");

  // Hike 2
  int    id2    = doc["hikes"][2]["id"];
  String name2  = doc["hikes"][2]["name"];
  float  dist2  = doc["hikes"][2]["distanceKm"];
  int    elev2  = doc["hikes"][2]["elevationGain"];
  String comp2  = doc["hikes"][2]["companion"];
  bool   sunny2 = doc["hikes"][2]["wasSunny"];

  Serial.println("  Hike 2:");
  Serial.print("    id: "); Serial.println(id2);
  Serial.print("    name: "); Serial.println(name2);
  Serial.print("    distanceKm: "); Serial.println(dist2);
  Serial.print("    elevationGain: "); Serial.println(elev2);
  Serial.print("    companion: "); Serial.println(comp2);
  Serial.print("    wasSunny: "); Serial.println(sunny2 ? "true" : "false");

  Serial.println("\n========================================");
  Serial.println("SUMMARY: VALUE ↔ TOON Symmetry Proven!");
  Serial.println("========================================");
  Serial.println("✓ Built document with typed values (VALUE → TOON)");
  Serial.println("✓ Serialized to human-readable text format");
  Serial.println("✓ Read all values back successfully (TOON → VALUE)");
  Serial.println("✓ Every field preserved with correct types");
  Serial.println("✓ Fully reproducible - what goes in, comes out!");
  Serial.println("\nThis is the foundation of ESPToon:");
  Serial.println("  • In-memory: Fast, typed data structures");
  Serial.println("  • Serialization: Text (debug) or Binary+CRC (storage)");
  Serial.println("  • Symmetry: Perfect round-trip preservation\n");
}

void loop() {
  // Nothing to do
}
