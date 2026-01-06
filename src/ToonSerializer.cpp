#include "ToonSerializer.h"
#include "EEPROMStream.h"
#include <cstring>  // For memcpy()

// CRC16-CCITT implementation
uint16_t calculateCRC16(const uint8_t* data, size_t length) {
    uint16_t crc = 0xFFFF;
    for (size_t i = 0; i < length; i++) {
        crc ^= (uint16_t)data[i] << 8;
        for (uint8_t bit = 0; bit < 8; bit++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ 0x1021;
            } else {
                crc = crc << 1;
            }
        }
    }
    return crc;
}

// ============================================================================
// TEXT TOON SERIALIZATION
// ============================================================================

namespace ToonSerializer {

void serializeValue(const ToonValue& value, String& output) {
    switch (value.type()) {
        case ToonType::TOON_NULL:
            output += "null";
            break;
        case ToonType::TOON_BOOL:
            output += value.asBool() ? "true" : "false";
            break;
        case ToonType::TOON_INT:
            output += String(value.asInt());
            break;
        case ToonType::TOON_FLOAT:
            output += String(value.asFloat());
            break;
        case ToonType::TOON_STRING: {
            String str = value.asString();
            // Escape special characters
            output += "\"";
            for (size_t i = 0; i < str.length(); i++) {
                char c = str[i];
                if (c == '"') output += "\\\"";
                else if (c == '\\') output += "\\\\";
                else if (c == '\n') output += "\\n";
                else if (c == '\r') output += "\\r";
                else if (c == '\t') output += "\\t";
                else output += c;
            }
            output += "\"";
            break;
        }
        default:
            output += "null";
            break;
    }
}

void serializeArray(const ToonArray& arr, String& output, int indent) {
    size_t size = arr.size();

    // Check if simple array (all primitives)
    bool isSimple = true;
    for (size_t i = 0; i < size; i++) {
        const ToonValue& val = arr[i];
        if (val.isObject() || val.isArray()) {
            isSimple = false;
            break;
        }
    }

    if (isSimple) {
        // Inline format: [3]: value1,value2,value3
        output += "[" + String(size) + "]: ";
        for (size_t i = 0; i < size; i++) {
            if (i > 0) output += ",";
            serializeValue(arr[i], output);
        }
        output += "\n";
    } else {
        // Multi-line format for complex arrays
        output += "[" + String(size) + "]:\n";
        for (size_t i = 0; i < size; i++) {
            for (int j = 0; j < indent + 2; j++) output += " ";

            const ToonValue& val = arr[i];
            if (val.isObject()) {
                serializeObject(*val._getObject(), output, indent + 2);
            } else if (val.isArray()) {
                serializeArray(*val._getArray(), output, indent + 2);
            } else {
                serializeValue(val, output);
                output += "\n";
            }
        }
    }
}

void serializeObject(const ToonObject& obj, String& output, int indent) {
    const auto& pairs = obj._getPairs();

    for (size_t i = 0; i < pairs.size(); i++) {
        const auto& pair = pairs[i];

        // Indentation
        for (int j = 0; j < indent; j++) output += " ";

        // Key
        output += pair.key;

        const ToonValue& val = pair.value;

        if (val.isObject()) {
            output += ":\n";
            serializeObject(*val._getObject(), output, indent + 2);
        } else if (val.isArray()) {
            serializeArray(*val._getArray(), output, indent);
        } else {
            output += ": ";
            serializeValue(val, output);
            output += "\n";
        }
    }
}

} // namespace ToonSerializer

size_t serializeToonText(const ToonDocument& doc, Print& output) {
    String result;
    ToonSerializer::serializeObject(doc._getRoot(), result, 0);
    return output.print(result);
}

size_t serializeToonText(const ToonDocument& doc, String& output) {
    output = "";
    ToonSerializer::serializeObject(doc._getRoot(), output, 0);
    return output.length();
}

// ============================================================================
// BINARY TOON SERIALIZATION
// ============================================================================

namespace ToonBinary {

void writeValue(const ToonValue& value, std::vector<uint8_t>& buffer) {
    uint8_t type = (uint8_t)value.type();
    buffer.push_back(type);

    switch (value.type()) {
        case ToonType::TOON_NULL:
            break;
        case ToonType::TOON_BOOL: {
            buffer.push_back(value.asBool() ? 1 : 0);
            break;
        }
        case ToonType::TOON_INT: {
            int32_t val = value.asInt();
            buffer.push_back((val >> 24) & 0xFF);
            buffer.push_back((val >> 16) & 0xFF);
            buffer.push_back((val >> 8) & 0xFF);
            buffer.push_back(val & 0xFF);
            break;
        }
        case ToonType::TOON_FLOAT: {
            float fval = value.asFloat();
            uint32_t val;
            memcpy(&val, &fval, sizeof(float));
            buffer.push_back((val >> 24) & 0xFF);
            buffer.push_back((val >> 16) & 0xFF);
            buffer.push_back((val >> 8) & 0xFF);
            buffer.push_back(val & 0xFF);
            break;
        }
        case ToonType::TOON_STRING: {
            String str = value.asString();
            uint16_t len = str.length();
            buffer.push_back((len >> 8) & 0xFF);
            buffer.push_back(len & 0xFF);
            for (uint16_t i = 0; i < len; i++) {
                buffer.push_back(str[i]);
            }
            break;
        }
        case ToonType::TOON_OBJECT: {
            const ToonObject* obj = value._getObject();
            if (obj) {
                const auto& pairs = obj->_getPairs();
                uint16_t size = pairs.size();
                buffer.push_back((size >> 8) & 0xFF);
                buffer.push_back(size & 0xFF);

                for (const auto& pair : pairs) {
                    // Write key
                    uint16_t keyLen = pair.key.length();
                    buffer.push_back((keyLen >> 8) & 0xFF);
                    buffer.push_back(keyLen & 0xFF);
                    for (uint16_t i = 0; i < keyLen; i++) {
                        buffer.push_back(pair.key[i]);
                    }
                    // Write value
                    writeValue(pair.value, buffer);
                }
            }
            break;
        }
        case ToonType::TOON_ARRAY: {
            const ToonArray* arr = value._getArray();
            if (arr) {
                const auto& elements = arr->_getElements();
                uint16_t size = elements.size();
                buffer.push_back((size >> 8) & 0xFF);
                buffer.push_back(size & 0xFF);

                for (const auto& element : elements) {
                    writeValue(element, buffer);
                }
            }
            break;
        }
    }
}

} // namespace ToonBinary

size_t serializeToonBinary(const ToonDocument& doc, Print& output) {
    std::vector<uint8_t> buffer;

    // Magic header "TOON"
    buffer.push_back('T');
    buffer.push_back('O');
    buffer.push_back('O');
    buffer.push_back('N');

    // Version
    buffer.push_back(1);
    buffer.push_back(0);

    // Reserve space for CRC (2 bytes) and data length (2 bytes)
    buffer.push_back(0);  // CRC high byte (placeholder)
    buffer.push_back(0);  // CRC low byte (placeholder)
    buffer.push_back(0);  // Data length high byte (placeholder)
    buffer.push_back(0);  // Data length low byte (placeholder)

    size_t dataStartIndex = buffer.size();  // Index where data starts

    // Serialize document as object
    ToonValue rootValue;
    rootValue._setObject(new ToonObject(doc._getRoot()));
    ToonBinary::writeValue(rootValue, buffer);

    // Calculate data length
    uint16_t dataLength = buffer.size() - dataStartIndex;
    buffer[8] = (dataLength >> 8) & 0xFF;
    buffer[9] = dataLength & 0xFF;

    // Calculate CRC (of data only)
    uint16_t crc = calculateCRC16(buffer.data() + dataStartIndex, dataLength);
    buffer[6] = (crc >> 8) & 0xFF;
    buffer[7] = crc & 0xFF;

    // Write to output
    return output.write(buffer.data(), buffer.size());
}

size_t serializeToonBinary(const ToonDocument& doc, uint8_t* buffer, size_t bufferSize) {
    std::vector<uint8_t> tempBuffer;

    // Magic header "TOON"
    tempBuffer.push_back('T');
    tempBuffer.push_back('O');
    tempBuffer.push_back('O');
    tempBuffer.push_back('N');

    // Version
    tempBuffer.push_back(1);
    tempBuffer.push_back(0);

    // Reserve space for CRC (2 bytes) and data length (2 bytes)
    tempBuffer.push_back(0);  // CRC high byte
    tempBuffer.push_back(0);  // CRC low byte
    tempBuffer.push_back(0);  // Data length high byte
    tempBuffer.push_back(0);  // Data length low byte

    size_t dataStartIndex = tempBuffer.size();

    // Serialize document
    ToonValue rootValue;
    rootValue._setObject(new ToonObject(doc._getRoot()));
    ToonBinary::writeValue(rootValue, tempBuffer);

    // Calculate data length
    uint16_t dataLength = tempBuffer.size() - dataStartIndex;
    tempBuffer[8] = (dataLength >> 8) & 0xFF;
    tempBuffer[9] = dataLength & 0xFF;

    // Calculate CRC
    uint16_t crc = calculateCRC16(tempBuffer.data() + dataStartIndex, dataLength);
    tempBuffer[6] = (crc >> 8) & 0xFF;
    tempBuffer[7] = crc & 0xFF;

    // Copy to output buffer
    size_t copySize = std::min(tempBuffer.size(), bufferSize);
    memcpy(buffer, tempBuffer.data(), copySize);

    return copySize;
}

// ============================================================================
// ARDUINOJSON SERIALIZATION (Optional - only if ArduinoJson is installed)
// ============================================================================

#ifdef ESPTOON_HAS_ARDUINOJSON

namespace ToonJsonConverter {

void valueToJson(const ToonValue& value, JsonVariant variant) {
    switch (value.type()) {
        case ToonType::TOON_NULL:
            // variant is already null
            break;
        case ToonType::TOON_BOOL:
            variant.set(value.asBool());
            break;
        case ToonType::TOON_INT:
            variant.set(value.asInt());
            break;
        case ToonType::TOON_FLOAT:
            variant.set(value.asFloat());
            break;
        case ToonType::TOON_STRING:
            variant.set(value.asString());
            break;
        case ToonType::TOON_OBJECT: {
            JsonObject obj = variant.to<JsonObject>();
            const ToonObject* toonObj = value._getObject();
            if (toonObj) {
                const auto& pairs = toonObj->_getPairs();
                for (const auto& pair : pairs) {
                    valueToJson(pair.value, obj.createNestedObject(pair.key));
                }
            }
            break;
        }
        case ToonType::TOON_ARRAY: {
            JsonArray arr = variant.to<JsonArray>();
            const ToonArray* toonArr = value._getArray();
            if (toonArr) {
                const auto& elements = toonArr->_getElements();
                for (const auto& element : elements) {
                    valueToJson(element, arr.createNestedObject());
                }
            }
            break;
        }
    }
}

} // namespace ToonJsonConverter

bool serializeToonJson(const ToonDocument& doc, JsonDocument& jsonDoc) {
    jsonDoc.clear();
    JsonObject root = jsonDoc.to<JsonObject>();

    const auto& pairs = doc._getRoot()._getPairs();
    for (const auto& pair : pairs) {
        ToonJsonConverter::valueToJson(pair.value, root.createNestedObject(pair.key));
    }

    return true;
}

#endif // ESPTOON_HAS_ARDUINOJSON

// ============================================================================
// UNIFIED SERIALIZATION API
// ============================================================================

size_t serializeToon(const ToonDocument& doc, Print& target) {
    return serializeToonText(doc, target);
}

size_t serializeToon(const ToonDocument& doc, String& target) {
    return serializeToonText(doc, target);
}

size_t serializeToon(const ToonDocument& doc, EEPROMStream& target) {
    return serializeToonBinary(doc, target);
}

#ifdef ESPTOON_HAS_ARDUINOJSON
size_t serializeToon(const ToonDocument& doc, JsonDocument& target) {
    serializeToonJson(doc, target);
    return 1;
}
#endif

#ifdef ESP32
size_t serializeToon(const ToonDocument& doc, fs::File& target) {
    return serializeToonBinary(doc, target);
}
#elif defined(ESP8266)
size_t serializeToon(const ToonDocument& doc, File& target) {
    return serializeToonBinary(doc, target);
}
#endif
