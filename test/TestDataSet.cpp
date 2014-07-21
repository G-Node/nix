// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <type_traits>
#include "TestDataSet.hpp"

#include <nix/hdf5/Selection.hpp>

using namespace nix; //quick fix for now
using namespace nix::hdf5;

unsigned int & TestDataSet::open_mode()
{
    static unsigned int openMode = H5F_ACC_TRUNC;
    return openMode;
}


void TestDataSet::setUp() {
    unsigned int &openMode = open_mode();

    h5file = H5::H5File("test_dataset.h5", openMode);
    if (openMode == H5F_ACC_TRUNC) {
        h5group = h5file.createGroup("charon");
    } else {
        h5group = h5file.openGroup("charon");
    }
    openMode = H5F_ACC_RDWR;
}

void TestDataSet::testNDSize() {
    NDSize invalidSize = {};
    NDSize a = {23, 42, 1982};

    CPPUNIT_ASSERT(!invalidSize); // testing operator bool()
    CPPUNIT_ASSERT(a ? true : false);

    typedef NDSize::value_type value_type;

    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(23),   a[0]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(42),   a[1]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(1982), a[2]);

    CPPUNIT_ASSERT_THROW(a[3], std::out_of_range);

    a++;

    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(24),   a[0]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(43),   a[1]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(1983), a[2]);

    a--;

    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(23),   a[0]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(42),   a[1]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(1982), a[2]);

    a += 13;

    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(36),   a[0]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(55),   a[1]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(1995), a[2]);

    a -= 13;

    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(23),   a[0]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(42),   a[1]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(1982), a[2]);


    NDSize b = {19, 1940, 18};

    NDSize c = a + b;

    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(42),   c[0]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(1982), c[1]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(2000), c[2]);

    NDSize d = c - b;

    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(23),   d[0]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(42),   d[1]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(1982), d[2]);

    NDSize f = {1, 2, 3, 4};
    CPPUNIT_ASSERT_THROW(a + f, std::out_of_range);

    NDSize g(f.size(), 0);

    g += f;

    CPPUNIT_ASSERT(g == f);
    CPPUNIT_ASSERT(g != a);

    NDSize h = b / b;
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(1), h[0]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(1), h[1]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(1), h[2]);

    NDSize j(h.size(), static_cast<value_type>(333));
    NDSize k = h * j;

    CPPUNIT_ASSERT(j == k);

    size_t dp = j.dot(h);
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(999), dp);

    NDSize s = {3, 4};
    dp = s.dot(s);
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(25), dp);

}

void TestDataSet::testChunkGuessing() {

    NDSize dims = {1024, 1024};

    NDSize chunks = DataSet::guessChunking(dims, DataType::Double);
    CPPUNIT_ASSERT_EQUAL(chunks[0], 64ULL);
    CPPUNIT_ASSERT_EQUAL(chunks[1], 64ULL);
}


void TestDataSet::testDataType() {
    static struct _type_info {
        std::string name;
        nix::DataType dtype;
    } _types[] = {
        {"bool", nix::DataType::Bool},
        {"int8", nix::DataType::Int8},
        {"uint8", nix::DataType::UInt8},
        {"int16", nix::DataType::Int16},
        {"uint16", nix::DataType::UInt16},
        {"int32", nix::DataType::Int32},
        {"uint32", nix::DataType::UInt32},
        {"int64", nix::DataType::Int64},
        {"uint64", nix::DataType::UInt64},
        {"float", nix::DataType::Float},
        {"double", nix::DataType::Double},
        {"string", nix::DataType::String}
    };

    const NDSize dims = {5, 5};

    for (size_t i = 0; i < (sizeof(_types)/sizeof(_type_info)); i++) {
        DataSet ds = DataSet::create(h5group, _types[i].name, _types[i].dtype, dims);
        CPPUNIT_ASSERT_EQUAL(ds.dataType(), _types[i].dtype);
    }

}

void TestDataSet::testBasic() {
    NDSize dims = {4, 6};

    DataSet ds = DataSet::create(h5group, "dsZero", DataType::Double, {0, 0});
    CPPUNIT_ASSERT_EQUAL(ds.size(), (NDSize{0, 0}));

    ds = DataSet::create(h5group, "dsDouble", DataType::Double, dims);

    typedef boost::multi_array<double, 2> array_type;
    typedef array_type::index index;
    array_type A(boost::extents[4][6]);
    int values = 0;
    for(index i = 0; i != 4; ++i)
        for(index j = 0; j != 6; ++j)
            A[i][j] = values++;

    ds.write(A);

    array_type B(boost::extents[1][1]);
    ds.read(B, true);

    for(index i = 0; i != 4; ++i)
        for(index j = 0; j != 6; ++j)
            CPPUNIT_ASSERT_EQUAL(A[i][j], B[i][j]);

    //** check for setExtent()
    array_type C(boost::extents[8][12]);
    values = 0;
    for(index i = 0; i != 8; ++i)
        for(index j = 0; j != 12; ++j)
            C[i][j] = values++;

    dims[0] = 8;
    dims[1] = 12;

    ds.setExtent(dims);
    ds.write(C);

    array_type E(boost::extents[8][12]);
    ds.read(E);

    for(index i = 0; i != 8; ++i)
        for(index j = 0; j != 12; ++j)
             CPPUNIT_ASSERT_EQUAL(C[i][j], E[i][j]);

    NDSize newSize = {4, 6};
    ds.setExtent(newSize);
    NDSize newDSSize = ds.size();
    CPPUNIT_ASSERT_EQUAL(newSize, newDSSize);

    array_type F(boost::extents[4][6]);
    ds.read(F);
    for(index i = 0; i != 4; ++i)
        for(index j = 0; j != 6; ++j)
            CPPUNIT_ASSERT_EQUAL(E[i][j], F[i][j]);

    CPPUNIT_ASSERT_THROW(ds.setExtent({4, 6, 8}), nix::InvalidRank);
    //***

    DataSet ds2 = DataSet::create(h5group, "dsDouble2", A);
    ds2.write(A);
    array_type D(boost::extents[1][1]);
    ds2.read(D, true);

    for(index i = 0; i != 4; ++i)
        for(index j = 0; j != 6; ++j)
            CPPUNIT_ASSERT_EQUAL(A[i][j], D[i][j]);

}



void TestDataSet::testSelection() {
    NDSize dims = {15, 15};

    DataSet ds = DataSet::create(h5group, "dsDoubleSelection", DataType::Double, dims);

    typedef boost::multi_array<double, 2> array_type;
    typedef array_type::index index;
    array_type A(boost::extents[5][5]);
    int values = 1;
    for(index i = 0; i != 5; ++i)
        for(index j = 0; j != 5; ++j)
            A[i][j] = values++;

    Selection memSelection(A);
    Selection fileSelection = ds.createSelection();

    NDSize fileCount(dims.size());
    NDSize fileStart(dims.size());
    fileCount.fill(5ULL);
    fileStart.fill(5ULL);
    fileSelection.select(fileCount, fileStart);

    NDSize boundsStart(dims.size());
    NDSize boundsEnd(dims.size());

    fileSelection.bounds(boundsStart, boundsEnd);
    NDSize boundsSize = fileSelection.size();

    ds.write(A, fileSelection, memSelection);

    array_type B(boost::extents[5][5]);
    ds.read(B, fileSelection); //NB: no mem-selection

    for(index i = 0; i != 5; ++i)
        for(index j = 0; j != 5; ++j)
            CPPUNIT_ASSERT_EQUAL(A[i][j], B[i][j]);

    NDSSize offset(dims.size(), 5);
    fileSelection.offset(offset);
    ds.write(A, fileSelection);

    array_type C(boost::extents[5][5]);
    ds.read(C, fileSelection, true);

    for(index i = 0; i != 5; ++i)
        for(index j = 0; j != 5; ++j)
            CPPUNIT_ASSERT_EQUAL(A[i][j], C[i][j]);
}

/* helper functions vor testValueIO */

template<typename T>
void test_val_generic(H5::Group &h5group, const T &test_value, std::string name)
{
    std::vector<nix::Value> values = {nix::Value(test_value), nix::Value(test_value)};

    nix::NDSize size = {1};
    H5::DataType fileType = nix::hdf5::DataSet::fileTypeForValue(values[0].type());

    nix::hdf5::DataSet ds = nix::hdf5::DataSet::create(h5group, name, fileType, size);

    ds.write(values);
    std::vector<nix::Value> checkValues;

    ds.read(checkValues);

    CPPUNIT_ASSERT_EQUAL(values.size(), checkValues.size());

    for (size_t i = 0; i < values.size(); ++i) {
        CPPUNIT_ASSERT_EQUAL(values[i].get<T>(), checkValues[i].get<T>());
    }

}

void TestDataSet::testValueIO() {

    test_val_generic(h5group, true, "boolValue");
    test_val_generic(h5group, 42.0, "doubleValue");
    test_val_generic(h5group, 42U, "unsighedValue");

    test_val_generic(h5group, std::string("String Value"), "stringValue");

}


void TestDataSet::testNDArrayIO()
{
    nix::NDSize dims({5, 5});
    nix::NDArray A(nix::DataType::Double, dims);

    int values = 0;
    for(size_t i = 0; i != 5; ++i)
        for(size_t j = 0; j != 5; ++j)
            A.set<double>({i,j}, values++);

    DataSet ds = nix::hdf5::DataSet::create(h5group, "NArray5x5", A);
    ds.write(A);

    nix::NDArray Atest(nix::DataType::Double, dims);
    ds.read(Atest);

    for(size_t i = 0; i != 5; ++i)
        for(size_t j = 0; j != 5; ++j)
            CPPUNIT_ASSERT_DOUBLES_EQUAL(A.get<double>({i,j}),
                                         Atest.get<double>({i,j}),
                                         std::numeric_limits<double>::epsilon());

     //**
    dims = {3, 4, 5};
    NDArray B(nix::DataType::Double, dims);
    values = 0;
    for(size_t i = 0; i != dims[0]; ++i)
        for(size_t j = 0; j != dims[1]; ++j)
            for(size_t k = 0; k != dims[2]; ++k)
                B.set<double>({i,j,k}, values++);

    ds = nix::hdf5::DataSet::create(h5group, "NDArray3x4x5", B);
    ds.write(B);

    nix::NDArray Btest(nix::DataType::Double, dims);
    ds.read(Btest);

    for(size_t i = 0; i != dims[0]; ++i)
        for(size_t j = 0; j != dims[1]; ++j)
            for(size_t k = 0; k != dims[2]; ++k)
                CPPUNIT_ASSERT_DOUBLES_EQUAL(B.get<double>({i,j,k}),
                                             Btest.get<double>({i,j,k}),
                                             std::numeric_limits<double>::epsilon());

}

void TestDataSet::testValArrayIO() {
    std::valarray<double> va_double(10);
    std::iota(std::begin(va_double), std::end(va_double), 0);

    DataSet ds = nix::hdf5::DataSet::create(h5group, "ValArrayd10", va_double);
    ds.write(va_double);

    std::valarray<double> va_double1{};
    ds.read(va_double1, true);

    CPPUNIT_ASSERT_EQUAL(va_double.size(), va_double1.size());
    for(size_t i = 0; i << va_double.size(); i++) {
        CPPUNIT_ASSERT_DOUBLES_EQUAL(va_double[i], va_double1[i],
                                     std::numeric_limits<double>::epsilon());
    }
}

void TestDataSet::tearDown() {
    h5group.close();
    h5file.close();
}
