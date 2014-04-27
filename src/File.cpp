// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.


#include <nix.hpp>
#include <nix/hdf5/FileHDF5.hpp>

using namespace std;

namespace nix {


File File::open(const std::string name, FileMode mode, Implementation impl) {
    if (impl == Implementation::Hdf5) {
        return File(std::make_shared<hdf5::FileHDF5>(name, mode));
    } else {
        throw runtime_error("Unknown implementation!");
    }
}


vector<Section> File::findSections(function<bool(const Section&)> filter, size_t max_depth) const {

    vector<Section> results;

    vector<Section> roots = sections();
    for (auto it = roots.begin(); it != roots.end(); ++it) {

        vector<Section> secs = it->findSections(filter, max_depth);
        results.insert(results.end(), secs.begin(), secs.end());
    }

    return results;
}


}
