#ifndef TOON_DOCUMENT_H
#define TOON_DOCUMENT_H

#include "ToonValue.h"
#include "ToonObject.h"
#include "ToonArray.h"

class ToonDocument {
private:
    ToonObject _root;

public:
    ToonDocument();
    ~ToonDocument();

    // Root access
    ToonValue& operator[](const String& key);
    ToonValue& operator[](const char* key);
    const ToonValue& operator[](const String& key) const;
    const ToonValue& operator[](const char* key) const;

    // Document operations
    void clear();
    size_t size() const;
    bool containsKey(const String& key) const;
    bool containsKey(const char* key) const;

    // Nested creation at root
    ToonObject& createNestedObject(const String& key);
    ToonArray& createNestedArray(const String& key);

    // Internal access
    ToonObject& _getRoot() { return _root; }
    const ToonObject& _getRoot() const { return _root; }
};

#endif // TOON_DOCUMENT_H
