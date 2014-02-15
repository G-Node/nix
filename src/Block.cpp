// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.


#include <nix.hpp>
#include <nix/hdf5/BlockHDF5.hpp>

using namespace std;

namespace nix {

    /**
     * Go through the tree of sources originating from every source in this 
     * block until a max. level of "max_depth" and check for each source
     * whether to return it depending on predicate function "filter".
     * Return resulting vector of sources, which may contain duplicates.
     * 
     * @param object filter function of type std::function<bool(const Source &)>
     * @param int maximum depth to search tree
     * @return object vector of sources
     */
    std::vector<Source> Block::findSources(
                           std::function<bool(const Source &)> filter, 
                           size_t max_depth) const
    {
        vector<Source> probes = sources();
        vector<Source> matches;
        vector<Source> result;

        for (typename std::vector<Source>::iterator it = probes.begin(); it!=probes.end(); ++it)
        {
            matches = it->findSources(filter, max_depth);
			result.insert( result.end(), matches.begin(), matches.end() );
        }

        return result;
    }

}
