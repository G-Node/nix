// Copyright (c) 2013 - 2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <iostream>
#include <nix/file/Directory.hpp>

namespace bfs = boost::filesystem;

namespace nix {
namespace file {

Directory::Directory(const bfs::path &location, FileMode mode)
    : loc(location), mode(mode) {
    open_or_create();
}

Directory::Directory(const std::string &location, FileMode mode): Directory(bfs::path(location.c_str()), mode) {}

void Directory::open_or_create() {
    if (!exists(loc)) {
        if (mode > FileMode::ReadOnly) {
            create_directories(loc);
        } else {
            throw std::logic_error("Trying to create new directory in ReadOnly mode!");
        }
    }
}


std::string Directory::location() const {
    return loc.string();
}


FileMode Directory::fileMode() const {
    return mode;
}


ndsize_t Directory::subdirCount() const {
    ndsize_t count = 0;
    bfs::directory_iterator end;
    bfs::directory_iterator di(location().c_str());
    while (di != end) {
        if (bfs::is_directory(*di))
            count ++;
        ++di;
    }
    return count;
}


void Directory::removeAll() {
    bfs::path p(location());
    for (bfs::directory_iterator end_it, it(p); it!=end_it; ++it) {
        bfs::remove_all(it->path());
    }
}


boost::filesystem::path Directory::sub_dir_by_index(ndsize_t index) const {
    bfs::path p;
    std::vector<bfs::path> paths;
    copy(bfs::directory_iterator(loc), bfs::directory_iterator(), back_inserter(paths));
    std::sort(paths.begin(), paths.end());
    if (index < paths.size())
        p = paths[index];
    return p;
}


boost::optional<bfs::path> Directory::findByNameOrAttribute(const std::string &attribute, const std::string &value) const {
    boost::optional<bfs::path> p;
    if (hasObject(value)) {
        p = location() / bfs::path(value.c_str());
        return p;
    }
    bfs::path attr_path("attributes");
    bfs::directory_iterator end;
    bfs::directory_iterator di(location().c_str());
    while (di != end) {
        bfs::path temp = *di;
        if (bfs::is_directory(temp) && exists(temp / attr_path)){
            AttributesFS attr(temp);
            std::string s;
            if (attr.has(attribute)) {
                attr.get(attribute, s);
                if (s == value) {
                    p = temp;
                    return p;
                }
            }
        }
        ++di;
    }
    return p;
}


bool Directory::hasObject(const std::string &name) const {
    bfs::directory_iterator end;
    bfs::directory_iterator di(location().c_str());
    while (di != end) {
        bfs::path temp = *di;
        if (bfs::is_directory(temp) && temp.filename().string() == name)
            return true;
        ++di;
    }
    return false;
}

bool Directory::removeObjectByNameOrAttribute(const std::string &attribute, const std::string &name_or_id) const {
    boost::optional<bfs::path> p = findByNameOrAttribute(attribute, name_or_id);
    if (p && mode > FileMode::ReadOnly) {
        bfs::path attr_path = "attributes";
        if (bfs::exists(*p / attr_path)) {
            AttributesFS attr(*p, mode);
            if (attr.has("links")) {
                std::vector<std::string> links;
                attr.get("links", links);
                for (auto &l :links) {
                    bfs::remove_all(bfs::path(l));
                }
            }
        }
        uintmax_t ret = remove_all(*p);
        return ret > 0;
    }
    return false;
}


void Directory::createDirectoryLink(const std::string &target, const std::string &name) {
    if (boost::filesystem::exists({target})) {
        boost::filesystem::create_directory_symlink(boost::filesystem::path(target), loc / boost::filesystem::path(name));
    } else {
        throw std::runtime_error("Directory::createLink: target does not exist");
    }
}


void Directory::renameSubdir(const std::string &old_name, const std::string &new_name) {
    bfs::path o(bfs::path(location()) / bfs::path(old_name)), n(bfs::path(location()) / bfs::path(new_name));
    if (hasObject(old_name) && ! hasObject(new_name)) {
        rename(o, n);
    }
}


bool Directory::isValid() const {
    return bfs::exists(location());
}

} // nix::file
} // nix
