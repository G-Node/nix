// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.


#include <nix.hpp>

using namespace std;

namespace nix {

    std::vector<Source> Block::findSources(util::Filter<Source>::type filter,
                                           size_t max_depth) const 
    {
        const vector<Source> probes = sources();
        vector<Source> matches;
        vector<Source> result;

        for (auto &probe : probes) {
            matches = probe.findSources(filter, max_depth);
            result.insert (result.end(), matches.begin(), matches.end());
        }

        return result;
    }

}
