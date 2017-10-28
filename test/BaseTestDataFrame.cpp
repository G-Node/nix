// Copyright © 2017 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <ckellner@redhat.com>

#include <iostream>
#include <sstream>
#include <iterator>
#include <stdexcept>
#include <limits>

#include "BaseTestDataFrame.hpp"

#include <cppunit/extensions/HelperMacros.h>


void BaseTestDataFrame::testBasic() {
    std::vector<nix::Column> cols = {
        {"bool", "", nix::DataType::Bool},
        {"int32", "V", nix::DataType::Int32},
        {"uint32", "V", nix::DataType::UInt32},
        {"int64", "A", nix::DataType::Int64},
        {"uint64", "A", nix::DataType::UInt64},
        {"string", "", nix::DataType::String},
        {"double", "mV", nix::DataType::Double}};

    nix::DataFrame f1 = block.createDataFrame("isd", "isd", cols);

    std::vector<std::string> names(cols.size());

    std::transform(cols.cbegin(), cols.cend(), names.begin(),
                   [](const nix::Column &c) {
                       return c.name;
                   });

    std::vector<unsigned> idx = f1.colIndex(names);
    std::vector<std::string> resolved = f1.colName(idx);

    CPPUNIT_ASSERT_EQUAL(cols.size(), idx.size());
    CPPUNIT_ASSERT_EQUAL(cols.size(), resolved.size());

    for (size_t i = 0; i < cols.size(); i++) {
        CPPUNIT_ASSERT_EQUAL(static_cast<unsigned>(i), idx[i]);
        CPPUNIT_ASSERT_EQUAL(cols[i].name, resolved[i]);
    }

    std::vector<nix::Column> cs = f1.columns();
    CPPUNIT_ASSERT_EQUAL(cols.size(), cs.size());

    for (size_t i = 0; i < cols.size(); i++) {
        CPPUNIT_ASSERT_EQUAL(cols[i].name, cs[i].name);
        CPPUNIT_ASSERT_EQUAL(cols[i].unit, cs[i].unit);
        CPPUNIT_ASSERT_EQUAL(cols[i].dtype, cs[i].dtype);
    }

    for (int i = 0; i < static_cast<int>(nix::DataType::Opaque); i++) {
        nix::DataType dt = static_cast<nix::DataType>(i);
        if (nix::Variant::supports_type(dt))
            continue;

        std::vector<nix::Column> wrong_col = {
            {"peng", "", dt}
        };

        CPPUNIT_ASSERT_THROW(block.createDataFrame("peng",
                                                   "peng",
                                                   wrong_col),
                             std::invalid_argument);
    }

}

nix::DataFrame createStandardFrame(nix::Block b) {
        std::vector<nix::Column> cols = {
        {"int32", "V", nix::DataType::Int32},
        {"string", "", nix::DataType::String},
        {"double", "mV", nix::DataType::Double}};

    return b.createDataFrame("frame", "frame", cols);
}

void BaseTestDataFrame::testRowIO() {
    nix::DataFrame df = createStandardFrame(block);

    std::vector<nix::Variant> vals = {nix::Variant(10),
                                      nix::Variant("test"),
                                      nix::Variant(42.0)};

    df.rows(1);

    CPPUNIT_ASSERT_EQUAL(nix::ndsize_t(1), df.rows());
    df.writeRow(0, vals);

    std::vector<nix::Variant> rr = df.readRow(0);
    for (size_t i = 0; i < vals.size(); i++) {
        CPPUNIT_ASSERT_EQUAL(rr[i], vals[i]);
    }

    // add a new row with a changed value
    df.rows(2);
    CPPUNIT_ASSERT_EQUAL(nix::ndsize_t(2), df.rows());
    vals[0].set(67);
    df.writeRow(1, vals);

    rr = df.readRow(1);
    for (size_t i = 0; i < vals.size(); i++) {
        CPPUNIT_ASSERT_EQUAL(rr[i], vals[i]);
    }

    // overwrite the first row
    df.writeRow(0, vals);

    rr = df.readRow(0);
    for (size_t i = 0; i < vals.size(); i++) {
        CPPUNIT_ASSERT_EQUAL(rr[i], vals[i]);
    }

}

void BaseTestDataFrame::testColIO() {
    nix::DataFrame df = createStandardFrame(block);
    size_t n = 10;

    df.rows(n);

    CPPUNIT_ASSERT_EQUAL(nix::ndsize_t(n), df.rows());

    std::vector<int32_t> i32(n);
    std::vector<std::string> str(n);
    std::vector<double> dbl(n);

    for (size_t i = 0; i < n; i++) {
        std::stringstream buf;

        i32[i] = static_cast<int32_t>(n);

        buf << i;
        str[i] = buf.str();

        dbl[i] = i / static_cast<double>(n);
    }

    df.writeColumn(0, i32);
    df.writeColumn(1, str);
    df.writeColumn(2, dbl);


    std::vector<int32_t> i32_out(n);
    std::vector<std::string> str_out(n);
    std::vector<double> dbl_out(n);

    df.readColumn(0, i32_out);
    df.readColumn(1, str_out);
    df.readColumn(2, dbl_out);

    for (size_t i = 0; i < n; i++) {
        CPPUNIT_ASSERT_EQUAL(i32[i], i32_out[i]);
        CPPUNIT_ASSERT_EQUAL(str[i], str_out[i]);
        CPPUNIT_ASSERT_EQUAL(dbl[i], dbl_out[i]);
    }

    size_t count = 5;

    std::vector<int32_t> i32_rs(1);
    std::vector<std::string> str_rs(1);
    std::vector<double> dbl_rs(1);

    df.readColumn(0, i32_rs, count, true);
    df.readColumn(1, str_rs, count, true);
    df.readColumn(2, dbl_rs, count, true);

    for (size_t i = 0; i < count; i++) {
        CPPUNIT_ASSERT_EQUAL(count, i32_rs.size());
        CPPUNIT_ASSERT_EQUAL(count, str_rs.size());
        CPPUNIT_ASSERT_EQUAL(count, dbl_rs.size());

        CPPUNIT_ASSERT_EQUAL(i32[i], i32_rs[i]);
        CPPUNIT_ASSERT_EQUAL(str[i], str_rs[i]);
        CPPUNIT_ASSERT_EQUAL(dbl[i], dbl_rs[i]);
    }

}

void BaseTestDataFrame::testCellIO() {
    nix::DataFrame df = createStandardFrame(block);

    df.rows(1);
    CPPUNIT_ASSERT_EQUAL(nix::ndsize_t(1), df.rows());

    df.writeCell(0, 0, nix::Variant(666));

    nix::Variant v = df.readCell(0, 0);
    CPPUNIT_ASSERT_EQUAL(nix::Variant(666), v);

    std::vector<nix::Cell> vals = {
        {"double", nix::Variant{77.7}},
        {"string", nix::Variant{"foobar"}}
    };

    df.writeCells(0, vals);

    std::vector<std::string> names(vals.size());

    std::transform(vals.cbegin(), vals.cend(), names.begin(),
                   [](const nix::Cell &c) {
                       return c.name;
                   });

    std::vector<nix::Cell> out = df.readCells(0, names);

    for (size_t i = 0; i < vals.size(); i++) {
        CPPUNIT_ASSERT_EQUAL(vals[i], out[i]);
        CPPUNIT_ASSERT_EQUAL(vals[i].name, out[i].name);
    }

}
