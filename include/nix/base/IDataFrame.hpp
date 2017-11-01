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
#include <nix/Variant.hpp>

#include <string>
#include <vector>

namespace nix {


class NIXAPI Column {
 public:
    std::string   name;
    std::string   unit;
    nix::DataType dtype;
};


struct Cell : public nix::Variant {

    Cell() : Variant(), col(0), name("")
    {}

    Cell(const std::string &name, const Variant &value) :
        Variant(value), col(0), name(name)
    {}

    Cell(unsigned col, const Variant &value) :
        Variant(value), col(col)
    {}

    Cell(const std::string &name, const char *str) :
        Variant(str), col(0), name(name)
    {}

    template<typename T>
    Cell(const std::string &name, const T &value) :
        Variant(value), col(0), name(name)
    {}

    template<typename T>
    Cell(int col, const T &value) :
        Variant(value), col(col)
    {}

    Cell(const Cell &other) :
        Variant(static_cast<const Variant &>(other)),
        col(other.col),
        name(other.name)
    {}

    Cell(Cell &&other) NOEXCEPT :
        Variant(std::forward<Variant>(other)),
        col(other.col),
        name(std::move(other.name))
    {}

    Cell& operator=(Cell other) {
        swap(other);
        return *this;
    }

    void swap(Cell &other) {
        using std::swap;
        Variant::swap(other);
        swap(other.col, col);
        swap(other.name, name);
    }

    bool haveName() const { return !name.empty(); }
    bool haveIndex() const { return name.empty(); }

    int col;
    std::string name;
};


namespace base {

class NIXAPI IDataFrame : virtual public base::IEntityWithSources {
public:

    virtual nix::ndsize_t rows() const = 0;
    virtual void rows(nix::ndsize_t n) = 0;

    virtual std::vector<Column> columns() const = 0;

    virtual std::vector<unsigned> colIndex(const std::vector<std::string> &names) const = 0;
    virtual std::vector<std::string> colName(const std::vector<unsigned> &cols) const = 0;

    virtual unsigned colIndex(const std::string &name) const = 0;
    virtual std::string colName(unsigned col) const = 0;

    virtual std::vector<Variant> readRow(ndsize_t row) const = 0;
    virtual void writeRow(ndsize_t row, const std::vector<Variant> &v) = 0;

    virtual std::vector<Cell> readCells(ndsize_t row, const std::vector<std::string> &names) const = 0;
    virtual void writeCells(ndsize_t row, const std::vector<Cell> &cells) = 0;


    virtual void readColumn(const std::string &name,
                            ndsize_t offset,
                            ndsize_t count,
                            DataType dtype,
                            void *data) const = 0;

    virtual void writeColumn(const std::string &name,
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
