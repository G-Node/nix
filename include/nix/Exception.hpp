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

namespace nix {

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

    const char *what() const noexcept {
        return msg.c_str();
    }

private:
    size_t index;
    std::string msg;
};

}


#endif
