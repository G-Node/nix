// Copyright © 2015 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <kellner@bio.lmu.de>

#ifndef NIX_EXCEPTION_H5_H
#define NIX_EXCEPTION_H5_H

#include <nix/Platform.hpp>

#include <nix/hdf5/hdf5include.hpp>

#include <stdexcept>
#include <string>

namespace nix {
namespace hdf5 {

class NIXAPI H5Exception : public std::exception {
public:
    H5Exception(const std::string &message)
            : msg(message) {

    }

    const char *what() const NOEXCEPT {
        return msg.c_str();
    }

private:
    std::string msg;
};


class NIXAPI H5Error : public H5Exception {
public:
    H5Error(herr_t err, const std::string &msg)
    : H5Exception(msg), error(err) {
    }

    static void check(herr_t result, const std::string &msg_if_fail) {
        if (result < 0) {
            throw H5Error(result, msg_if_fail);
        }
    }

private:
    herr_t      error;
};


} // nix::hdf5
} // nix::


#endif