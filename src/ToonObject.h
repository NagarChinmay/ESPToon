#ifndef TOON_OBJECT_H
#define TOON_OBJECT_H

#include "ToonValue.h"
#include <vector>

class ToonObject {
private:
    struct KeyValuePair {
        String key;
        ToonValue value;
    };

    std::vector<KeyValuePair> _pairs;

    int _findKey(const String& key) const;

public:
    ToonObject();
    ~ToonObject();

    // Access operators
    ToonValue& operator[](const String& key);
    ToonValue& operator[](const char* key);
    const ToonValue& operator[](const String& key) const;
    const ToonValue& operator[](const char* key) const;

    // Methods
    bool containsKey(const String& key) const;
    bool containsKey(const char* key) const;
    bool remove(const String& key);
    bool remove(const char* key);
    void clear();
    size_t size() const;

    // Nested creation
    ToonObject& createNestedObject(const String& key);
    ToonArray& createNestedArray(const String& key);

    // Internal iteration
    const std::vector<KeyValuePair>& _getPairs() const { return _pairs; }
    std::vector<KeyValuePair>& _getPairs() { return _pairs; }
};

#endif // TOON_OBJECT_H
