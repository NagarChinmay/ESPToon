#ifndef ESP_TOON_H
#define ESP_TOON_H

/**
 * ESPToon - Typed Object Oriented Notation for ESP32/ESP8266
 *
 * Author: Chinmay Nagar <chinmay@chipnexa.in>
 * Organization: Chipnexa
 *
 * A lightweight, typed, hierarchical data notation optimized for MCU-class systems.
 * Features:
 * - Low memory usage
 * - Deterministic behavior
 * - Offline-first operation
 * - Binary-safe persistent storage with CRC validation
 * - Human-readable debugging
 *
 * Supported Platforms:
 * - ESP32 (all variants: C2, C3, S2, S3)
 * - ESP8266
 */

#include "ToonValue.h"
#include "ToonObject.h"
#include "ToonArray.h"
#include "ToonDocument.h"
#include "ToonSerializer.h"
#include "EEPROMStream.h"

#endif // ESP_TOON_H
