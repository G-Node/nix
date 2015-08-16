// Copyright (c) 2013 - 2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <iostream>
#include <nix/file/Directory.hpp>

using namespace std;

using namespace boost::filesystem;

namespace nix {
namespace file {

Directory::Directory(const path &location, FileMode mode)
    : loc(location), mode(mode) {
    open_or_create();
}

Directory::Directory(const string &location, FileMode mode): Directory(path(location.c_str()), mode) {}

void Directory::open_or_create() {
    if (!exists(loc)) {
        if (mode > FileMode::ReadOnly) {
            create_directories(loc);
        } else {
            throw std::logic_error("Trying to create new directory in ReadOnly mode!");
        }
    }
}


string Directory::location() const {
    return loc.string();
}


FileMode Directory::fileMode() const {
    return mode;
}


ndsize_t Directory::subdirCount() const {
    ndsize_t count = 0;
    directory_iterator end;
    directory_iterator di(location().c_str());
    while (di != end) {
        if (is_directory(*di))
            count ++;
        ++di;
    }
    return count;
}


void Directory::removeAll() {
    directory_iterator end;
    directory_iterator di(location().c_str());
    while (di != end) {
        remove_all(*di);
        ++di;
    }
}


boost::filesystem::path Directory::sub_dir_by_index(ndsize_t index) const {
    path p;
    vector<path> paths;
    copy(directory_iterator(loc), directory_iterator(), back_inserter(paths));
    sort(paths.begin(), paths.end());
    if (index < paths.size())
        p = paths[index];
    return p;
}


boost::optional<path> Directory::findByNameOrAttribute(const std::string &attribute, const std::string &value) const {
    boost::optional<path> p;
    if (hasObject(value)) {
        p = location() / path(value.c_str());
        return p;
    }
    if (util::looksLikeUUID(value)) {
        path attr_path("attributes");
        directory_iterator end;
        directory_iterator di(location().c_str());
        while (di != end) {
            path temp = *di;
            if (is_directory(temp) && exists(temp / attr_path)){
                AttributesFS attr(temp);
                string s;
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
    }
    return p;
}


bool Directory::hasObject(const std::string &name) const {
    directory_iterator end;
    directory_iterator di(location().c_str());
    while (di != end) {
        path temp = *di;
        if (is_directory(temp) && temp.filename().string() == name)
            return true;
        ++di;
    }
    return false;
}

bool Directory::removeObjectByNameOrAttribute(const std::string &attribute, const std::string &name_or_id) const {
    boost::optional<path> p = findByNameOrAttribute(attribute, name_or_id);
    if (p && mode > FileMode::ReadOnly) {
        path attr_path = "attributes";
        if (exists(*p / attr_path)) {
            AttributesFS attr(*p, mode);
            if (attr.has("links")) {
                vector<string> links;
                attr.get("links", links);
                for (auto &l :links) {
                    remove_all(path(l));
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
        throw runtime_error("Directory::createLink: target does not exist");
    }

}


void Directory::renameSubdir(const std::string &old_name, const std::string &new_name) {
    path o(path(location()) / path(old_name)), n(path(location()) / path(new_name));
    if (hasObject(old_name) && ! hasObject(new_name)) {
        rename(o, n);
    }
}

} // nix::file
} // nix