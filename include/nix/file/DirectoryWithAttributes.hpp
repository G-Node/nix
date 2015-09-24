// Copyright (c) 2013 - 2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_DIRECTORYWITHATTRIBUTES_HPP
#define NIX_DIRECTORYWITHATTRIBUTES_HPP

#include <boost/filesystem.hpp>
#include <nix/file/AttributesFS.hpp>
#include <nix/file/Directory.hpp>

#include <string>
#include <vector>

namespace nix {
namespace file {

class DirectoryWithAttributes: public Directory {

private:
    mutable AttributesFS attributes;

public:
    DirectoryWithAttributes (const boost::filesystem::path &location, FileMode mode = FileMode::ReadOnly);

    DirectoryWithAttributes (const std::string &location, FileMode mode = FileMode::ReadOnly);

    template <typename T> void setAttr(const std::string &name, const T &value);

    template <typename T> void getAttr(const std::string &name, T &value) const;

    bool hasAttr(const std::string &name) const;

    void removeAttr(const std::string &name);

    void removeAll();

    void createLink(const boost::filesystem::path &linker);

    void unlink(const boost::filesystem::path &linker);
};


template <typename T>
void DirectoryWithAttributes::setAttr(const std::string &name, const T &value) {
    attributes.set(name, value);
}

template <typename T>
void DirectoryWithAttributes::getAttr(const std::string &name, T &value) const {
    attributes.get(name, value);
}

}
}


#endif //NIX_DIRECTORYWITHATTRIBUTES_HPP
