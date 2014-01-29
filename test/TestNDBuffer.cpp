// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "TestNDBuffer.hpp"

#include <nix/NDBuffer.hpp>

void TestNDBuffer::setUp() {
}

void TestNDBuffer::basic() {
    nix::NDSize dims(2, 5);
    nix::NDBuffer A(nix::DataType::Double, dims);

    int values = 0;
    for(size_t i = 0; i != 5; ++i)
        for(size_t j = 0; j != 5; ++j)
            A.set<double>(i*5 + j, values++);

    values = 0;
    for(size_t i = 0; i != 5; ++i)
        for(size_t j = 0; j != 5; ++j)
            CPPUNIT_ASSERT_EQUAL(A.get<double>(i*5 + j), static_cast<double>(values++));

    CPPUNIT_ASSERT_EQUAL(static_cast<double>(5), A.get<double>({1, 0}));
    CPPUNIT_ASSERT_EQUAL(static_cast<double>(18), A.get<double>({3, 3}));
    CPPUNIT_ASSERT_EQUAL(static_cast<double>(24), A.get<double>({4, 4}));

    dims = {3, 4, 5};
    nix::NDBuffer B(nix::DataType::Double, dims);
    values = 0;
    for(size_t i = 0; i != dims[0]; ++i)
        for(size_t j = 0; j != dims[1]; ++j)
            for(size_t k = 0; k != dims[2]; ++k)
                B.set<double>({i,j,k}, values++);

    CPPUNIT_ASSERT_EQUAL(static_cast<double>(23), B.get<double>({1, 0, 3}));
    CPPUNIT_ASSERT_EQUAL(static_cast<double>(42), B.get<double>({2, 0, 2}));
    CPPUNIT_ASSERT_EQUAL(static_cast<double>(26), B.get<double>({1, 1 ,1}));

}

void TestNDBuffer::tearDown() {
}
