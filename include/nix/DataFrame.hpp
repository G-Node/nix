// Copyright (c) 2017, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <ckellner@redhat.com>

#ifndef NIX_DATA_FRAME_H
#define NIX_DATA_FRAME_H

#include <nix/Platform.hpp>
#include <nix/DataType.hpp>

#include <nix/base/EntityWithSources.hpp>
#include <nix/base/IDataFrame.hpp>

#include <nix/Hydra.hpp>

#include <string>
#include <vector>

namespace nix {

class NIXAPI DataFrame : public base::EntityWithSources<base::IDataFrame> {
public:

    DataFrame()
        : EntityWithSources()
        {}

    DataFrame(const std::shared_ptr<base::IDataFrame> &df)
        : EntityWithSources(df)
        {}

    DataFrame(std::shared_ptr<base::IDataFrame> &&ptr)
        : EntityWithSources(std::move(ptr))
        {}

    ndsize_t rows() const {
        return backend()->rows();
    }

    void rows(ndsize_t n) {
        return backend()->rows(n);
    }

    virtual std::vector<unsigned> nameToCol(const std::vector<std::string> &names) const {
        return backend()->nameToCol(names);
    }

    virtual std::vector<std::string> colToName(const std::vector<unsigned> &cols) const {
        return backend()->colToName(cols);
    }

    std::vector<Column> columns() const {
        return backend()->columns();
    }

    void writeRow(ndsize_t row, const std::vector<Variant> &v) {
        return backend()->writeRow(row, v);
    }

    void writeCell(ndsize_t row, ndsize_t col, const Variant &v) {
        return this->writeCells(row, {{col, v}});
    }

    void writeCells(ndsize_t row, const std::vector<Cell> &cells) {
        return backend()->writeCells(row, cells);
    }

    Variant readCell(ndsize_t row, ndsize_t col) {
        return backend()->readCell(row, col);
    }

    std::vector<Variant> readRow(ndsize_t row) {
        return backend()->readRow(row);
    }

    template<typename T>
        void writeColumn(ndsize_t col,
                         ndsize_t offset,
                         const std::vector<T> &vals,
                         ndsize_t count = 0) {
        const Hydra<const std::vector<T>> hydra(vals);

        if (count == 0)
            count = vals.size();
        else if (count > vals.size())
            throw OutOfBounds("bla");

        DataType dtype = hydra.element_data_type();
        backend()->writeColumn(col, offset, count, dtype, (const char *) hydra.data());
    }

    template<typename T>
        void readColumn(ndsize_t col,
                        ndsize_t offset,
                        std::vector<T> &vals,
                        ndsize_t count = 0) {
        Hydra<std::vector<T>> hydra(vals);

        if (count == 0)
            count = vals.size();

        DataType dtype = hydra.element_data_type();
        backend()->readColumn(col, offset, count, dtype, (void *) hydra.data());
    }

};


template<>
struct objectToType<nix::DataFrame> {
    static const bool isValid = true;
    static const ObjectType value = ObjectType::DataFrame;
    typedef nix::base::IDataFrame backendType;
};


}

#endif
