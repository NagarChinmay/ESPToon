#include "ToonSerializer.h"
#include <vector>

// ============================================================================
// TEXT TOON DESERIALIZATION (Simplified parser)
// ============================================================================

namespace ToonParser {

String trim(const String& str) {
    int start = 0;
    int end = str.length() - 1;

    while (start <= end && isspace(str[start])) start++;
    while (end >= start && isspace(str[end])) end--;

    return str.substring(start, end + 1);
}

bool parseValue(const String& valueStr, ToonValue& value) {
    String trimmed = trim(valueStr);

    if (trimmed.length() == 0 || trimmed == "null") {
        // null value - already default
        return true;
    } else if (trimmed == "true") {
        value = true;
        return true;
    } else if (trimmed == "false") {
        value = false;
        return true;
    } else if (trimmed[0] == '"' && trimmed[trimmed.length() - 1] == '"') {
        // String value
        String str = trimmed.substring(1, trimmed.length() - 1);
        // Unescape
        str.replace("\\\"", "\"");
        str.replace("\\\\", "\\");
        str.replace("\\n", "\n");
        str.replace("\\r", "\r");
        str.replace("\\t", "\t");
        value = str;
        return true;
    } else if (trimmed.indexOf('.') >= 0) {
        // Float
        value = trimmed.toFloat();
        return true;
    } else {
        // Integer
        value = (int32_t)trimmed.toInt();
        return true;
    }
}

bool parseSimpleArray(const String& line, ToonArray& arr) {
    // Format: key[N]: val1,val2,val3
    int colonPos = line.indexOf(':');
    if (colonPos < 0) return false;

    String valuesStr = trim(line.substring(colonPos + 1));
    if (valuesStr.length() == 0) return true;

    // Split by comma
    int start = 0;
    for (int i = 0; i <= valuesStr.length(); i++) {
        if (i == valuesStr.length() || valuesStr[i] == ',') {
            String token = trim(valuesStr.substring(start, i));
            if (token.length() > 0) {
                ToonValue val;
                parseValue(token, val);
                arr.add(val);
            }
            start = i + 1;
        }
    }

    return true;
}

} // namespace ToonParser

bool deserializeToonText(ToonDocument& doc, const String& input) {
    doc.clear();

    std::vector<String> lines;
    int start = 0;

    // Split into lines
    for (int i = 0; i <= input.length(); i++) {
        if (i == input.length() || input[i] == '\n') {
            String line = input.substring(start, i);
            if (line.endsWith("\r")) {
                line = line.substring(0, line.length() - 1);
            }
            lines.push_back(line);
            start = i + 1;
        }
    }

    // Simple parser for basic key:value pairs
    for (const String& line : lines) {
        String trimmed = ToonParser::trim(line);
        if (trimmed.length() == 0) continue;

        int colonPos = trimmed.indexOf(':');
        if (colonPos < 0) continue;

        String key = ToonParser::trim(trimmed.substring(0, colonPos));
        String valueStr = ToonParser::trim(trimmed.substring(colonPos + 1));

        // Check for array notation key[N]
        int bracketPos = key.indexOf('[');
        if (bracketPos > 0) {
            String arrayKey = key.substring(0, bracketPos);
            ToonArray arr = doc[arrayKey].to<ToonArray>();

            if (valueStr.length() > 0) {
                ToonParser::parseSimpleArray(trimmed, arr);
            }
        } else if (valueStr.length() > 0) {
            // Simple value
            ToonValue val;
            ToonParser::parseValue(valueStr, val);
            doc[key] = val;
        }
    }

    return true;
}

bool deserializeToonText(ToonDocument& doc, const char* input) {
    return deserializeToonText(doc, String(input));
}

bool deserializeToonText(ToonDocument& doc, Stream& input) {
    String data;
    while (input.available()) {
        data += (char)input.read();
    }
    return deserializeToonText(doc, data);
}

// ============================================================================
// BINARY TOON DESERIALIZATION
// ============================================================================

namespace ToonBinaryReader {

bool readValue(const uint8_t*& ptr, const uint8_t* end, ToonValue& value) {
    if (ptr >= end) return false;

    uint8_t type = *ptr++;
    ToonType toonType = (ToonType)type;

    switch (toonType) {
        case ToonType::TOON_NULL:
            // value is already null
            break;

        case ToonType::TOON_BOOL:
            if (ptr >= end) return false;
            value = (*ptr++ != 0);
            break;

        case ToonType::TOON_INT: {
            if (ptr + 4 > end) return false;
            int32_t val = ((int32_t)ptr[0] << 24) |
                         ((int32_t)ptr[1] << 16) |
                         ((int32_t)ptr[2] << 8) |
                         ((int32_t)ptr[3]);
            ptr += 4;
            value = val;
            break;
        }

        case ToonType::TOON_FLOAT: {
            if (ptr + 4 > end) return false;
            uint32_t bits = ((uint32_t)ptr[0] << 24) |
                           ((uint32_t)ptr[1] << 16) |
                           ((uint32_t)ptr[2] << 8) |
                           ((uint32_t)ptr[3]);
            ptr += 4;
            float fval;
            memcpy(&fval, &bits, sizeof(float));
            value = fval;
            break;
        }

        case ToonType::TOON_STRING: {
            if (ptr + 2 > end) return false;
            uint16_t len = ((uint16_t)ptr[0] << 8) | ptr[1];
            ptr += 2;
            if (ptr + len > end) return false;
            String str;
            str.reserve(len);
            for (uint16_t i = 0; i < len; i++) {
                str += (char)ptr[i];
            }
            ptr += len;
            value = str;
            break;
        }

        case ToonType::TOON_OBJECT: {
            if (ptr + 2 > end) return false;
            uint16_t size = ((uint16_t)ptr[0] << 8) | ptr[1];
            ptr += 2;

            ToonObject* obj = new ToonObject();

            for (uint16_t i = 0; i < size; i++) {
                // Read key
                if (ptr + 2 > end) {
                    delete obj;
                    return false;
                }
                uint16_t keyLen = ((uint16_t)ptr[0] << 8) | ptr[1];
                ptr += 2;
                if (ptr + keyLen > end) {
                    delete obj;
                    return false;
                }

                String key;
                key.reserve(keyLen);
                for (uint16_t j = 0; j < keyLen; j++) {
                    key += (char)ptr[j];
                }
                ptr += keyLen;

                // Read value
                ToonValue val;
                if (!readValue(ptr, end, val)) {
                    delete obj;
                    return false;
                }

                (*obj)[key] = val;
            }

            value._setObject(obj);
            break;
        }

        case ToonType::TOON_ARRAY: {
            if (ptr + 2 > end) return false;
            uint16_t size = ((uint16_t)ptr[0] << 8) | ptr[1];
            ptr += 2;

            ToonArray* arr = new ToonArray();

            for (uint16_t i = 0; i < size; i++) {
                ToonValue val;
                if (!readValue(ptr, end, val)) {
                    delete arr;
                    return false;
                }
                arr->add(val);
            }

            value._setArray(arr);
            break;
        }

        default:
            return false;
    }

    return true;
}

} // namespace ToonBinaryReader

bool deserializeToonBinary(ToonDocument& doc, const uint8_t* buffer, size_t length) {
    doc.clear();

    if (length < 8) return false;  // Minimum: header(4) + version(2) + CRC(2)

    // Check magic header "TOON"
    if (buffer[0] != 'T' || buffer[1] != 'O' || buffer[2] != 'O' || buffer[3] != 'N') {
        return false;
    }

    // Check version
    uint8_t versionMajor = buffer[4];
    uint8_t versionMinor = buffer[5];
    if (versionMajor != 1 || versionMinor != 0) {
        return false;
    }

    // Extract CRC
    uint16_t storedCRC = ((uint16_t)buffer[6] << 8) | buffer[7];

    // Calculate CRC of data
    uint16_t calculatedCRC = calculateCRC16(buffer + 8, length - 8);

    // Verify CRC
    if (storedCRC != calculatedCRC) {
        return false;  // CRC mismatch - corrupted data
    }

    // Parse data
    const uint8_t* ptr = buffer + 8;
    const uint8_t* end = buffer + length;

    ToonValue rootValue;
    if (!ToonBinaryReader::readValue(ptr, end, rootValue)) {
        return false;
    }

    if (rootValue.isObject()) {
        ToonObject* obj = rootValue._getObject();
        if (obj) {
            // Copy to document root
            const auto& pairs = obj->_getPairs();
            for (const auto& pair : pairs) {
                doc[pair.key] = pair.value;
            }
        }
    }

    return true;
}

bool deserializeToonBinary(ToonDocument& doc, Stream& input) {
    std::vector<uint8_t> buffer;
    while (input.available()) {
        buffer.push_back(input.read());
    }
    return deserializeToonBinary(doc, buffer.data(), buffer.size());
}

// ============================================================================
// ARDUINOJSON DESERIALIZATION (Optional - only if ArduinoJson is installed)
// ============================================================================

#ifdef ESPTOON_HAS_ARDUINOJSON

namespace ToonJsonReader {

void jsonToValue(JsonVariantConst variant, ToonValue& value) {
    if (variant.is<bool>()) {
        value = variant.as<bool>();
    } else if (variant.is<int>()) {
        value = variant.as<int>();
    } else if (variant.is<float>()) {
        value = variant.as<float>();
    } else if (variant.is<const char*>()) {
        value = variant.as<const char*>();
    } else if (variant.is<JsonObjectConst>()) {
        JsonObjectConst obj = variant.as<JsonObjectConst>();
        ToonObject* toonObj = new ToonObject();

        for (JsonPairConst kv : obj) {
            ToonValue val;
            jsonToValue(kv.value(), val);
            (*toonObj)[kv.key().c_str()] = val;
        }

        value._setObject(toonObj);
    } else if (variant.is<JsonArrayConst>()) {
        JsonArrayConst arr = variant.as<JsonArrayConst>();
        ToonArray* toonArr = new ToonArray();

        for (JsonVariantConst elem : arr) {
            ToonValue val;
            jsonToValue(elem, val);
            toonArr->add(val);
        }

        value._setArray(toonArr);
    }
}

} // namespace ToonJsonReader

bool deserializeToonJson(ToonDocument& doc, const JsonDocument& jsonDoc) {
    doc.clear();

    JsonObjectConst root = jsonDoc.as<JsonObjectConst>();
    for (JsonPairConst kv : root) {
        ToonValue val;
        ToonJsonReader::jsonToValue(kv.value(), val);
        doc[kv.key().c_str()] = val;
    }

    return true;
}

#endif // ESPTOON_HAS_ARDUINOJSON

// ============================================================================
// UNIFIED DESERIALIZATION API
// ============================================================================

bool deserializeToon(ToonDocument& doc, String& source) {
    return deserializeToonText(doc, source);
}

bool deserializeToon(ToonDocument& doc, const char*& source) {
    return deserializeToonText(doc, source);
}

bool deserializeToon(ToonDocument& doc, Stream& source) {
    // Try binary first (check for magic header)
    uint8_t header[4];
    size_t read = source.readBytes(header, 4);

    if (read == 4 && header[0] == 'T' && header[1] == 'O' && header[2] == 'O' && header[3] == 'N') {
        // Binary format - read rest
        std::vector<uint8_t> buffer;
        buffer.push_back('T');
        buffer.push_back('O');
        buffer.push_back('O');
        buffer.push_back('N');

        while (source.available()) {
            buffer.push_back(source.read());
        }

        return deserializeToonBinary(doc, buffer.data(), buffer.size());
    } else {
        // Text format - rewind not possible, convert to string
        String data;
        for (size_t i = 0; i < read; i++) {
            data += (char)header[i];
        }
        while (source.available()) {
            data += (char)source.read();
        }
        return deserializeToonText(doc, data);
    }
}

#ifdef ESPTOON_HAS_ARDUINOJSON
bool deserializeToon(ToonDocument& doc, JsonDocument& source) {
    return deserializeToonJson(doc, source);
}
#endif

#ifdef ESP32
bool deserializeToon(ToonDocument& doc, fs::File& source) {
    return deserializeToonBinary(doc, source);
}
#elif defined(ESP8266)
bool deserializeToon(ToonDocument& doc, File& source) {
    return deserializeToonBinary(doc, source);
}
#endif
