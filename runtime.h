#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <type_traits>
#include <functional>

struct Value {
    int* i = nullptr;
    float* f = nullptr;
    std::string* s = nullptr;

    Value() = default;
    Value(int i);
    Value(float f);
    Value(const std::string& s);
    Value(const Value& v);

    Value& operator=(const Value& v);
    Value operator+(const Value& v) const;
    Value operator-(const Value& v) const;
    Value operator*(const Value& v) const;
    Value operator/(const Value& v) const;
    Value operator%(const Value& v) const;
    Value operator&&(const Value& v) const;
    Value operator||(const Value& v) const;

    Value operator!() const;

    Value min(const Value& v) const;
    Value max(const Value& v) const;

    explicit operator bool() const;

    ~Value();

    friend std::ostream& operator<<(std::ostream& os, const Value& v);
private:
    template <class T>
    Value execute(const Value& v) const;
    void copyFrom(const Value& v);
};

class Runtime {
public:
    ~Runtime();
    std::unordered_map<std::string, Value> vars;
};