// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <iostream>
#include <nix/file/DirectoryWithAttributes.hpp>

using namespace std;

using namespace boost::filesystem;

namespace nix {
namespace file {

DirectoryWithAttributes::DirectoryWithAttributes(const path &location, FileMode mode)
    : Directory(location, mode) {
    attributes = AttributesFS(location, mode);
}

DirectoryWithAttributes::DirectoryWithAttributes(const string &location, FileMode mode)
    : DirectoryWithAttributes(path(location.c_str()), mode)
{
}

bool DirectoryWithAttributes::hasAttr(const string &name) const  {
    return attributes.has(name);
}

void DirectoryWithAttributes::removeAttr(const string &name) {
    attributes.remove(name);
}

} // nix::file
} // nix