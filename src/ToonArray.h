#ifndef TOON_ARRAY_H
#define TOON_ARRAY_H

#include "ToonValue.h"
#include <vector>

class ToonArray {
private:
    std::vector<ToonValue> _elements;

public:
    ToonArray();
    ~ToonArray();

    // Access operators
    ToonValue& operator[](size_t index);
    const ToonValue& operator[](size_t index) const;

    // Methods
    ToonValue& add();
    ToonValue& add(const ToonValue& value);
    ToonValue& add(bool value);
    ToonValue& add(int32_t value);
    ToonValue& add(float value);
    ToonValue& add(double value);
    ToonValue& add(const char* value);
    ToonValue& add(const String& value);

    // Nested creation
    ToonObject& createNestedObject();
    ToonArray& createNestedArray();

    bool remove(size_t index);
    void clear();
    size_t size() const;

    // Internal access
    const std::vector<ToonValue>& _getElements() const { return _elements; }
    std::vector<ToonValue>& _getElements() { return _elements; }
};

#endif // TOON_ARRAY_H
