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

    std::vector<unsigned> colIndex(const std::vector<std::string> &names) const {
        return backend()->colIndex(names);
    }

    std::vector<std::string> colName(const std::vector<unsigned> &cols) const {
        return backend()->colName(cols);
    }

    unsigned colIndex(const std::string &name) const {
        return backend()->colIndex(name);
    }

    std::string colName(unsigned col) const {
        return backend()->colName(col);
    }

    std::vector<Column> columns() const {
        return backend()->columns();
    }

    void writeRow(ndsize_t row, const std::vector<Variant> &v) {
        return backend()->writeRow(row, v);
    }

    void writeCell(ndsize_t row, unsigned col, const Variant &v) {
        return this->writeCells(row, {{col, v}});
    }

    void writeCells(ndsize_t row, const std::vector<Cell> &cells) {
        return backend()->writeCells(row, cells);
    }

    Variant readCell(ndsize_t row, unsigned col) const {
        const std::vector<std::string> &cols = colName({col});
        if (cols.size() < 1) {
            return Variant{};
        }
        return this->readCell(row, cols[0]);
    }

    Variant readCell(ndsize_t row, const std::string &col) const {
        std::vector<Variant> cells = backend()->readCells(row, {col});
        if (cells.size() < 1) {
            return Variant{};
        }

        return cells[0];
    }

    std::vector<Variant> readCells(ndsize_t row, const std::vector<std::string> &cols) const {
        return backend()->readCells(row, cols);
    }

    std::vector<Variant> readRow(ndsize_t row) {
        return backend()->readRow(row);
    }

    template<typename T>
    void writeColumn(int col,
                     const std::vector<T> &vals,
                     ndsize_t offset = 0,
                     ndsize_t count = 0) {
        const Hydra<const std::vector<T>> hydra(vals);

        if (count == 0)
            count = vals.size();
        else if (count > vals.size())
            throw OutOfBounds("Requested to write more data than available");

        DataType dtype = hydra.element_data_type();
        backend()->writeColumn(col, offset, count, dtype, (const char *) hydra.data());
    }

    template<typename T>
    void readColumn(int col,
                    std::vector<T> &vals,
                    bool resize = false,
                    ndsize_t offset = 0) {
        Hydra<std::vector<T>> hydra(vals);

        size_t count = vals.size();
        if (resize) {
            ndsize_t n_rows = rows();
            if (offset > n_rows) {
                throw OutOfBounds("offset > number of rows");
            }

            // >= 0, as per the check above
            n_rows -= offset;

            count = check::fits_in_size_t(n_rows, "n > sizeof(size_t)");
        }

        // OOB check done by called readColumn below
        return this->readColumn<T>(col, vals, count, resize, offset);
    }

    template<typename T>
    void readColumn(int col,
                    std::vector<T> &vals,
                    size_t count,
                    bool resize,
                    ndsize_t offset = 0) {
        Hydra<std::vector<T>> hydra(vals);

        if (resize) {
            vals.resize(count);
        } else if (count > vals.size()) {
            throw OutOfBounds("Vector not big enough for requested data");
        }

        DataType dtype = hydra.element_data_type();
        backend()->readColumn(col, offset, count, dtype, hydra.data());
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
