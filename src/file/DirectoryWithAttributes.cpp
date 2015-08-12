// Copyright (c) 2013 - 2015, German Neuroinformatics Node (G-Node)
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

void DirectoryWithAttributes::removeAll() {
    Directory::removeAll();
    attributes = AttributesFS(location(), fileMode());
}

void DirectoryWithAttributes::createLink(const boost::filesystem::path &linker) {
    vector<string> links;
    if (hasAttr("links")) {
        getAttr("links", links);
    }
    create_directory_symlink(path(location()), linker);
    links.push_back(linker.string());
    setAttr("links", links);
}

void DirectoryWithAttributes::unlink(const boost::filesystem::path &linker) {
    vector<string> links, active_links;
    if (hasAttr("links")) {
        getAttr("links", links);
    }
    for (auto &l : links) {
        if (l != linker.string()) {
            active_links.push_back(l);
        }
    }
    setAttr("links", active_links);
}

} // nix::file
} // nix