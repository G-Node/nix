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
#include <nix/NDSize.hpp>

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

    virtual nix::ndsize_t rows() const = 0;
    virtual void rows(nix::ndsize_t n) = 0;

    virtual std::vector<Column> columns() const = 0;
    virtual void writeCell(ndsize_t row, ndsize_t col, const Variant &v) = 0;
    virtual void writeRow(ndsize_t row, const std::vector<Variant> &v) = 0;
};

}

template<>
struct objectToType<nix::base::IDataFrame> {
    static const bool isValid = true;
    static const ObjectType value = ObjectType::DataFrame;
};

}

#endif
