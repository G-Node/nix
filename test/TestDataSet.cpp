// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "TestDataSet.hpp"

#include <nix/hdf5/DataSetHDF5.hpp>
#include <nix/NDArray.hpp>

#include <type_traits>

#include <nix/hdf5/Selection.hpp>
#include <nix/DataType.hpp>

#include <string.h>
#include "RefTester.hpp"

using namespace nix; //quick fix for now


unsigned int & TestDataSet::open_mode()
{
    static unsigned int openMode = H5F_ACC_TRUNC;
    return openMode;
}


void TestDataSet::setUp() {
    unsigned int &openMode = open_mode();

    if (openMode == H5F_ACC_TRUNC) {
        h5file = H5Fcreate("test_dataset.h5", openMode, H5P_DEFAULT, H5P_DEFAULT);
    } else {
        h5file = H5Fopen("test_dataset.h5", openMode, H5P_DEFAULT);
    }

    CPPUNIT_ASSERT(H5Iis_valid(h5file));

    hid_t g;
    if (openMode == H5F_ACC_TRUNC) {
        g = H5Gcreate2(h5file, "charon", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    } else {
        g = H5Gopen2(h5file, "charon", H5P_DEFAULT);
    }

    CPPUNIT_ASSERT(H5Iis_valid(g));
    h5group = nix::hdf5::Group(g);

    openMode = H5F_ACC_RDWR;
}

void TestDataSet::testNDSize() {
    NDSize invalidSize = {};
    NDSize a({23, 42, 1982});

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


    NDSize b({19, 1940, 18});

    NDSize c = a + b;

    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(42),   c[0]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(1982), c[1]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(2000), c[2]);

    NDSize d = c - b;

    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(23),   d[0]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(42),   d[1]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(1982), d[2]);

    NDSize f({1, 2, 3, 4});
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

    NDSize::value_type dp = j.dot(h);
    CPPUNIT_ASSERT_EQUAL(static_cast<NDSize::value_type>(999), dp);

    NDSize s({3, 4});
    dp = s.dot(s);
    CPPUNIT_ASSERT_EQUAL(static_cast<NDSize::value_type>(25), dp);

    //comparison tests
    CPPUNIT_ASSERT_THROW(f < s, IncompatibleDimensions);

    NDSize t({4, 5});
    NDSize u({4, 4});

    // actual non-delegation implementations are < and <=
    CPPUNIT_ASSERT(s < t);
    CPPUNIT_ASSERT(u <= t);

    // everything else, i.e. >, >= is delegated
    CPPUNIT_ASSERT(t > s);
    CPPUNIT_ASSERT(t >= u);

    CPPUNIT_ASSERT(!(t <= s));
    CPPUNIT_ASSERT(!(t < u));
}

void TestDataSet::testChunkGuessing() {

    CPPUNIT_ASSERT_THROW(hdf5::DataSet::guessChunking(NDSize{}, DataType::Double),
                         InvalidRank);

    NDSize dims({1024, 1024});

    NDSize chunks = hdf5::DataSet::guessChunking(dims, DataType::Double);
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

    const NDSize dims({5, 5});

    for (size_t i = 0; i < (sizeof(_types)/sizeof(_type_info)); i++) {
        hdf5::DataSet ds = h5group.createData(_types[i].name, _types[i].dtype, dims);
        CPPUNIT_ASSERT_EQUAL(ds.dataType(), _types[i].dtype);
    }

}

void TestDataSet::testBasic() {
    NDSize dims({4, 6});

    hdf5::DataSet dsZero = h5group.createData("dsZero", DataType::Double, nix::NDSize({ 0, 0 }));
    CPPUNIT_ASSERT_EQUAL(dsZero.size(), (NDSize{0, 0}));

    hdf5::DataSet ds = h5group.createData("dsDouble", DataType::Double, dims);

    test_refcounting<hdf5::DataSet>(dsZero.h5id(), ds.h5id());

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

    NDSize newSize({4, 6});
    ds.setExtent(newSize);
    NDSize newDSSize = ds.size();
    CPPUNIT_ASSERT_EQUAL(newSize, newDSSize);

    array_type F(boost::extents[4][6]);
    ds.read(F);
    for(index i = 0; i != 4; ++i)
        for(index j = 0; j != 6; ++j)
            CPPUNIT_ASSERT_EQUAL(E[i][j], F[i][j]);

    CPPUNIT_ASSERT_THROW(ds.setExtent(nix::NDSize({ 4, 6, 8 })), nix::InvalidRank);

}



void TestDataSet::testSelection() {
    NDSize dims({15, 15});

    hdf5::DataSet ds = h5group.createData("dsDoubleSelection", DataType::Double, dims);

    typedef boost::multi_array<double, 2> array_type;
    typedef array_type::index index;
    array_type A(boost::extents[5][5]);
    int values = 1;
    for(index i = 0; i != 5; ++i)
        for(index j = 0; j != 5; ++j)
            A[i][j] = values++;

    hdf5::Selection memSelection(A);
    hdf5::Selection fileSelection = ds.createSelection();

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
void test_val_generic(nix::hdf5::Group &h5group, const T &test_value, std::string name)
{
    namespace h5x = nix::hdf5::h5x;

    std::vector<nix::Value> values = {nix::Value(test_value), nix::Value(test_value)};

    nix::NDSize size = {1};
    h5x::DataType fileType = nix::hdf5::DataSet::fileTypeForValue(values[0].type());

    nix::hdf5::DataSet ds = h5group.createData(name, fileType, size);

    nix::DataType dt = nix::to_data_type<T>::value;
    CPPUNIT_ASSERT_EQUAL(ds.dataType(), dt);

    ds.write(values);
    std::vector<nix::Value> checkValues;

    ds.read(checkValues);

    CPPUNIT_ASSERT_EQUAL(values.size(), checkValues.size());

    for (size_t i = 0; i < values.size(); ++i) {
        CPPUNIT_ASSERT_EQUAL(values[i].get<T>(), checkValues[i].get<T>());
    }

}

void TestDataSet::testValueIO() {

    test_val_generic(h5group, true,  "boolValue");
    test_val_generic(h5group, 42.0,  "doubleValue");
    test_val_generic(h5group, uint32_t(42), "uint32Value");
    test_val_generic(h5group,  int32_t(42),  "int32Value");
    test_val_generic(h5group, uint64_t(42), "uint64Value");
    test_val_generic(h5group,  int64_t(42),  "int64Value");

    test_val_generic(h5group, std::string("String Value"), "stringValue");

}


void TestDataSet::testNDArrayIO()
{
    nix::NDSize dims({5, 5});
    nix::NDArray A(nix::DataType::Double, dims);

    int values = 0;
    for(size_t i = 0; i != 5; ++i)
        for(size_t j = 0; j != 5; ++j)
            A.set<double>(nix::NDSize({ i, j }), values++);

    hdf5::DataSet ds = h5group.createData("NArray5x5", nix::DataType::Double, dims);
    ds.write(A);

    nix::NDArray Atest(nix::DataType::Double, dims);
    ds.read(Atest);

    for(size_t i = 0; i != 5; ++i)
        for(size_t j = 0; j != 5; ++j)
            CPPUNIT_ASSERT_DOUBLES_EQUAL(A.get<double>(nix::NDSize({ i, j })),
                                         Atest.get<double>(nix::NDSize({ i, j })),
                                         std::numeric_limits<double>::epsilon());

     //**
    dims = nix::NDSize({ 3, 4, 5 });
    NDArray B(nix::DataType::Double, dims);
    values = 0;
    for(size_t i = 0; i != dims[0]; ++i)
        for(size_t j = 0; j != dims[1]; ++j)
            for(size_t k = 0; k != dims[2]; ++k)
                B.set<double>(nix::NDSize({ i, j, k }), values++);

    ds = h5group.createData("NDArray3x4x5", nix::DataType::Double, dims);
    ds.write(B);

    nix::NDArray Btest(nix::DataType::Double, dims);
    ds.read(Btest);

    for(size_t i = 0; i != dims[0]; ++i)
        for(size_t j = 0; j != dims[1]; ++j)
            for(size_t k = 0; k != dims[2]; ++k)
                CPPUNIT_ASSERT_DOUBLES_EQUAL(B.get<double>(nix::NDSize({ i, j, k })),
                                             Btest.get<double>(nix::NDSize({ i, j, k })),
                                             std::numeric_limits<double>::epsilon());

}

void TestDataSet::testValArrayIO() {
    std::valarray<double> va_double(10);
    std::iota(std::begin(va_double), std::end(va_double), 0);

    hdf5::DataSet ds = h5group.createData("ValArrayd10", nix::DataType::Double, NDSize{10});
    ds.write(va_double);

    std::valarray<double> va_double1{};
    ds.read(va_double1, true);

    CPPUNIT_ASSERT_EQUAL(va_double.size(), va_double1.size());
    for(size_t i = 0; i << va_double.size(); i++) {
        CPPUNIT_ASSERT_DOUBLES_EQUAL(va_double[i], va_double1[i],
                                     std::numeric_limits<double>::epsilon());
    }
}

void TestDataSet::testOpaqueIO() {
    char bytes[10];
    std::iota(std::begin(bytes), std::end(bytes), 0);
    NDSize size = {sizeof(bytes)};

    hdf5::DataSet ds = h5group.createData("OpaqueB10", DataType::Opaque, size);
    ds.write(DataType::Opaque, size, bytes);

    char bytes_read[10];
    ds.read(DataType::Opaque, size, bytes_read);

    CPPUNIT_ASSERT(memcmp(bytes, bytes_read, sizeof(bytes)) == 0);
}

void TestDataSet::tearDown() {
    h5group.close();
    H5Fclose(h5file);
}
