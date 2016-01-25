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
class NIXAPI Value : public Variant {
public:
    double uncertainty = 0.0;

    std::string reference;
    std::string filename;
    std::string encoder;
    std::string checksum;


    Value() : Variant() { }

    explicit Value(char *value) : Variant(value) { }

    explicit Value(const char *value) : Variant(value) { }

    template<typename T>
    explicit Value(const T &value) : Variant(value) { }

    template<size_t N>
    explicit Value(const char (&value)[N]) : Variant(value) { }

    Value(const Value &other) : Variant(static_cast<const Variant &>(other)) {
        uncertainty = other.uncertainty;
        checksum = other.checksum;
        encoder = other.encoder;
        filename = other.filename;
        reference = other.reference;
    }

    Value(Value &&other) NOEXCEPT : Value() {
        swap(other);
    }

    Value &operator=(Value other) {
        swap(other);
        return *this;
    }

    void swap(Value &other);

};


NIXAPI std::ostream& operator<<(std::ostream &out, const Value &value);
NIXAPI bool operator==(const Value &a, const Value &b);
inline bool operator!=(const Value &a, const Value &b) { return !(a == b); }
NIXAPI void swap(Value &a, Value &b);




} // namespace nix

#endif // NIX_VALUE_H
