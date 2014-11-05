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

#include <stdexcept>
#include <sstream>

#include <nix/Platform.hpp>

namespace nix {

class OutOfBounds : public std::out_of_range {
public:
    OutOfBounds(const std::string &what_arg, size_t where) :
            out_of_range(make_message(what_arg, where)), index(where) {

    }

    OutOfBounds(const std::string &what_arg) :
            out_of_range(what_arg), index(0) { }

    size_t where(void) const {
        return index;
    }

    static std::string make_message(const std::string &str, size_t where) {
            std::stringstream sstream(str);

            sstream << " [at index: " << where << "]";
            return sstream.str();
    }

private:
    size_t index;
};


class UninitializedEntity : public std::exception {
public:
    UninitializedEntity() { }
    const char *what() const NOEXCEPT {
        return "The Entity being accessed is uninitialized.";
    }
};


class EmptyString: public std::exception {
public:
    EmptyString(const std::string &caller_arg):
    caller(caller_arg) { }

    const char *what() const NOEXCEPT {
        std::stringstream sstream("EmptyString: ");
        sstream << "Empty string given - not a valid value for " << caller;
        return sstream.str().c_str();
    }

private:
    std::string caller;
};


class DuplicateName: public std::exception {
public:
    DuplicateName(const std::string &caller_arg):
    caller(caller_arg) { }

    const char *what() const NOEXCEPT {
        std::stringstream sstream("DuplicateName: ");
        sstream << "Duplicate name given - names have to be unique for a given entity type & parent. (" << caller << ")";
        return sstream.str().c_str();
    }

private:
    std::string caller;
};


class InvalidName: public std::exception {
public:
    InvalidName(const std::string &caller_arg):
    caller(caller_arg) { }

    const char *what() const NOEXCEPT {
        std::stringstream sstream("InvalidName: ");
        sstream << "Invalid name given - names have to be sanitized using util function. (" << caller << ")";
        return sstream.str().c_str();
    }

private:
    std::string caller;
};


class MissingAttr: public std::exception {
public:
    MissingAttr(const std::string &attr_arg):
    attr(attr_arg) { }

    const char *what() const NOEXCEPT {
        std::stringstream sstream("MissingAttribute: ");
        sstream << "Obligatory attribute " << attr << " is not set!";
        return sstream.str().c_str();
    }

private:
    std::string attr;
};


class UnsortedTicks: public std::exception {
public:
    UnsortedTicks(const std::string &caller_arg):
    caller(caller_arg) { }

    const char *what() const NOEXCEPT {
        std::stringstream sstream("UnsortedTics: ");
        sstream << "Ticks are not given in a ascending order: " << caller;
        return sstream.str().c_str();
    }

private:
    std::string caller;
};


class InvalidUnit: public std::exception {
public:
    InvalidUnit(const std::string &what_arg, const std::string &caller_arg):
    what_msg(what_arg), caller(caller_arg) { }

    const char *what() const NOEXCEPT {
        std::stringstream sstream("InvalidUnit: ");
        sstream << what_msg << " evoked at: " << caller;
        return sstream.str().c_str();
    }

private:
    std::string what_msg;
    std::string caller;
};


class IncompatibleDimensions: public std::exception {
public:
    IncompatibleDimensions(const std::string &what_arg, const std::string &caller_arg):
    what_msg(what_arg), caller(caller_arg) { }

    const char *what() const NOEXCEPT {
        std::stringstream sstream("IncompatibleDimensions: ");
        sstream << what_msg << " evoked at: " << caller;
        return sstream.str().c_str();
    }

private:
    std::string what_msg;
    std::string caller;
};

class InvalidRank : public std::out_of_range {
public:
    InvalidRank(const std::string &message)
        : std::out_of_range(message) { }
};

}

#endif
