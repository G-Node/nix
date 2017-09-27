// Copyright (c) 2017, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <ckellner@redhat.com>

#ifndef NIX_I_DATA_FRAME_H
#define NIX_I_DATA_FRAME_H

#include <nix/base/IEntityWithSources.hpp>

#include <string>
#include <vector>

namespace nix {


class NIXAPI Column {
 public:
    std::string   name;
    std::string   unit;
    nix::DataType dtype;
};


namespace base {

class NIXAPI IDataFrame : virtual public base::IEntityWithSources {
public:


    virtual std::vector<Column> columns() const = 0;

};

}

template<>
struct objectToType<nix::base::IDataFrame> {
    static const bool isValid = true;
    static const ObjectType value = ObjectType::DataFrame;
};

}

#endif
