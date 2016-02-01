// Copyright (c) 2013 - 2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_DIRECTORY_H
#define NIX_DIRECTORY_H

#include <boost/filesystem.hpp>
#include "AttributesFS.hpp"
#include <nix/File.hpp>

#include <string>
#include <vector>

namespace nix {
namespace file {

class Directory {

private:
    boost::filesystem::path loc;
    FileMode mode;

    void open_or_create();

public:
    Directory () {};

    Directory (const boost::filesystem::path &location, FileMode mode = FileMode::ReadOnly);

    Directory (const std::string &location, FileMode mode = FileMode::ReadOnly);

    std::string location() const;

    FileMode fileMode() const;

    ndsize_t subdirCount() const;

    boost::filesystem::path sub_dir_by_index(ndsize_t index) const;

    bool hasObject(const std::string &name) const;

    boost::optional<boost::filesystem::path> findByNameOrAttribute(const std::string &attribute, const std::string &value) const;

    bool removeObjectByNameOrAttribute(const std::string &attribute, const std::string &name_or_id) const;

    void createDirectoryLink(const std::string &target, const std::string &name);

    void renameSubdir(const std::string &old_name, const std::string &new_name);

    bool isValid() const;

    virtual void removeAll();
};

}
}

#endif //NIX_DIRECTORY_H
