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
        col(0), name(name), value(value)
    {}

    Cell(unsigned col, const Variant &value) :
        col(col), value(value)
    {}

    Cell(const std::string &name, const char *str) :
        col(0), name(name), value(Variant(str))
    {}

    template<typename T>
    Cell(const std::string &name, const T &value) :
        col(0), name(name), value(Variant{value})
    {}

    template<typename T>
    Cell(int col, const T &value) :
        col(col), value(Variant{value})
    {}

    Cell(const Cell &other) :
        col(other.col), name(other.name), value(other.value)
    {}

    Cell(Cell &&other) :
        col(other.col),
        name(std::move(other.name)),
        value(std::move(other.value))
    {}

    Cell& operator=(Cell other) {
        swap(other);
        return *this;
    }

    void swap(Cell &other) {
        using std::swap;
        swap(other.col, col);
        swap(other.name, name);
        swap(other.value, value);
    }

    bool haveName() const { return !name.empty(); }
    bool haveIndex() const { return name.empty(); }

    int col;
    std::string name;
    Variant value;
};


namespace base {

class NIXAPI IDataFrame : virtual public base::IEntityWithSources {
public:

    virtual nix::ndsize_t rows() const = 0;
    virtual void rows(nix::ndsize_t n) = 0;

    virtual std::vector<Column> columns() const = 0;

    virtual std::vector<unsigned> colIndex(const std::vector<std::string> &names) const {
        std::vector<unsigned> indices(names.size());
        for (size_t i = 0; i < names.size(); i++) {
            indices[i] = colIndex(names[i]);
        }
        return indices;
    }

    virtual std::vector<std::string> colName(const std::vector<unsigned> &cols) const {
        std::vector<std::string> names(cols.size());
        for (size_t i = 0; i < cols.size(); i++) {
            names[i] = colName(cols[i]);
        }
        return names;
    }

    virtual unsigned colIndex(const std::string &name) const = 0;
    virtual std::string colName(unsigned col) const = 0;

    virtual std::vector<Variant> readRow(ndsize_t row) const = 0;
    virtual void writeRow(ndsize_t row, const std::vector<Variant> &v) = 0;

    virtual std::vector<Variant> readCells(ndsize_t row, const std::vector<std::string> &names) const = 0;
    virtual void writeCells(ndsize_t row, const std::vector<Cell> &cells) = 0;


    virtual void readColumn(int col,
                            ndsize_t offset,
                            ndsize_t count,
                            DataType dtype,
                            void *data) const = 0;

    virtual void writeColumn(int col,
                             ndsize_t offset,
                             ndsize_t count,
                             DataType dtype,
                             const void *data) = 0;

};

}

template<>
struct objectToType<nix::base::IDataFrame> {
    static const bool isValid = true;
    static const ObjectType value = ObjectType::DataFrame;
};

}

#endif
