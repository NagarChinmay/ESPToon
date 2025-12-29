#include "ToonDocument.h"

ToonDocument::ToonDocument() {}

ToonDocument::~ToonDocument() {
    clear();
}

ToonValue& ToonDocument::operator[](const String& key) {
    return _root[key];
}

ToonValue& ToonDocument::operator[](const char* key) {
    return _root[key];
}

const ToonValue& ToonDocument::operator[](const String& key) const {
    return _root[key];
}

const ToonValue& ToonDocument::operator[](const char* key) const {
    return _root[key];
}

void ToonDocument::clear() {
    _root.clear();
}

size_t ToonDocument::size() const {
    return _root.size();
}

bool ToonDocument::containsKey(const String& key) const {
    return _root.containsKey(key);
}

bool ToonDocument::containsKey(const char* key) const {
    return _root.containsKey(key);
}

ToonObject& ToonDocument::createNestedObject(const String& key) {
    return _root.createNestedObject(key);
}

ToonArray& ToonDocument::createNestedArray(const String& key) {
    return _root.createNestedArray(key);
}
