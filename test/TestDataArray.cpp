// Copyright © 2014 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <kellner@bio.lmu.de>



#include "TestDataArray.hpp"

void TestDataArray::setUp()
{
    file = nix::File::open("test_DataArray.h5", nix::FileMode::Overwrite);
}

void TestDataArray::tearDown()
{
}

void TestDataArray::testData()
{
    nix::Block block = file.createBlock("testData", "testdata");
    nix::DataArray dA = block.createDataArray("narf", "bla");

    typedef boost::multi_array<double, 3> array_type;
    typedef array_type::index index;
    array_type A(boost::extents[3][4][2]);

    int values = 0;
    for(index i = 0; i != 3; ++i)
        for(index j = 0; j != 4; ++j)
            for(index k = 0; k != 2; ++k)
                A[i][j][k] = values++;

    dA.setData(A);

    //test the getDataType() function
    nix::DataType dtype = dA.getDataType();
    CPPUNIT_ASSERT_EQUAL(dtype, nix::DataType::Double);

    array_type B(boost::extents[1][1][1]);
    dA.getData(B);

    int verify = 0;
    int errors = 0;
    for(index i = 0; i != 3; ++i) {
        for(index j = 0; j != 4; ++j) {
            for(index k = 0; k != 2; ++k) {
                int v = verify++;
                errors += B[i][j][k] != v;
            }
        }
    }

    CPPUNIT_ASSERT_EQUAL(errors, 0);


    typedef boost::multi_array<double, 2> array2D_type;
    typedef array_type::index index;
    array2D_type C(boost::extents[5][5]);

    for(index i = 0; i != 5; ++i)
        for(index j = 0; j != 5; ++j)
            C[i][j] = 42.0;

    nix::DataArray dB = block.createDataArray("random", "double");
    dB.createData(C, {20, 20});

    CPPUNIT_ASSERT_EQUAL(dB.getDataExtent(), (nix::NDSize{20, 20}));

    dB.setData(C, {0,0});

    dB.setDataExtent({40, 40});
    CPPUNIT_ASSERT_EQUAL(dB.getDataExtent(), (nix::NDSize{40, 40}));

    array2D_type D(boost::extents[5][5]);
    for(index i = 0; i != 5; ++i)
        for(index j = 0; j != 5; ++j)
            D[i][j] = 42.0;

    dB.setData(D, {20, 20});

    array2D_type E(boost::extents[1][1]);
    dB.getData(E, {5,5}, {20, 20});

    for(index i = 0; i != 5; ++i)
        for(index j = 0; j != 5; ++j)
            CPPUNIT_ASSERT_DOUBLES_EQUAL(D[i][j], E[i][j],
                std::numeric_limits<double>::epsilon());


    array2D_type F(boost::extents[5][5]);
    dB.getData(F, {20, 20});

    for(index i = 0; i != 5; ++i)
        for(index j = 0; j != 5; ++j)
            CPPUNIT_ASSERT_DOUBLES_EQUAL(D[i][j], F[i][j],
                std::numeric_limits<double>::epsilon());
}
