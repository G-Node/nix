// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/Group.hpp>
#include <nix/util/util.hpp>

using namespace nix;

bool Group::hasDataArray(const DataArray &data_array) const {
    util::checkEntityInput(data_array);
    return backend()->hasDataArray(data_array.id());
}
