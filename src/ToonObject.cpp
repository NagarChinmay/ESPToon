#include "ToonObject.h"
#include "ToonArray.h"

ToonObject::ToonObject() {}

ToonObject::~ToonObject() {
    clear();
}

int ToonObject::_findKey(const String& key) const {
    for (size_t i = 0; i < _pairs.size(); i++) {
        if (_pairs[i].key == key) {
            return i;
        }
    }
    return -1;
}

ToonValue& ToonObject::operator[](const String& key) {
    int idx = _findKey(key);
    if (idx >= 0) {
        return _pairs[idx].value;
    }

    // Create new pair
    KeyValuePair pair;
    pair.key = key;
    _pairs.push_back(pair);
    return _pairs[_pairs.size() - 1].value;
}

ToonValue& ToonObject::operator[](const char* key) {
    return (*this)[String(key)];
}

const ToonValue& ToonObject::operator[](const String& key) const {
    int idx = _findKey(key);
    if (idx >= 0) {
        return _pairs[idx].value;
    }

    static ToonValue nullValue;
    return nullValue;
}

const ToonValue& ToonObject::operator[](const char* key) const {
    return (*this)[String(key)];
}

bool ToonObject::containsKey(const String& key) const {
    return _findKey(key) >= 0;
}

bool ToonObject::containsKey(const char* key) const {
    return containsKey(String(key));
}

bool ToonObject::remove(const String& key) {
    int idx = _findKey(key);
    if (idx >= 0) {
        _pairs.erase(_pairs.begin() + idx);
        return true;
    }
    return false;
}

bool ToonObject::remove(const char* key) {
    return remove(String(key));
}

void ToonObject::clear() {
    _pairs.clear();
}

size_t ToonObject::size() const {
    return _pairs.size();
}

ToonObject& ToonObject::createNestedObject(const String& key) {
    ToonValue& val = (*this)[key];
    ToonObject* obj = new ToonObject();
    val._setObject(obj);
    return *obj;
}

ToonArray& ToonObject::createNestedArray(const String& key) {
    ToonValue& val = (*this)[key];
    ToonArray* arr = new ToonArray();
    val._setArray(arr);
    return *arr;
}
