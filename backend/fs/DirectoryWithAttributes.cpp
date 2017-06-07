// Copyright (c) 2013 - 2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <iostream>
#include "DirectoryWithAttributes.hpp"

namespace bfs = boost::filesystem;

namespace nix {
namespace file {

DirectoryWithAttributes::DirectoryWithAttributes(const bfs::path &location, FileMode mode, bool checkHeader)
    : Directory(location, mode) {
    if (checkHeader && mode < FileMode::ReadWrite) {
        if (!bfs::exists(location / bfs::path("attributes"))) {
           throw nix::InvalidFile("DirectoryWithAttributes");
        } else {
            attributes = AttributesFS(location, mode);
            bool check = true;
            std::vector<int> version;
            std::string str;
            getAttr("format", str);
            check = str == FILE_FORMAT;
            getAttr("version", version);
            check = check && version == FILE_VERSION;
            if (!check) {
                throw nix::InvalidFile("DirectoryWithAttributes");
            }
        }
    } else {
        attributes = AttributesFS(location, mode);
    }
}

DirectoryWithAttributes::DirectoryWithAttributes(const std::string &location, FileMode mode, bool checkHeader)
    : DirectoryWithAttributes(bfs::path(location.c_str()), mode, checkHeader)
{
}

bool DirectoryWithAttributes::hasAttr(const std::string &name) const  {
    return attributes.has(name);
}

void DirectoryWithAttributes::removeAttr(const std::string &name) {
    attributes.remove(name);
}

void DirectoryWithAttributes::removeAll() {
    Directory::removeAll();
    attributes = AttributesFS(location(), fileMode());
}

void DirectoryWithAttributes::createLink(const bfs::path &linker) {
    std::vector<std::string> links;
    if (hasAttr("links")) {
        getAttr("links", links);
    }
    bfs::create_directory_symlink(bfs::path(location()), linker);
    links.push_back(linker.string());
    setAttr("links", links);
}

void DirectoryWithAttributes::unlink(const bfs::path &linker) {
    std::vector<std::string> links, active_links;
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


bool DirectoryWithAttributes::isValid() const {
    bfs::path attr_path = "attributes";
    return  bfs::exists(location()) &&
            bfs::exists(location() / attr_path);
}


} // nix::file
} // nix
