#include "runtime.h"
#include <iostream>

struct Sum {
    static int calc(int a, int b) {
        return a + b;
    }

    static float calc(float a, float b) {
        return a + b;
    }
};

struct Minus {
    static int calc(int a, int b) {
        return a - b;
    }

    static float calc(float a, float b) {
        return a - b;
    }
};

struct Mul {
    static int calc(int a, int b) {
        return a * b;
    }

    static float calc(float a, float b) {
        return a * b;
    }
};

struct Div {
    static int calc(int a, int b) {
        return a / b;
    }

    static float calc(float a, float b) {
        return a / b;
    }
};

struct Mod {
    static int calc(int a, int b) {
        return a % b;
    }
};

struct Max {
    static int calc(int a, int b) {
        return std::max(a, b);
    }

    static float calc(float a, float b) {
        return std::max(a, b);
    }
};

struct Min {
    static int calc(int a, int b) {
        return std::min(a, b);
    }

    static float calc(float a, float b) {
        return std::min(a, b);
    }
};

struct And {
    static int calc(int a, int b) {
        return a != 0 && b != 0;
    }
};

struct Or {
    static int calc (int a, int b) {
        return a != 0 || b != 0;
    }
};

///////////////////////////////////////////////////////////////////////////////

Value::Value(int i): i(new int(i)) { }
Value::Value(float f): f(new float(f)) { }
Value::Value(const std::string& s): s(new std::string(s)) { }
Value::Value(const Value& v) {
    copyFrom(v);
}

Value& Value::operator=(const Value& v) {
    if (this != &v)
        copyFrom(v);
    return *this;
}

Value Value::operator+(const Value& v) const {
    return execute<Sum>(v);
}

Value Value::operator-(const Value& v) const {
    return execute<Minus>(v);
}

Value Value::operator*(const Value& v) const {
    return execute<Mul>(v);
}

Value Value::operator/(const Value& v) const {
    return execute<Div>(v);
}

Value Value::operator%(const Value& v) const {
    return execute<Mod>(v);
}

Value Value::min(const Value& v) const {
    return execute<Min>(v);
}
Value Value::max(const Value& v) const {
    return execute<Max>(v);
}

Value Value::operator&&(const Value& v) const {
    return execute<And>(v);
}

Value Value::operator||(const Value& v) const {
    return execute<Or>(v);
}

Value Value::operator!() const {
    if (i)
        return !i;
    if (s)
        return s->empty();
    return Value();
}

Value::~Value() {
    delete i;
    delete f;
    delete s;
}

static int toInt(std::string* s) {
    if (s->empty())
        return 0;
    return std::stoi(*s);
}

static float toFloat(std::string* s) {
    if (s->empty())
        return 0;
    return std::stof(*s);
}

template <class T>
Value Value::execute(const Value& v) const {
    if (i) {
        if (v.i)
            return Value(T::calc(*i, *v.i));
        else if (v.f)
            return Value(T::calc((float) *i, *v.f));
        else
            return Value(T::calc(*i, toInt(v.s))); 
    } else if (f) {
        if (v.f)
            return Value(T::calc(*f, *v.f));
        else if (v.i)
            return Value(T::calc(*f, (float) *v.i));
        else
            return Value(T::calc(*f, toFloat(v.s)));
    } else if (s && v.s)
        return Value(T::calc(toInt(s), toInt(v.s)));
    else
        return v + *this;
}

void Value::copyFrom(const Value& v) {
    if (v.i)
        i = new int(*v.i);
    if (v.f)
        f = new float(*v.f);
    if (v.s)
        s = new std::string(*v.s);
}

std::ostream& operator<<(std::ostream& os, const Value& v) {
    if (v.i)
        return os << *v.i;
    if (v.f)
        return os << *v.f;
    if (v.s)
        return os << *v.s;
    return os;
}

Value::operator bool() const {
    if (i)
        return *i;
    if (s)
        return !s->empty();
    return false;
}

Runtime::~Runtime() {
    std::cout << "\nStack:\n";
    for (auto& v : vars)
        std::cout << v.first << " = " << v.second << '\n';
}