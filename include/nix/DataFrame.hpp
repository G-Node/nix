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

    /**
     * @brief Returns the number of rows in the DataFrame.
     *
     * @return The current number of rows.
     */
    ndsize_t rows() const {
        return backend()->rows();
    }

    /**
     * @brief Resize the number of rows of the DataFrame.
     *
     * @param n     The new  number of rows.
     */
    void rows(ndsize_t n) {
        return backend()->rows(n);
    }

    /**
     * @brief Resolve column names to column indices.
     *
     * @param names     The names to resolve.
     *
     * @return The resolved column indices.
     */
    std::vector<unsigned> colIndex(const std::vector<std::string> &names) const {
        return backend()->colIndex(names);
    }

    /**
     * @brief Resolve column indices to column names.
     *
     * @param cols     The indices to resolve.
     *
     * @return The resolved column names.
     */
    std::vector<std::string> colName(const std::vector<unsigned> &cols) const {
        return backend()->colName(cols);
    }

    /**
     * @brief Resolve a column name to a column index.
     *
     * @param name     The name to resolve.
     *
     * @return The resolved column index.
     */
    unsigned colIndex(const std::string &name) const {
        return backend()->colIndex(name);
    }

    /**
     * @brief Resolve column index to a column name.
     *
     * @param col     The index to resolve.
     *
     * @return The resolved column name.
     */
    std::string colName(unsigned col) const {
        return backend()->colName(col);
    }

    /**
     * @brief Returns the description of the columns of the DataFrame.
     *
     * @return A std::vector containing Column objects.
     */
    std::vector<Column> columns() const {
        return backend()->columns();
    }

    /**
     * @brief Write a complete row, overwriting any existing data.
     *
     * @param row     Index of the row to write to.
     * @param v       std::vector of nix::Variant objects
     */
    void writeRow(ndsize_t row, const std::vector<Variant> &v) {
        return backend()->writeRow(row, v);
    }

    /**
     * @brief Write a single cell.
     *
     * @param row     Index of the row to write to.
     * @param col     Index of the column to write to.
     * @param v       nix::Variant containing the data.
     */
    void writeCell(ndsize_t row, unsigned col, const Variant &v) {
        return this->writeCells(row, {{col, v}});
    }

    /**
     * @brief Write multiple cells of the same row.
     *
     * @param row     Index of the row to write to.
     * @param cells   std::vector of {@link nix::Cell} object. Either
     *                the column index or the column name must be set
     *                in the cell object.
     */
    void writeCells(ndsize_t row, const std::vector<Cell> &cells) {
        return backend()->writeCells(row, cells);
    }

    /**
     * @brief Read a single cell.
     *
     * @param row     Index of the row to read from.
     * @param col     Column index of the cell to read.
     *
     * @return A {@link nix::Cell} with the cell's data.
     */
    Cell readCell(ndsize_t row, unsigned col) const {
        const std::string name = colName(col);
        return this->readCell(row, name);
    }

    /**
     * @brief Read a single cell.
     *
     * @param row     Index of the row to read from.
     * @param col     Column name of the cell to read.
     *
     * @return A {@link nix::Cell} with the cell's data.
     */
    Cell readCell(ndsize_t row, const std::string &col) const {
        std::vector<Cell> cells = backend()->readCells(row, {col});

        if (cells.size() < 1) {
            return Cell{};
        }

        return cells[0];
    }

    /**
     * @brief Read multiple cells of the same row.
     *
     * @param row     Index of the row to read from.
     * @param cols    Names of the cells to read.
     *
     * @return A std::vector of {@link nix::Cell} with the cells' data.
     */
    std::vector<Cell> readCells(ndsize_t row, const std::vector<std::string> &cols) const {
        return backend()->readCells(row, cols);
    }

    /**
     * @brief Read an entire row of the DataFrame.
     *
     * @param row     Index of the row to read from.
     *
     * @return A std::vector of {@link nix::Variant} containing the row data.
     */
    std::vector<Variant> readRow(ndsize_t row) {
        return backend()->readRow(row);
    }

    /**
     * @brief Write column data.
     *
     * @param name    Name of the column to write.
     * @param vals    A std::vector with the data to write.
     * @param offset  Where to start.
     * @param count   How many columns to write.
     */
    template<typename T>
    void writeColumn(const std::string &name,
                     const std::vector<T> &vals,
                     ndsize_t offset = 0,
                     ndsize_t count = 0) {
        const Hydra<const std::vector<T>> hydra(vals);

        if (count == 0)
            count = vals.size();
        else if (count > vals.size())
            throw OutOfBounds("Requested to write more data than available");

        DataType dtype = hydra.element_data_type();
        backend()->writeColumn(name, offset, count, dtype, (const char *) hydra.data());
    }

    /**
     * @brief Write column data.
     *
     * @param col     The index of the column to write.
     * @param vals    A std::vector with the data to write.
     * @param offset  Where to start.
     * @param count   How many columns to write.
     */
    template<typename T>
    void writeColumn(unsigned col,
                     const std::vector<T> &vals,
                     ndsize_t offset = 0,
                     ndsize_t count = 0) {
        const std::string name = this->colName(col);
        writeColumn(name, vals, offset, count);
    }

    /**
     * @brief Read column data.
     *
     * @param name    Name of the column to read.
     * @param vals    A std::vector to store the data in
     * @param resize  Resize the vector to fit the data;
     *                if false the size of the vector is used
     *                as indication how much data to read.
     * @param offset  Where to start.
     */
    template<typename T>
    void readColumn(const std::string &name,
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
        return this->readColumn<T>(name, vals, count, resize, offset);
    }

    /**
     * @brief Read column data.
     *
     * @param col     Index of the column to read from.
     * @param vals    A std::vector to store the data in
     * @param resize  Resize the vector to fit the data;
     *                if false the size of the vector is used
     *                as indication how much data to read.
     * @param offset  Where to start.
     */
    template<typename T>
    void readColumn(unsigned col,
                    std::vector<T> &vals,
                    bool resize = false,
                    ndsize_t offset = 0) {
        const std::string name = this->colName(col);
        readColumn(name, vals, resize, offset);
    }

    /**
     * @brief Read column data.
     *
     * @param name    Name of the column to read.
     * @param vals    A std::vector to store the data in
     * @param count   How many rows to read.
     * @param resize  Resize the vector to fit the data
     * @param offset  Which row to start reading.
     */
    template<typename T>
    void readColumn(const std::string &name,
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
        backend()->readColumn(name, offset, count, dtype, hydra.data());
    }

    /**
     * @brief Read column data.
     *
     * @param col     Index of the column to read data from.
     * @param vals    A std::vector to store the data in
     * @param count   How many rows to read.
     * @param resize  Resize the vector to fit the data
     * @param offset  Which row to start reading.
     */
    template<typename T>
    void readColumn(unsigned col,
                    std::vector<T> &vals,
                    size_t count,
                    bool resize,
                    ndsize_t offset = 0) {
        const std::string name = this->colName(col);
        readColumn(name, vals, count, resize, offset);
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
