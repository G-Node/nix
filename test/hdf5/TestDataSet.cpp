// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "TestDataSet.hpp"

#include "hdf5/h5x/H5DataSet.hpp"
#include <nix/NDArray.hpp>

#include <type_traits>

#include <nix/DataType.hpp>

#include <string.h>
#include <sstream>
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
    h5group = nix::hdf5::H5Group(g);

    openMode = H5F_ACC_RDWR;
}


void TestDataSet::testChunkGuessing() {
    CPPUNIT_ASSERT_THROW(hdf5::DataSet::guessChunking(NDSize{}, H5T_NATIVE_DOUBLE),
                         InvalidRank);
    ndsize_t chunk_max = 1024 * 1024;
    ndsize_t chunk_min = 8 * 1024;
    
    NDSize dims({1024, 1024});
    NDSize chunks = hdf5::DataSet::guessChunking(dims, hdf5::h5x::DataType(H5T_NATIVE_DOUBLE));
    CPPUNIT_ASSERT_EQUAL(chunks[0], 64ULL);
    CPPUNIT_ASSERT_EQUAL(chunks[1], 64ULL);

    NDSize max_chunk_dims({512, 512, 512});
    chunks = hdf5::DataSet::guessChunking(max_chunk_dims, hdf5::h5x::DataType(H5T_NATIVE_DOUBLE));
    CPPUNIT_ASSERT((chunks.nelms() * hdf5::h5x::DataType(H5T_NATIVE_DOUBLE).size()) <= chunk_max);
    CPPUNIT_ASSERT((chunks.nelms() * hdf5::h5x::DataType(H5T_NATIVE_DOUBLE).size()) >= chunk_min);
    
    NDSize ones_dims({1, 1, 1});
    chunks = hdf5::DataSet::guessChunking(ones_dims, hdf5::h5x::DataType(H5T_NATIVE_INT));
    CPPUNIT_ASSERT((chunks.nelms() * hdf5::h5x::DataType(H5T_NATIVE_INT).size()) >= chunk_min);  

    NDSize ones_dims_2({1});
    chunks = hdf5::DataSet::guessChunking(ones_dims_2, hdf5::h5x::DataType(H5T_NATIVE_INT));
    CPPUNIT_ASSERT((chunks.nelms() * hdf5::h5x::DataType(H5T_NATIVE_INT).size()) >= chunk_min);
    CPPUNIT_ASSERT_EQUAL(chunks[0], 1024ULL);
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
        {"string", nix::DataType::String},
        {"opaque", nix::DataType::Opaque}
    };

    const NDSize dims({5, 5});

    for (size_t i = 0; i < (sizeof(_types)/sizeof(_type_info)); i++) {
        nix::hdf5::h5x::DataType fileType = nix::hdf5::data_type_to_h5_filetype(_types[i].dtype);
        hdf5::DataSet ds = h5group.createData(_types[i].name, fileType, dims);
        nix::hdf5::h5x::DataType h5dtype = ds.dataType();
        nix::DataType nixdtype = nix::hdf5::data_type_from_h5(h5dtype);
        CPPUNIT_ASSERT_EQUAL(_types[i].dtype, nixdtype);
    }

}


void TestDataSet::testDataTypeFromString() {
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
            {"string", nix::DataType::String},
            {"opaque", nix::DataType::Opaque},
            {"nothing", nix::DataType::Nothing}
    };

    for (size_t i = 0; i < (sizeof(_types)/sizeof(_type_info)); i++) {
        nix::DataType dt = nix::string_to_data_type(_types[i].name);
        CPPUNIT_ASSERT_EQUAL(dt, _types[i].dtype);
        std::string name = nix::data_type_to_string(dt);
        std::transform (name.begin(), name.end(), name.begin(), ::tolower);
        std::stringstream stream;
        stream << dt;
        CPPUNIT_ASSERT_EQUAL(name, _types[i].name);
        CPPUNIT_ASSERT_EQUAL(stream.str(), nix::data_type_to_string(dt));
    }
}


void TestDataSet::testDataTypeIsNumeric() {
    static struct _type_info {
        bool is_numeric;
        nix::DataType dtype;
    } _types[] = {
            {false, nix::DataType::Bool},
            {true, nix::DataType::Int8},
            {true, nix::DataType::UInt8},
            {true, nix::DataType::Int16},
            {true, nix::DataType::UInt16},
            {true, nix::DataType::Int32},
            {true, nix::DataType::UInt32},
            {true, nix::DataType::Int64},
            {true, nix::DataType::UInt64},
            {true, nix::DataType::Float},
            {true, nix::DataType::Double},
            {false, nix::DataType::String},
            {false, nix::DataType::Nothing},
            {false, nix::DataType::Opaque}
    };

    for (size_t i = 0; i < (sizeof(_types)/sizeof(_type_info)); i++) {
        bool is_numeric = nix::data_type_is_numeric(_types[i].dtype);
        CPPUNIT_ASSERT_EQUAL(is_numeric, _types[i].is_numeric);
    }
}



void TestDataSet::testBasic() {
    NDSize dims({4, 6});

    hdf5::DataSet dsZero = h5group.createData("dsZero", H5T_NATIVE_DOUBLE, nix::NDSize({ 0, 0 }));
    CPPUNIT_ASSERT_EQUAL(dsZero.size(), (NDSize{0, 0}));

    hdf5::DataSet ds = h5group.createData("dsDouble", H5T_NATIVE_DOUBLE, dims);

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


void TestDataSet::testNDArrayIO()
{
    nix::NDSize dims({5, 5});
    nix::NDArray A(nix::DataType::Double, dims);

    int values = 0;
    for(size_t i = 0; i != 5; ++i)
        for(size_t j = 0; j != 5; ++j)
            A.set<double>(nix::NDSize({ i, j }), values++);

    hdf5::DataSet ds = h5group.createData("NArray5x5", H5T_NATIVE_DOUBLE, dims);
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

    ds = h5group.createData("NDArray3x4x5", H5T_NATIVE_DOUBLE, dims);
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

    hdf5::DataSet ds = h5group.createData("ValArrayd10", H5T_NATIVE_DOUBLE, NDSize{10});
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

    const hdf5::h5x::DataType opaque(H5T_NATIVE_OPAQUE);

    hdf5::DataSet ds = h5group.createData("OpaqueB10", opaque, size);
    ds.write(bytes, opaque, size);

    char bytes_read[10];
    ds.read(bytes_read, opaque, size);

    CPPUNIT_ASSERT(memcmp(bytes, bytes_read, sizeof(bytes)) == 0);
}

void TestDataSet::tearDown() {
    h5group.close();
    H5Fclose(h5file);
}
