// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <iostream>
#include <nix/file/Directory.hpp>
#include "../../include/nix/file/Directory.hpp"
#include "../../include/nix/base/IFile.hpp"

using namespace std;

using namespace boost::filesystem;

namespace nix {
namespace file {

Directory::Directory(const path &parent, const std::string &name, FileMode mode)
    : loc(parent / path(name.c_str())), mode(mode) {
    open_or_create();
    attributes = AttributesFS(loc, mode);
}

Directory::Directory(const string &parent, const string &name, FileMode mode): Directory(path(parent.c_str()), name, mode) {}

void Directory::open_or_create() {
    if (!exists(loc)) {
        if (mode > FileMode::ReadOnly) {
            create_directories(loc);
        } else {
            throw std::logic_error("Trying to create new directory in ReadOnly mode!");
        }
    }
}

bool Directory::hasAttr(const string &name) const  {
    return attributes.has(name);
}

void Directory::removeAttr(const string &name) {
    attributes.remove(name);
}

string Directory::location() const {
    return loc.string();
}

} // nix::file
} // nix