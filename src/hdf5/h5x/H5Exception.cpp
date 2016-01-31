// Copyright © 2015 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <kellner@bio.lmu.de>

#include "H5Exception.hpp"
#include <nix/Exception.hpp>
#include <nix/util/util.hpp>

#include <sstream>

namespace nix {
namespace hdf5 {
namespace check {

void check_h5_arg_name_loc(const std::string &name, const SourceLocation &loc) {
    if (name.find("/") != std::string::npos) {
        std::stringstream stream;
        stream << "Invalid Name: " << name << " contains a '/'!";
        stream << " [" << loc.funcsig << " in " << loc.filepath << ": " << loc.fileline << "]";
        throw InvalidName(stream.str());
    }
}


} //nix::hdf5::check
} //nix::hdf5
} //nix