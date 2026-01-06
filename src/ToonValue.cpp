#include "ToonValue.h"
#include "ToonObject.h"
#include "ToonArray.h"

ToonValue::ToonValue() : _type(ToonType::TOON_NULL) {
    _value._int = 0;
}

ToonValue::ToonValue(bool value) : _type(ToonType::TOON_BOOL) {
    _value._bool = value;
}

ToonValue::ToonValue(int32_t value) : _type(ToonType::TOON_INT) {
    _value._int = value;
}

ToonValue::ToonValue(float value) : _type(ToonType::TOON_FLOAT) {
    _value._float = value;
}

ToonValue::ToonValue(double value) : _type(ToonType::TOON_FLOAT) {
    _value._float = (float)value;
}

ToonValue::ToonValue(const char* value) : _type(ToonType::TOON_STRING) {
    _value._string = new String(value);
}

ToonValue::ToonValue(const String& value) : _type(ToonType::TOON_STRING) {
    _value._string = new String(value);
}

ToonValue::ToonValue(const ToonValue& other) : _type(ToonType::TOON_NULL) {
    _value._int = 0;
    _copyFrom(other);
}

ToonValue::~ToonValue() {
    _clear();
}

void ToonValue::_clear() {
    switch (_type) {
        case ToonType::TOON_STRING:
            delete _value._string;
            break;
        case ToonType::TOON_OBJECT:
            delete _value._object;
            break;
        case ToonType::TOON_ARRAY:
            delete _value._array;
            break;
        default:
            break;
    }
    _type = ToonType::TOON_NULL;
    _value._int = 0;
}

void ToonValue::_copyFrom(const ToonValue& other) {
    _clear();
    _type = other._type;

    switch (_type) {
        case ToonType::TOON_BOOL:
            _value._bool = other._value._bool;
            break;
        case ToonType::TOON_INT:
            _value._int = other._value._int;
            break;
        case ToonType::TOON_FLOAT:
            _value._float = other._value._float;
            break;
        case ToonType::TOON_STRING:
            _value._string = new String(*other._value._string);
            break;
        case ToonType::TOON_OBJECT:
            _value._object = new ToonObject(*other._value._object);
            break;
        case ToonType::TOON_ARRAY:
            _value._array = new ToonArray(*other._value._array);
            break;
        default:
            break;
    }
}

ToonValue& ToonValue::operator=(const ToonValue& other) {
    if (this != &other) {
        _copyFrom(other);
    }
    return *this;
}

ToonValue& ToonValue::operator=(bool value) {
    _clear();
    _type = ToonType::TOON_BOOL;
    _value._bool = value;
    return *this;
}

ToonValue& ToonValue::operator=(int32_t value) {
    _clear();
    _type = ToonType::TOON_INT;
    _value._int = value;
    return *this;
}

// Integer type overload implementations
ToonValue& ToonValue::operator=(int value) {
    return operator=(static_cast<int32_t>(value));
}

ToonValue& ToonValue::operator=(long value) {
    return operator=(static_cast<int32_t>(value));
}

ToonValue& ToonValue::operator=(unsigned int value) {
    return operator=(static_cast<int32_t>(value));
}

ToonValue& ToonValue::operator=(unsigned long value) {
    return operator=(static_cast<int32_t>(value));
}

ToonValue& ToonValue::operator=(short value) {
    return operator=(static_cast<int32_t>(value));
}

ToonValue& ToonValue::operator=(unsigned short value) {
    return operator=(static_cast<int32_t>(value));
}

ToonValue& ToonValue::operator=(float value) {
    _clear();
    _type = ToonType::TOON_FLOAT;
    _value._float = value;
    return *this;
}

ToonValue& ToonValue::operator=(double value) {
    _clear();
    _type = ToonType::TOON_FLOAT;
    _value._float = (float)value;
    return *this;
}

ToonValue& ToonValue::operator=(const char* value) {
    _clear();
    _type = ToonType::TOON_STRING;
    _value._string = new String(value);
    return *this;
}

ToonValue& ToonValue::operator=(const String& value) {
    _clear();
    _type = ToonType::TOON_STRING;
    _value._string = new String(value);
    return *this;
}

// Forwarding operators for nested access
ToonValue& ToonValue::operator[](const String& key) {
    if (_type != ToonType::TOON_OBJECT) {
        _clear();
        _type = ToonType::TOON_OBJECT;
        _value._object = new ToonObject();
    }
    return (*_value._object)[key];
}

ToonValue& ToonValue::operator[](const char* key) {
    if (_type != ToonType::TOON_OBJECT) {
        _clear();
        _type = ToonType::TOON_OBJECT;
        _value._object = new ToonObject();
    }
    return (*_value._object)[key];
}

ToonValue& ToonValue::operator[](int index) {
    return (*this)[(size_t)index];
}

ToonValue& ToonValue::operator[](size_t index) {
    if (_type != ToonType::TOON_ARRAY) {
        _clear();
        _type = ToonType::TOON_ARRAY;
        _value._array = new ToonArray();
    }
    return (*_value._array)[index];
}

const ToonValue& ToonValue::operator[](const String& key) const {
    if (_type == ToonType::TOON_OBJECT) {
        return (*_value._object)[key];
    }
    static ToonValue nullValue;
    return nullValue;
}

const ToonValue& ToonValue::operator[](const char* key) const {
    if (_type == ToonType::TOON_OBJECT) {
        return (*_value._object)[key];
    }
    static ToonValue nullValue;
    return nullValue;
}

const ToonValue& ToonValue::operator[](int index) const {
    return (*this)[(size_t)index];
}

const ToonValue& ToonValue::operator[](size_t index) const {
    if (_type == ToonType::TOON_ARRAY) {
        return (*_value._array)[index];
    }
    static ToonValue nullValue;
    return nullValue;
}

// Type conversions
ToonValue::operator bool() const {
    return asBool();
}

ToonValue::operator int32_t() const {
    return asInt();
}

ToonValue::operator float() const {
    return asFloat();
}

ToonValue::operator double() const {
    return (double)asFloat();
}

ToonValue::operator String() const {
    return asString();
}

bool ToonValue::asBool() const {
    switch (_type) {
        case ToonType::TOON_BOOL:
            return _value._bool;
        case ToonType::TOON_INT:
            return _value._int != 0;
        case ToonType::TOON_FLOAT:
            return _value._float != 0.0f;
        case ToonType::TOON_STRING:
            return _value._string->length() > 0;
        default:
            return false;
    }
}

int32_t ToonValue::asInt() const {
    switch (_type) {
        case ToonType::TOON_BOOL:
            return _value._bool ? 1 : 0;
        case ToonType::TOON_INT:
            return _value._int;
        case ToonType::TOON_FLOAT:
            return (int32_t)_value._float;
        case ToonType::TOON_STRING:
            return _value._string->toInt();
        default:
            return 0;
    }
}

float ToonValue::asFloat() const {
    switch (_type) {
        case ToonType::TOON_BOOL:
            return _value._bool ? 1.0f : 0.0f;
        case ToonType::TOON_INT:
            return (float)_value._int;
        case ToonType::TOON_FLOAT:
            return _value._float;
        case ToonType::TOON_STRING:
            return _value._string->toFloat();
        default:
            return 0.0f;
    }
}

String ToonValue::asString() const {
    switch (_type) {
        case ToonType::TOON_BOOL:
            return _value._bool ? "true" : "false";
        case ToonType::TOON_INT:
            return String(_value._int);
        case ToonType::TOON_FLOAT:
            return String(_value._float);
        case ToonType::TOON_STRING:
            return *_value._string;
        default:
            return String();
    }
}

// Object/Array access as references
ToonObject& ToonValue::asObject() {
    if (_type != ToonType::TOON_OBJECT) {
        _clear();
        _type = ToonType::TOON_OBJECT;
        _value._object = new ToonObject();
    }
    return *_value._object;
}

const ToonObject& ToonValue::asObject() const {
    if (_type == ToonType::TOON_OBJECT) {
        return *_value._object;
    }
    static ToonObject emptyObject;
    return emptyObject;
}

ToonArray& ToonValue::asArray() {
    if (_type != ToonType::TOON_ARRAY) {
        _clear();
        _type = ToonType::TOON_ARRAY;
        _value._array = new ToonArray();
    }
    return *_value._array;
}

const ToonArray& ToonValue::asArray() const {
    if (_type == ToonType::TOON_ARRAY) {
        return *_value._array;
    }
    static ToonArray emptyArray;
    return emptyArray;
}

// Template specializations for to<T>() - return by value for backward compatibility
template<>
ToonObject ToonValue::to<ToonObject>() {
    return asObject();
}

template<>
const ToonObject ToonValue::to<ToonObject>() const {
    return asObject();
}

template<>
ToonArray ToonValue::to<ToonArray>() {
    return asArray();
}

template<>
const ToonArray ToonValue::to<ToonArray>() const {
    return asArray();
}

// Template specializations for is<T>()
template<>
bool ToonValue::is<bool>() const {
    return isBool();
}

template<>
bool ToonValue::is<int32_t>() const {
    return isInt();
}

template<>
bool ToonValue::is<float>() const {
    return isFloat();
}

template<>
bool ToonValue::is<double>() const {
    return isFloat();
}

template<>
bool ToonValue::is<String>() const {
    return isString();
}

template<>
bool ToonValue::is<const char*>() const {
    return isString();
}

template<>
bool ToonValue::is<ToonObject>() const {
    return isObject();
}

template<>
bool ToonValue::is<ToonArray>() const {
    return isArray();
}

void ToonValue::_setObject(ToonObject* obj) {
    _clear();
    _type = ToonType::TOON_OBJECT;
    _value._object = obj;
}

void ToonValue::_setArray(ToonArray* arr) {
    _clear();
    _type = ToonType::TOON_ARRAY;
    _value._array = arr;
}

ToonObject* ToonValue::_getObject() const {
    if (_type == ToonType::TOON_OBJECT) {
        return _value._object;
    }
    return nullptr;
}

ToonArray* ToonValue::_getArray() const {
    if (_type == ToonType::TOON_ARRAY) {
        return _value._array;
    }
    return nullptr;
}
