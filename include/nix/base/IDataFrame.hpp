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


struct Cell {

    Cell(const std::string &name, const Variant &value) :
        col(-1), name(name), value(value)
    {}

    Cell(ndsize_t col, const Variant &value) :
        col(col), value(value)
    {}

    Cell(const std::string &name, const char *str) :
        col(-1), name(name), value(Variant(str))
    {}

    template<typename T>
    Cell(const std::string &name, const T &value) :
        col(-1), name(name), value(Variant{value})
    {}

    template<typename T>
    Cell(ndsize_t col, const T &value) :
        col(col), value(Variant{value})
    {}

    Cell(const Cell &other) :
        col(other.col), name(other.name), value(other.value)
    {}

    bool haveName() const { return !name.empty(); }
    bool haveIndex() const { return col > -1; }

    const ndssize_t col;
    const std::string name;
    const Variant value;
};


namespace base {

class NIXAPI IDataFrame : virtual public base::IEntityWithSources {
public:

    virtual nix::ndsize_t rows() const = 0;
    virtual void rows(nix::ndsize_t n) = 0;

    virtual std::vector<Column> columns() const = 0;
    virtual void writeCell(ndsize_t row, ndsize_t col, const Variant &v) = 0;
    virtual void writeCells(ndsize_t row, const std::vector<Cell> &cells) = 0;
    virtual void writeRow(ndsize_t row, const std::vector<Variant> &v) = 0;

    virtual Variant readCell(ndsize_t row, ndsize_t col) = 0;
    virtual std::vector<Variant> readRow(ndsize_t row) = 0;

    virtual void writeColumn(ndsize_t col,
                             ndsize_t offset,
                             ndsize_t count,
                             DataType dtype,
                             const char *data) = 0;

    virtual void readColumn(ndsize_t col,
                            ndsize_t offset,
                            ndsize_t count,
                            DataType dtype,
                            void *data) = 0;
};

}

template<>
struct objectToType<nix::base::IDataFrame> {
    static const bool isValid = true;
    static const ObjectType value = ObjectType::DataFrame;
};

}

#endif
