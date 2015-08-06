// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
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


ndsize_t Directory::subdir_count() const {
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


boost::filesystem::path Directory::sub_dir_by_index(ndsize_t index) const {
    path p;
    vector<path> paths;
    copy(directory_iterator(loc), directory_iterator(), back_inserter(paths));
    sort(paths.begin(), paths.end());
    if (index < paths.size())
        p = paths[index];
    return p;
}

} // nix::file
} // nix