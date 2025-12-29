#include "ToonArray.h"
#include "ToonObject.h"

ToonArray::ToonArray() {}

ToonArray::~ToonArray() {
    clear();
}

ToonValue& ToonArray::operator[](size_t index) {
    if (index >= _elements.size()) {
        _elements.resize(index + 1);
    }
    return _elements[index];
}

const ToonValue& ToonArray::operator[](size_t index) const {
    if (index < _elements.size()) {
        return _elements[index];
    }
    static ToonValue nullValue;
    return nullValue;
}

ToonValue& ToonArray::add() {
    _elements.push_back(ToonValue());
    return _elements[_elements.size() - 1];
}

ToonValue& ToonArray::add(const ToonValue& value) {
    _elements.push_back(value);
    return _elements[_elements.size() - 1];
}

ToonValue& ToonArray::add(bool value) {
    _elements.push_back(ToonValue(value));
    return _elements[_elements.size() - 1];
}

ToonValue& ToonArray::add(int32_t value) {
    _elements.push_back(ToonValue(value));
    return _elements[_elements.size() - 1];
}

ToonValue& ToonArray::add(float value) {
    _elements.push_back(ToonValue(value));
    return _elements[_elements.size() - 1];
}

ToonValue& ToonArray::add(double value) {
    _elements.push_back(ToonValue(value));
    return _elements[_elements.size() - 1];
}

ToonValue& ToonArray::add(const char* value) {
    _elements.push_back(ToonValue(value));
    return _elements[_elements.size() - 1];
}

ToonValue& ToonArray::add(const String& value) {
    _elements.push_back(ToonValue(value));
    return _elements[_elements.size() - 1];
}

ToonObject& ToonArray::createNestedObject() {
    ToonValue& val = add();
    ToonObject* obj = new ToonObject();
    val._setObject(obj);
    return *obj;
}

ToonArray& ToonArray::createNestedArray() {
    ToonValue& val = add();
    ToonArray* arr = new ToonArray();
    val._setArray(arr);
    return *arr;
}

bool ToonArray::remove(size_t index) {
    if (index < _elements.size()) {
        _elements.erase(_elements.begin() + index);
        return true;
    }
    return false;
}

void ToonArray::clear() {
    _elements.clear();
}

size_t ToonArray::size() const {
    return _elements.size();
}
