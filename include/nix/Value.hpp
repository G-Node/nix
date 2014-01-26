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

#include <string>
#include <cstdint>
#include <stdexcept>
#include <iostream>

#include <nix/DataType.hpp>

namespace nix {

class Value {
private:
    DataType dtype;

    union {
        bool        v_bool;
        double      v_double;
        uint32_t    v_uint32;
        int32_t     v_int32;
        uint64_t    v_uint64;
        int64_t     v_int64;
        std::string v_string;
    };

public:
    double uncertainty;

    std::string reference;
    std::string filename;
    std::string encoder;
    std::string checksum;


    Value() : dtype(DataType::Nothing), v_bool(false) { }

    template<typename T>
    explicit Value(const T &value) : dtype(to_data_type<T>::value) {
        if (dtype == DataType::String) {
            new (&v_string) std::string();
        }

        set(value);
    }

    template<size_t N>
    explicit Value(const char (&value)[N]) : dtype(DataType::String) {
        new (&v_string) std::string();
        v_string = value;
    }

    Value(const Value &other) {
        assign_variant_from(other);
    }

    Value(Value &&other) : Value() {
        swap(other);
    }

    Value &operator=(Value other) {
        swap(other);
        return *this;
    }

    ~Value() {
        maybe_deallocte_string();
    }

    void set(bool   value);
    void set(int32_t value);
    void set(uint32_t value);
    void set(int64_t value);
    void set(uint64_t value);
    void set(double value);
    void set(const std::string &value);

    template<typename T>
    T get() const {
        T temp;
        get(temp);
        return temp;
    }

    void get(bool &out) const;
    void get(int32_t &value) const;
    void get(uint32_t &value) const;
    void get(int64_t &value) const;
    void get(uint64_t &value) const;
    void get(double &value) const;
    void get(std::string &value) const;

    DataType type() const {
        return dtype;
    }

    void swap(Value &other);

private:

    template<typename T>
    void swap_helper(Value &other) {
        T temp = get<T>();
        assign_variant_from(other);
        other.set(temp);
    }

    void assign_variant_from(const Value &other);
    void maybe_deallocte_string();

    inline void check_argument_type(DataType check) const {
        if (dtype != check) {
            throw std::invalid_argument("Incompatible DataType");
        }
    }
};


template<>
inline const char * Value::get<const char *>() const {
    check_argument_type(DataType::String);
    return v_string.c_str();
}


std::ostream& operator<<(std::ostream &out, const Value &value);
void swap(Value &a, Value &b);


} // namespace nix

#endif // NIX_VALUE_H
