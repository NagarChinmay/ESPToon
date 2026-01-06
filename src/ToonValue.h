#ifndef TOON_VALUE_H
#define TOON_VALUE_H

#include <Arduino.h>
#include <WString.h>
#include <cstdint>  // For int32_t, uint8_t, uint16_t, etc.

// Forward declarations
class ToonObject;
class ToonArray;

enum class ToonType {
    TOON_NULL,
    TOON_BOOL,
    TOON_INT,
    TOON_FLOAT,
    TOON_STRING,
    TOON_OBJECT,
    TOON_ARRAY
};

class ToonValue {
private:
    ToonType _type;

    union {
        bool _bool;
        int32_t _int;
        float _float;
        String* _string;
        ToonObject* _object;
        ToonArray* _array;
    } _value;

    void _clear();
    void _copyFrom(const ToonValue& other);

public:
    ToonValue();
    ToonValue(bool value);
    ToonValue(int32_t value);
    ToonValue(float value);
    ToonValue(double value);
    ToonValue(const char* value);
    ToonValue(const String& value);
    ToonValue(const ToonValue& other);
    ~ToonValue();

    ToonValue& operator=(const ToonValue& other);
    ToonValue& operator=(bool value);
    ToonValue& operator=(int32_t value);

    // Integer type overloads for cross-platform compatibility
    // These resolve ambiguities across different compilers (MSVC, GCC, Clang)
    ToonValue& operator=(int value);        // For plain int literals
    ToonValue& operator=(long value);       // For long literals
    ToonValue& operator=(unsigned int value);  // For unsigned int
    ToonValue& operator=(unsigned long value); // For unsigned long
    ToonValue& operator=(short value);      // For short
    ToonValue& operator=(unsigned short value); // For unsigned short

    ToonValue& operator=(float value);
    ToonValue& operator=(double value);
    ToonValue& operator=(const char* value);
    ToonValue& operator=(const String& value);

    // Forwarding operators for nested access
    ToonValue& operator[](const String& key);
    ToonValue& operator[](const char* key);
    ToonValue& operator[](int index);
    ToonValue& operator[](size_t index);
    const ToonValue& operator[](const String& key) const;
    const ToonValue& operator[](const char* key) const;
    const ToonValue& operator[](int index) const;
    const ToonValue& operator[](size_t index) const;

    // Type checking
    ToonType type() const { return _type; }
    bool isNull() const { return _type == ToonType::TOON_NULL; }
    bool isBool() const { return _type == ToonType::TOON_BOOL; }
    bool isInt() const { return _type == ToonType::TOON_INT; }
    bool isFloat() const { return _type == ToonType::TOON_FLOAT; }
    bool isString() const { return _type == ToonType::TOON_STRING; }
    bool isObject() const { return _type == ToonType::TOON_OBJECT; }
    bool isArray() const { return _type == ToonType::TOON_ARRAY; }

    // Type conversion (returns true if type matches)
    template<typename T>
    bool is() const;

    // Value extraction with conversion
    operator bool() const;
    operator int32_t() const;
    operator float() const;
    operator double() const;
    operator String() const;

    // Get as specific type
    bool asBool() const;
    int32_t asInt() const;
    float asFloat() const;
    String asString() const;

    // Object/Array conversions
    template<typename T>
    T to();

    template<typename T>
    const T to() const;

    // Object/Array access as references (preferred method)
    ToonObject& asObject();
    const ToonObject& asObject() const;
    ToonArray& asArray();
    const ToonArray& asArray() const;

    // For internal use
    void _setObject(ToonObject* obj);
    void _setArray(ToonArray* arr);
    ToonObject* _getObject() const;
    ToonArray* _getArray() const;
};

#endif // TOON_VALUE_H
