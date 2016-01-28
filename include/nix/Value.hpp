// Copyright © 2014, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <kellner@bio.lmu.de>

#ifndef NIX_VALUE_H
#define NIX_VALUE_H

#include <nix/DataType.hpp>
#include <nix/Platform.hpp>
#include <nix/None.hpp>
#include <nix/Variant.hpp>

namespace nix {


/**
 * @brief Class that corresponds to the odML Value.
 */
class NIXAPI Value {
private:
    Variant data;

public:
    double uncertainty = 0.0;

    std::string reference;
    std::string filename;
    std::string encoder;
    std::string checksum;


    Value() : data() { }

    explicit Value(char *value) : data(value) { }

    explicit Value(const char *value) : data(value) { }

    template<typename T>
    explicit Value(const T &value) : data(value) { }

    template<size_t N>
    explicit Value(const char (&value)[N]) : data(value) { }

    Value(const Value &other) : data(other.data) {
        uncertainty = other.uncertainty;
        checksum = other.checksum;
        encoder = other.encoder;
        filename = other.filename;
        reference = other.reference;
    }

    Value(Value &&other) NOEXCEPT : data() {
        swap(other);
    }

    Value &operator=(Value other) {
        swap(other);
        return *this;
    }

    void set(none_t) { data.set(nix::none); }
    void set(bool value) { data.set(value); }
    void set(int32_t value) { data.set(value); }
    void set(uint32_t value) { data.set(value); }
    void set(int64_t value) { data.set(value); }
    void set(uint64_t value) { data.set(value); }
    void set(double value) { data.set(value); }
    void set(const char *value, const size_t len) { data.set(value, len); }
    void set(const char *value) { data.set(value); }
    void set(const std::string &value) { data.set(value); }

    template<typename T>
    T get() const {
        T temp;
        get(temp);
        return temp;
    }

    void get(none_t &tag) const { data.get(tag); }
    void get(bool &out) const { data.get(out); }
    void get(int32_t &value) const { data.get(value); }
    void get(uint32_t &value) const { data.get(value); }
    void get(int64_t &value) const { data.get(value); }
    void get(uint64_t &value) const { data.get(value); }
    void get(double &value) const { data.get(value); }
    void get(std::string &value) const { data.get(value); }

    DataType type() const {
        return data.type();
    }

    static bool supports_type(DataType dtype) {
        // that is true for now, but will change 
        return Variant::supports_type(dtype);
    }

    void swap(Value &other);

    bool operator ==(Value const& other) const;
};


NIXAPI std::ostream& operator<<(std::ostream &out, const Value &value);
inline bool operator!=(const Value &a, const Value &b) { return !(a == b); }
NIXAPI void swap(Value &a, Value &b);

template<>
inline const char *Value::get<const char *>() const {
    return data.get<const char*>();
}

template<>
inline const none_t Value::get<>() const {
    return data.get<none_t>();
}

} // namespace nix

#endif // NIX_VALUE_H
