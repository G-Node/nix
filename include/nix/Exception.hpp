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

namespace nix {

#ifdef _MSC_VER
#define NOEXCEPT
#else
#define NOEXCEPT noexcept
#endif

class OutOfBounds : public std::out_of_range {
public:
    OutOfBounds(const std::string &what_arg, size_t where) :
        out_of_range(what_arg), index(where) {
        std::stringstream sstream(what_arg);

        sstream << " [at index: " << where << "]";
        msg = sstream.str();
    }

    size_t where(void) const {
        return index;
    }

    const char *what() const NOEXCEPT {
        return msg.c_str();
    }

private:
    size_t index;
    std::string msg;
};


class UninitializedEntity : public std::exception {
public:
    UninitializedEntity() { }
    const char *what() const NOEXCEPT {
        return "The Entity being accessed is uninitialized.";
    }
};


class InvalidUnit: public std::exception {
public:
    InvalidUnit(const std::string &what_arg, const std::string &caller_arg):
    what_msg(what_arg), caller(caller_arg){ }

    const char *what() const NOEXCEPT {
        std::stringstream sstream("InvalidUnit: ");
        sstream << what_msg << " evoked at: " << caller;
        return sstream.str().c_str();
    }

private:
    std::string what_msg;
    std::string caller;
};

}

#endif
