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
    startup_time = time(NULL);
    file = nix::File::open("test_DataArray.h5", nix::FileMode::Overwrite);

    block = file.createBlock("block_one", "dataset");
    array1 = block.createDataArray("array_one",
                                   "testdata",
                                   nix::DataType::Double,
                                   {0, 0, 0});
    array2 = block.createDataArray("random",
                                   "double",
                                   nix::DataType::Double,
                                   {20, 20});
}

void TestDataArray::tearDown()
{
    file.close();
}


void TestDataArray::testValidate() {
    std::cout << std::endl << array1.validate();
}


void TestDataArray::testId() {
    CPPUNIT_ASSERT(array1.id().size() == 27);
}


void TestDataArray::testName() {
    CPPUNIT_ASSERT(array1.name() == "array_one");
    std::string name = nix::util::createId("", 32);
    array1.name(name);
    CPPUNIT_ASSERT(array1.name() == name);
}


void TestDataArray::testType() {
    CPPUNIT_ASSERT(array1.type() == "testdata");
    std::string typ = nix::util::createId("", 32);
    array1.type(typ);
    CPPUNIT_ASSERT(array1.type() == typ);
}


void TestDataArray::testDefinition() {
    std::string def = nix::util::createId("", 128);
    array1.definition(def);
    CPPUNIT_ASSERT(*array1.definition() == def);
    array1.definition(boost::none);
    CPPUNIT_ASSERT(array1.definition() == nix::none);
}


void TestDataArray::testData()
{
    typedef boost::multi_array<double, 3> array_type;
    typedef array_type::index index;
    array_type A(boost::extents[3][4][2]);

    int values = 0;
    for(index i = 0; i != 3; ++i)
        for(index j = 0; j != 4; ++j)
            for(index k = 0; k != 2; ++k)
                A[i][j][k] = values++;

    CPPUNIT_ASSERT_EQUAL(array1.hasData(), true);
    CPPUNIT_ASSERT_EQUAL(array1.dataType(), nix::DataType::Double);
    CPPUNIT_ASSERT_EQUAL(array1.dataExtent(), (nix::NDSize{0, 0, 0}));
    CPPUNIT_ASSERT(array1.getDimension(1) == false);

    array1.setData(A);

    array_type B(boost::extents[1][1][1]);
    array1.getData(B);

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

    CPPUNIT_ASSERT_EQUAL(array2.hasData(), true);
    CPPUNIT_ASSERT_EQUAL(array2.dataExtent(), (nix::NDSize{20, 20}));

    array2.setData(C, {0,0});
    array2.dataExtent({40, 40});
    CPPUNIT_ASSERT_EQUAL(array2.dataExtent(), (nix::NDSize{40, 40}));

    array2D_type D(boost::extents[5][5]);
    for(index i = 0; i != 5; ++i)
        for(index j = 0; j != 5; ++j)
            D[i][j] = 42.0;


    array2.setData(D, {20, 20});

    array2D_type E(boost::extents[1][1]);
    array2.getData(E, {5,5}, {20, 20});

    for(index i = 0; i != 5; ++i)
        for(index j = 0; j != 5; ++j)
            CPPUNIT_ASSERT_DOUBLES_EQUAL(D[i][j], E[i][j],
                std::numeric_limits<double>::epsilon());


    array2D_type F(boost::extents[5][5]);

    array2.getData(F, {20, 20});

    for(index i = 0; i != 5; ++i)
        for(index j = 0; j != 5; ++j)
            CPPUNIT_ASSERT_DOUBLES_EQUAL(D[i][j], F[i][j],
                std::numeric_limits<double>::epsilon());


    nix::DataArray da3 = block.createDataArray("direct-vector",
                                               "double",
                                               nix::DataType::Double,
                                               {5});

    CPPUNIT_ASSERT(da3.dataExtent() == nix::NDSize{5});
    CPPUNIT_ASSERT(da3.getDimension(1) == false);
    CPPUNIT_ASSERT_EQUAL(da3.hasData(), true);

    std::vector<double> dv = {1.0, 2.0, 3.0, 4.0, 5.0};
    da3.setData(nix::DataType::Double, dv.data(), {5}, {0});

    std::vector<double> dvin;
    dvin.resize(5);
    da3.getData(nix::DataType::Double, dvin.data(), {5}, {0});

    for(size_t i = 0; i < dvin.size(); i++) {
        CPPUNIT_ASSERT_DOUBLES_EQUAL(dv[i], dvin[i],
                                     std::numeric_limits<double>::epsilon());
    }
}

void TestDataArray::testPolynomial()
{
    double PI = boost::math::constants::pi<double>();
    boost::array<double, 10> coefficients1;
    std::vector<double> coefficients2;
    for(int i=0; i<10; i++) {
        coefficients1[i] = i;
        coefficients2.push_back(i);
    }

    double res1 = array1.applyPolynomial(coefficients2, 0, PI);
    double res2 = boost::math::tools::evaluate_polynomial(coefficients1, PI);

    //evalutate_polynomial from boost might use a different algorithm
    //therefore we raise the allowed detlta to 10e-10
    CPPUNIT_ASSERT_DOUBLES_EQUAL(res1, res2, 10e-10);

    array2.polynomCoefficients(coefficients2);
    std::vector<double> ret = array2.polynomCoefficients();
    for(size_t i=0; i<ret.size(); i++) {
        CPPUNIT_ASSERT(ret[i] == coefficients2[i]);
    }

    array2.expansionOrigin(3);
    boost::optional<double> retval = array2.expansionOrigin();
    CPPUNIT_ASSERT(*retval == 3);
    array2.expansionOrigin(nix::none);
    CPPUNIT_ASSERT(array2.expansionOrigin() == nix::none);
}

void TestDataArray::testLabel()
{
    std::string testStr = "somestring";

    array1.label(testStr);
    CPPUNIT_ASSERT(*array1.label() == testStr);
    array1.label(boost::none);
    CPPUNIT_ASSERT(array1.label() == nix::none);
}

void TestDataArray::testUnit()
{
    std::string testStr = "somestring";
    std::string validUnit = "mV^2";

    CPPUNIT_ASSERT_THROW(array1.unit(testStr), nix::InvalidUnit);
    CPPUNIT_ASSERT_NO_THROW(array1.unit(validUnit));
    CPPUNIT_ASSERT(array1.unit() == validUnit);
    CPPUNIT_ASSERT_NO_THROW(array1.unit(boost::none));
    CPPUNIT_ASSERT(array1.unit() == nix::none);
}

void TestDataArray::testDimension()
{
    std::vector<nix::Dimension> dims;
    std::vector<double> ticks;
    double samplingInterval = boost::math::constants::pi<double>();

    for(size_t i = 0; i < 5; i++) {
        ticks.push_back(i * boost::math::constants::pi<double>());
    }

    dims.push_back(array2.createSampledDimension(1, samplingInterval));
    dims.push_back(array2.createSetDimension(2));
    dims.push_back(array2.createRangeDimension(3, ticks));
    dims.push_back(array2.appendSampledDimension(samplingInterval));
    dims.push_back(array2.appendSetDimension());
    dims[3] = array2.createRangeDimension(4, ticks);
    
    // have some explicit dimension types
    nix::RangeDimension dim_range = array1.appendRangeDimension(ticks);
    nix::SampledDimension dim_sampled = array1.appendSampledDimension(samplingInterval);
    nix::SetDimension dim_set = array1.appendSetDimension();

    CPPUNIT_ASSERT(array2.getDimension(dims[0].index()).dimensionType() == nix::DimensionType::Sample);
    CPPUNIT_ASSERT(array2.getDimension(dims[1].index()).dimensionType() == nix::DimensionType::Set);
    CPPUNIT_ASSERT(array2.getDimension(dims[2].index()).dimensionType() == nix::DimensionType::Range);
    CPPUNIT_ASSERT(array2.getDimension(dims[3].index()).dimensionType() == nix::DimensionType::Range);
    CPPUNIT_ASSERT(array2.getDimension(dims[4].index()).dimensionType() == nix::DimensionType::Set);

    CPPUNIT_ASSERT(array2.dimensionCount() == 5);

    dims = array2.dimensions([](nix::Dimension dim) { return dim.dimensionType() == nix::DimensionType::Sample; });
    CPPUNIT_ASSERT(dims.size() == 1);
    CPPUNIT_ASSERT(dims[0].dimensionType() == nix::DimensionType::Sample);
    dims = array2.dimensions([](nix::Dimension dim) { return dim.dimensionType() == nix::DimensionType::Set; });
    CPPUNIT_ASSERT(dims.size() == 2);
    CPPUNIT_ASSERT(dims[0].dimensionType() == nix::DimensionType::Set);
    CPPUNIT_ASSERT(dims[1].dimensionType() == nix::DimensionType::Set);
    dims = array2.dimensions([](nix::Dimension dim) { return dim.dimensionType() == nix::DimensionType::Range; });
    CPPUNIT_ASSERT(dims.size() == 2);
    CPPUNIT_ASSERT(dims[0].dimensionType() == nix::DimensionType::Range);
    CPPUNIT_ASSERT(dims[1].dimensionType() == nix::DimensionType::Range);
    dims = array2.dimensions();
    CPPUNIT_ASSERT(dims.size() == 5);
    CPPUNIT_ASSERT(dims[0].dimensionType() == nix::DimensionType::Sample);
    CPPUNIT_ASSERT(dims[1].dimensionType() == nix::DimensionType::Set);
    CPPUNIT_ASSERT(dims[2].dimensionType() == nix::DimensionType::Range);
    CPPUNIT_ASSERT(dims[3].dimensionType() == nix::DimensionType::Range);
    CPPUNIT_ASSERT(dims[4].dimensionType() == nix::DimensionType::Set);

    // since deleteDimension renumbers indices to be continuous we test that too
    array2.deleteDimension(5);
    array2.deleteDimension(4);
    array2.deleteDimension(1);
    array2.deleteDimension(1);
    array2.deleteDimension(1);
    array1.deleteDimension(1);
    array1.deleteDimension(1);
    array1.deleteDimension(1);

    dims = array2.dimensions();
    CPPUNIT_ASSERT(array2.dimensionCount() == 0);
    CPPUNIT_ASSERT(dims.size() == 0);
}


void TestDataArray::testOperator()
{
    std::stringstream mystream;
    mystream << array1;
}
