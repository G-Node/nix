// Copyright © 2014 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <kellner@bio.lmu.de>

#ifndef NIX_EXCEPTION_H
#define NIX_EXCEPTION_H

#include <nix/Platform.hpp>
#include <nix/types.hpp>

#include <limits>
#include <type_traits>
#include <cstddef>

#include <stdexcept>
#include <sstream>
#include <vector>

namespace nix {

class OutOfBounds : public std::out_of_range {
public:
    OutOfBounds(const std::string &what_arg, ndsize_t where) :
            out_of_range(make_message(what_arg, where)), index(where) {

    }

    OutOfBounds(const std::string &what_arg) :
            out_of_range(what_arg), index(0) { }

    ndsize_t where(void) const {
        return index;
    }

    static std::string make_message(const std::string &str, ndsize_t where) {
            std::stringstream sstream(str);

            sstream << " [at index: " << where << "]";
            return sstream.str();
    }

private:
    ndsize_t index;
};


class InvalidRank : public std::out_of_range {
public:
    InvalidRank(const std::string &message)
            : std::out_of_range(message) { }
};


class UninitializedEntity : public std::exception {
public:
    UninitializedEntity() { }
    const char *what() const NOEXCEPT {
        return "The Entity being accessed is uninitialized or empty.";
    }
};

class EmptyString: public std::invalid_argument {
public:
    EmptyString(const std::string &caller) :
            std::invalid_argument("Empty string given - not a valid value for " + caller) { }
};


class DuplicateName: public std::invalid_argument {
public:
    DuplicateName(const std::string &caller):
    std::invalid_argument("Duplicate name - names have to be unique for a given entity type & parent. (" + caller + ")") { }

};


class InvalidName: public std::invalid_argument {
public:
    InvalidName(const std::string &caller):
    std::invalid_argument("Invalid name given - names have to be sanitized using util function. (" + caller + ")") { }
};


class InvalidFile: public std::invalid_argument {
public:
    InvalidFile(const std::string &message):
        std::invalid_argument("Invalid file! (" + message + ")") { }
};


class UnsortedTicks: public std::invalid_argument {
public:
    UnsortedTicks(const std::string &caller):
            std::invalid_argument("Ticks are not given in a ascending order: " + caller) { }
};


class InvalidUnit: public std::invalid_argument {
public:
    InvalidUnit(const std::string &what, const std::string &where):
            std::invalid_argument("InvalidUnit: " + what + " evoked at: " + where) { }
};


class IncompatibleDimensions: public std::invalid_argument {
public:
    IncompatibleDimensions(const std::string &what, const std::string &where):
            std::invalid_argument("IncompatibleDimensions: " + what + " evoked at: " + where) { }
};


class InvalidDimension : public std::invalid_argument {
public:
    InvalidDimension(const std::string &what, const std::string &where):
            std::invalid_argument("InvalidDimension: " + what + " evoked at: " + where) { }
};

class ConsistencyError: public std::runtime_error {
public:
    ConsistencyError(const std::string &what) : runtime_error("ConsistencyError: " + what){ }
};

class MissingAttr: public std::runtime_error {
public:
    MissingAttr(const std::string &name):
            std::runtime_error("MissingAttribute: Obligatory attribute " + name + " is not set!") { }
};

class InvalidLinkType: public std::invalid_argument {
public:
    InvalidLinkType(const std::string &what, const std::string &where):
            std::invalid_argument("InvalidLinkType: " + what + " evoked at: " + where) { }
};


namespace check {

template<typename T>
inline typename std::enable_if<! std::is_same<T, size_t>::value, size_t>::type
fits_in_size_t(T size, const std::string &msg_if_fail) {
    if (size > std::numeric_limits<size_t>::max()) {
        throw OutOfBounds(msg_if_fail);
    }
    return static_cast<size_t>(size);
}

template<typename T>
inline typename std::enable_if<std::is_same<T, size_t>::value, size_t>::type
fits_in_size_t(T size, const std::string &msg_if_fail) {
    return size;
}

template<typename T>
inline typename std::enable_if<std::is_integral<T>::value, double>::type
converts_to_double(T num, const std::string &msg_if_fail) {
    double dbl = static_cast<double>(num);
    if (static_cast<T>(dbl) != num) {
        throw OutOfBounds(msg_if_fail);
    }
    return dbl;
}

} // nix::check::


}

#endif
