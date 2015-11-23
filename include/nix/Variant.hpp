// Copyright © 2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <kellner@bio.lmu.de>

#ifndef NIX_VARIANT_H
#define NIX_VARIANT_H

#include <nix/DataType.hpp>
#include <nix/Platform.hpp>
#include <nix/None.hpp>

#include <string>
#include <cstdint>
#include <stdexcept>
#include <iostream>

namespace nix {
/**
 * @brief Class that can hold either a bool, double, (u)int(34|64) or a string.
 */
class NIXAPI Variant {
private:
    DataType dtype;

    union {
        bool v_bool;
        double v_double;
        uint32_t v_uint32;
        int32_t v_int32;
        uint64_t v_uint64;
        int64_t v_int64;
        char *v_string;
    };

public:
    Variant() : dtype(DataType::Nothing), v_bool(false) { }

    explicit Variant(char *value) : dtype(DataType::Nothing) {
        set(value);
    }

    explicit Variant(const char *value) : dtype(DataType::Nothing) {
        set(value);
    }

    template<typename T>
    explicit Variant(const T &value) : dtype(DataType::Nothing) {
        set(value);
    }

    template<size_t N>
    explicit Variant(const char (&value)[N]) : dtype(DataType::Nothing) {
        set(value, N);
    }

    Variant(const Variant &other) : Variant() {
        assign_variant_from(other);
    }

    Variant(Variant &&other) NOEXCEPT : Variant() {
        swap(other);
    }

    Variant &operator=(Variant other) {
        swap(other);
        return *this;
    }

    virtual ~Variant() {
        maybe_deallocte_string();
    }

    void set(none_t);
    void set(bool value);
    void set(int32_t value);
    void set(uint32_t value);
    void set(int64_t value);
    void set(uint64_t value);
    void set(double value);
    void set(const char *value, const size_t len);
    void set(const char *value);
    void set(const std::string &value);

    template<typename T>
    T get() const {
        T temp;
        get(temp);
        return temp;
    }

    void get(none_t &tag) const;
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

    void swap(Variant &other);

    static bool supports_type(DataType dtype);

private:

    template<typename T>
    void swap_helper(Variant &other) {
        T temp = get<T>();
        assign_variant_from(other);
        other.set(temp);
    }

    void assign_variant_from(const Variant &other);

    void maybe_deallocte_string();

    inline void check_argument_type(DataType check) const {
        if (dtype != check) {
            throw std::invalid_argument("Incompatible DataType");
        }
    }
};


template<>
inline const char *Variant::get<const char *>() const {
    check_argument_type(DataType::String);
    return v_string;
}


NIXAPI std::ostream &operator<<(std::ostream &out, const Variant &value);
NIXAPI bool operator==(const Variant &a, const Variant &b);
inline bool operator!=(const Variant &a, const Variant &b) { return !(a == b); }
NIXAPI void swap(Variant &a, Variant &b);

} // nix::
#endif