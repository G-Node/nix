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


// The single include point of HDF5
//
// A bit of a hack, but we directly include 
// hdf5's H5version.h to bypass any potetional
// H5_USE_16_API defines set in H5pubconf.h
// especially on old hdf5 versions (ubuntu precise)
#undef H5_USE_16_API
#include <H5version.h>

#include <hdf5.h>

#include <stdexcept>
#include <string>

namespace nix {
namespace hdf5 {

class NIXAPI H5Exception : public std::exception {
public:
    H5Exception(const std::string &message)
            : msg(message) {

    }
    
    H5Exception(const std::string &message, herr_t error) 
            : msg(message) {
        msg += " Error code is: " + std::to_string(error); 
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
    : H5Exception(msg, err), error(err) {
    }

    static void check(herr_t result, const std::string &msg_if_fail) {
        if (result < 0) {
            throw H5Error(result, msg_if_fail);
        }
    }

    herr_t code(void) {
        return error;
    }

private:
    herr_t      error;
};

namespace check {

struct SourceLocation {

    SourceLocation(const std::string &fp, int fl, const std::string &fs) :
            filepath(fp), fileline(fl), funcsig(fs) {}
    std::string filepath;
    int         fileline;
    std::string funcsig;

};

NIXAPI void check_h5_arg_name_loc(const std::string &name, const SourceLocation &location);
#define check_h5_arg_name(name__) nix::hdf5::check::check_h5_arg_name_loc(name__, {NIX_SRC_FILE, \
                                                                                   NIX_SRC_LINE, \
                                                                                   NIX_SRC_FUNC})

} //nix::hdf5::check
} // nix::hdf5
} // nix::


#endif
