// Copyright (c) 2017, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_OBJECT_TYPE_H
#define NIX_OBJECT_TYPE_H

namespace nix {

enum class ObjectType {
    Unknown     = 0,

    File        = 1,
    Block,
    DataArray,
    Tag,
    Source,
    Feature,
    MultiTag,
    Section,
    Property,
    Group,

    // Use value > 0x80 for entities that do NOT
    // inherit from nix::base::IEntity

    SetDimension      = 0x81,
    SampledDimension,
    RangeDimension
};


template<typename T>
struct objectToType { };

}

#endif //NIX_OBJECT_TYPE_H
