// Copyright © 2014 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <kellner@bio.lmu.de>

#include <iostream>
#include <sstream>
#include <iterator>
#include <stdexcept>
#include <limits>

#include <boost/math/constants/constants.hpp>
#include <boost/math/tools/rational.hpp>
#include <boost/iterator/zip_iterator.hpp>

#include <nix/util/util.hpp>
#include <nix/valid/validate.hpp>
#include <nix/hydra/multiArray.hpp>

#include "BaseTestDataArray.hpp"

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>

using namespace nix;
using namespace valid;


void BaseTestDataArray::testValidate() {
    // dims are not equal data dims: 1 warning
    valid::Result result = validate(array1);
    CPPUNIT_ASSERT(result.getErrors().size() == 1);
    CPPUNIT_ASSERT(result.getWarnings().size() == 0);
}


void BaseTestDataArray::testId() {
    CPPUNIT_ASSERT(array1.id().size() == 36);
}


void BaseTestDataArray::testName() {
    CPPUNIT_ASSERT(array1.name() == "array_one");
}


void BaseTestDataArray::testType() {
    CPPUNIT_ASSERT(array1.type() == "testdata");
    std::string typ = nix::util::createId();
    array1.type(typ);
    CPPUNIT_ASSERT(array1.type() == typ);
}


void BaseTestDataArray::testDefinition() {
    std::string def = nix::util::createId();
    array1.definition(def);
    CPPUNIT_ASSERT(*array1.definition() == def);
    array1.definition(boost::none);
    CPPUNIT_ASSERT(array1.definition() == nix::none);
}


void BaseTestDataArray::testData() {
    typedef boost::multi_array<double, 3> array_type;
    typedef array_type::index index;
    array_type A(boost::extents[3][4][2]);

    int values = 0;
    for(index i = 0; i != 3; ++i)
        for(index j = 0; j != 4; ++j)
            for(index k = 0; k != 2; ++k)
                A[i][j][k] = values++;

    CPPUNIT_ASSERT_EQUAL(array1.dataType(), nix::DataType::Double);
    CPPUNIT_ASSERT_EQUAL(array1.dataExtent(), nix::NDSize({0, 0, 0}));
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

    DataArray direct = block.createDataArray("da_direct", "double", A);
    array_type Adirect(boost::extents[3][4][2]);
    direct.getData(Adirect);

    errors = 0;
    verify = 0;
    for(index i = 0; i != 3; ++i) {
        for(index j = 0; j != 4; ++j) {
            for(index k = 0; k != 2; ++k) {
                int v = verify++;
                errors += Adirect[i][j][k] != v;
            }
        }
    }

    CPPUNIT_ASSERT_EQUAL(0, errors);

    //test createDataArray overload that takes data but specify an storage datat type
    DataArray directFloat = block.createDataArray("da_direct_int", "int", A, DataType::Int32);
    CPPUNIT_ASSERT_EQUAL(DataType::Int32, directFloat.dataType());
    boost::multi_array<int, 3> AdirectFloat(boost::extents[3][4][2]);
    direct.getData(AdirectFloat);

    errors = 0;
    verify = 0;
    for(index i = 0; i != 3; ++i) {
        for(index j = 0; j != 4; ++j) {
            for(index k = 0; k != 2; ++k) {
                int v = verify++;
                errors += AdirectFloat[i][j][k] != v;
            }
        }
    }

    CPPUNIT_ASSERT_EQUAL(0, errors);

    typedef boost::multi_array<double, 2> array2D_type;
    typedef array_type::index index;
    array2D_type C(boost::extents[5][5]);

    for(index i = 0; i != 5; ++i)
        for(index j = 0; j != 5; ++j)
            C[i][j] = 42.0;

    CPPUNIT_ASSERT_EQUAL(array2.dataExtent(), nix::NDSize({20, 20}));

    array2.setData(C, nix::NDSize({0, 0}));
    array2.dataExtent(nix::NDSize({40, 40}));
    CPPUNIT_ASSERT_EQUAL(array2.dataExtent(), nix::NDSize({40, 40}));

    array2D_type D(boost::extents[5][5]);
    for(index i = 0; i != 5; ++i)
        for(index j = 0; j != 5; ++j)
            D[i][j] = 42.0;


    array2.setData(D, nix::NDSize({ 20, 20 }));

    array2D_type E(boost::extents[1][1]);
    array2.getData(E, nix::NDSize({ 5, 5 }), nix::NDSize({ 20, 20 }));

    for(index i = 0; i != 5; ++i)
        for(index j = 0; j != 5; ++j)
            CPPUNIT_ASSERT_DOUBLES_EQUAL(D[i][j], E[i][j],
                std::numeric_limits<double>::epsilon());


    array2D_type F(boost::extents[5][5]);

    array2.getData(F, nix::NDSize({ 20, 20 }));

    for(index i = 0; i != 5; ++i)
        for(index j = 0; j != 5; ++j)
            CPPUNIT_ASSERT_DOUBLES_EQUAL(D[i][j], F[i][j],
                std::numeric_limits<double>::epsilon());


    nix::DataArray da3 = block.createDataArray("direct-vector",
                                               "double",
                                               nix::DataType::Double,
                                               nix::NDSize({5}));

    CPPUNIT_ASSERT(da3.dataExtent() == nix::NDSize{5});
    CPPUNIT_ASSERT(da3.getDimension(1) == false);

    std::vector<double> dv = {1.0, 2.0, 3.0, 4.0, 5.0};
    da3.setData(nix::DataType::Double, dv.data(), nix::NDSize({ 5 }), nix::NDSize({ 0 }));

    std::vector<double> dvin;
    dvin.resize(5);
    da3.getData(nix::DataType::Double, dvin.data(), nix::NDSize({ 5 }), nix::NDSize({ 0 }));

    for(size_t i = 0; i < dvin.size(); i++) {
        CPPUNIT_ASSERT_DOUBLES_EQUAL(dv[i], dvin[i],
                                     std::numeric_limits<double>::epsilon());
    }

    // test IO of a scalar

    double scalar = -1.0;

    //value + offset + count
    // nix::NDSize({}) indicates a scalar
    scalar = -1.0;
    da3.getData(scalar, nix::NDSize({}), nix::NDSize({0}));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(dv[0], scalar,
                                 std::numeric_limits<double>::epsilon());


    // nix::NDSize({1, 1, ..., 1}) has a number of elemts == 1
    // so that should work too
    scalar = -1.0;
    da3.getData(scalar, nix::NDSize({1}), nix::NDSize({0}));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(dv[0], scalar,
                                 std::numeric_limits<double>::epsilon());

    scalar = -1.0;
    da3.getData(scalar, nix::NDSize({1, 1}), nix::NDSize({0}));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(dv[0], scalar,
                                 std::numeric_limits<double>::epsilon());

    //value + offset
    scalar = -1.0;
    da3.getData(scalar, nix::NDSize({0}));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(dv[0], scalar,
                                 std::numeric_limits<double>::epsilon());

    //TODO: setData(scalar) + getData(scalar)

    DataArray daA = block.createDataArray("append", "double", DataType::Double , NDSize({0, 4, 5}));

    CPPUNIT_ASSERT_THROW(daA.appendData(DataType::Double, nullptr, NDSize({2, 4, 5}), 3), InvalidRank);
    CPPUNIT_ASSERT_THROW(daA.appendData(DataType::Double, nullptr, NDSize({2, 4, 5, 6}), 0), IncompatibleDimensions);
    CPPUNIT_ASSERT_THROW(daA.appendData(DataType::Double, nullptr, NDSize({4, 4, 6}), 0), IncompatibleDimensions);

    std::vector<double> append_data(3*4*5, 1);

    daA.appendData(DataType::Double, append_data.data(), {3, 4, 5}, 0);
    CPPUNIT_ASSERT_EQUAL(NDSize({3, 4, 5}), daA.dataExtent());

    std::fill(append_data.begin(), append_data.end(), 2);

    daA.appendData(DataType::Double, append_data.data(), {3, 4, 5}, 0);
    CPPUNIT_ASSERT_EQUAL(NDSize({6, 4, 5}), daA.dataExtent());

    std::vector<double> append_check(6*4*5, 0);

    daA.getData(DataType::Double, append_check.data(), {6, 4, 5}, {});

    for(size_t i = 0; i < 6*4*5; i++) {
        CPPUNIT_ASSERT_EQUAL(i > 3*4*5-1 ? 2.0 : 1.0, append_check[i]);
    }

}


void BaseTestDataArray::testPolynomial() {
    double PI = boost::math::constants::pi<double>();
    boost::array<double, 10> coefficients1;
    std::vector<double> coefficients2;
    for(int i=0; i<10; i++) {
        coefficients1[i] = i;
        coefficients2.push_back(i);
    }

    double res1 = 0.0;
    util::applyPolynomial(coefficients2, 0, &PI, &res1, 1);

    double res2 = boost::math::tools::evaluate_polynomial(coefficients1, PI);

    //evalutate_polynomial from boost might use a different algorithm
    //therefore we raise the allowed delta to 10e-10
    CPPUNIT_ASSERT_DOUBLES_EQUAL(res1, res2, 10e-10);

    array2.polynomCoefficients(coefficients2);
    std::vector<double> ret = array2.polynomCoefficients();
    for(size_t i=0; i<ret.size(); i++) {
        CPPUNIT_ASSERT(ret[i] == coefficients2[i]);
    }

    array2.polynomCoefficients(nix::none);
    CPPUNIT_ASSERT(array2.polynomCoefficients().size() == 0);

    array2.expansionOrigin(3);
    boost::optional<double> retval = array2.expansionOrigin();
    CPPUNIT_ASSERT(*retval == 3);
    array2.expansionOrigin(nix::none);
    CPPUNIT_ASSERT(array2.expansionOrigin() == nix::none);

    //test IO with a polynomial set
    nix::DataArray dap = block.createDataArray("polyio", "double", nix::DataType::Double,
                                               nix::NDSize({2, 3}));

    std::vector<double> dv = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    dap.setData(nix::DataType::Double, dv.data(), nix::NDSize({2, 3}), nix::NDSize({0, 0}));

    std::vector<double> poly = {1.0, 2.0, 3.0};
    dap.polynomCoefficients(poly);

    std::vector<int32_t> dvin_poly;
    dvin_poly.resize(2*3);
    dap.getData(DataType::Int32, dvin_poly.data(), nix::NDSize({2, 3}), nix::NDSize({0, 0}));

    // reference data generated with MATLAB(c) as folows:
    // p = [3, 2, 1]
    // polyval(p, [1, 2, 3, 4, 5, 6])
    // ans =
    // 6    17    34    57    86   121
    std::vector<int32_t> ref = {6, 17, 34, 57, 86, 121};

    CPPUNIT_ASSERT_EQUAL(ref.size(), dvin_poly.size());
    for (size_t i = 0; i < dvin_poly.size(); i++) {
        CPPUNIT_ASSERT_EQUAL(ref[i], dvin_poly[i]);
    }

    //poly + expansionOrigin
    const double origin = 4.0;
    dap.expansionOrigin(origin);
    dap.getData(DataType::Int32, dvin_poly.data(), nix::NDSize({2, 3}), nix::NDSize({0, 0}));
    // polyval(p, [1,2,3,4,5,6]-4)
    // ans =  22     9     2     1     6    17
    std::vector<int32_t> ref_eop = {22, 9, 2, 1, 6, 17};

    CPPUNIT_ASSERT_EQUAL(ref_eop.size(), dvin_poly.size());
    for (size_t i = 0; i < dvin_poly.size(); i++) {
        CPPUNIT_ASSERT_EQUAL(ref_eop[i], dvin_poly[i]);
    }

    // expansionOrigin alone
    dap.polynomCoefficients(boost::none);
    dap.getData(DataType::Int32, dvin_poly.data(), nix::NDSize({2, 3}), nix::NDSize({0, 0}));
    CPPUNIT_ASSERT_EQUAL(dv.size(), dvin_poly.size());
    for (size_t i = 0; i < dvin_poly.size(); i++) {
        CPPUNIT_ASSERT_EQUAL(static_cast<int32_t >(dv[i]-origin), dvin_poly[i]);
    }
}


void BaseTestDataArray::testLabel() {
    std::string testStr = "somestring";
    array1.label(testStr);
    CPPUNIT_ASSERT(*array1.label() == testStr);
    array1.label(boost::none);
    CPPUNIT_ASSERT(array1.label() == nix::none);
    CPPUNIT_ASSERT_THROW(array1.label(""), EmptyString);
}


void BaseTestDataArray::testPolynomialSetter() {
    boost::array<double, 10> coefficients1;
    std::vector<double> coefficients2;
    for(int i=0; i<10; i++) {
        coefficients1[i] = i;
        coefficients2.push_back(i);
    }

    array1.polynomCoefficients(coefficients2);
    std::vector<double> ret = array1.polynomCoefficients();
    for(size_t i=0; i<ret.size(); i++) {
        CPPUNIT_ASSERT(ret[i] == coefficients2[i]);
    }

    array1.polynomCoefficients(nix::none);
    CPPUNIT_ASSERT(array1.polynomCoefficients().size() == 0);

    array1.expansionOrigin(3);
    boost::optional<double> retval = array1.expansionOrigin();
    CPPUNIT_ASSERT(*retval == 3);
    array1.expansionOrigin(nix::none);
    CPPUNIT_ASSERT(array1.expansionOrigin() == nix::none);
}


void BaseTestDataArray::testUnit() {
    std::string testStr = "somestring";
    std::string validUnit = "mV^2";

    CPPUNIT_ASSERT_NO_THROW(array1.unit(validUnit));
    CPPUNIT_ASSERT(*array1.unit() == validUnit);
    CPPUNIT_ASSERT_NO_THROW(array1.unit(nix::none));
    CPPUNIT_ASSERT(array1.unit() == nix::none);
    CPPUNIT_ASSERT_THROW(array1.unit(""), nix::EmptyString);

    array1.unit(nix::none);
    CPPUNIT_ASSERT_NO_THROW(array1.unit(testStr));
    CPPUNIT_ASSERT_THROW(array1.appendAliasRangeDimension(), nix::InvalidDimension);

    nix::Dimension dim = array3.appendAliasRangeDimension();
    CPPUNIT_ASSERT_NO_THROW(array3.unit(validUnit));
    array3.unit(nix::none);
    CPPUNIT_ASSERT_THROW(array3.unit(testStr), nix::InvalidUnit);
}


void BaseTestDataArray::testDimension() {
    std::vector<nix::Dimension> dims;
    std::vector<double> ticks;
    std::vector<std::string> setLabels = {"a", "b", "c"};
    double samplingInterval = boost::math::constants::pi<double>();

    for(size_t i = 0; i < 5; i++) {
        ticks.push_back(i * boost::math::constants::pi<double>());
    }
    CPPUNIT_ASSERT_THROW(array2.appendRangeDimension(std::vector<double>{}), nix::InvalidDimension);
    dims.push_back(array2.appendSampledDimension(samplingInterval));
    dims.push_back(array2.appendSetDimension());
    dims.push_back(array2.appendRangeDimension(ticks));
    dims.push_back(array2.appendSetDimension());

    // have some explicit dimension types
    nix::RangeDimension dim_range = array1.appendRangeDimension(ticks, "time", "s");
    CPPUNIT_ASSERT(dim_range.label() && *dim_range.label() == "time");
    CPPUNIT_ASSERT(dim_range.unit() && *dim_range.unit() == "s");

    nix::SampledDimension dim_sampled = array1.appendSampledDimension(samplingInterval, "time", "s");
    CPPUNIT_ASSERT(dim_sampled.label() && *dim_sampled.label() == "time");
    CPPUNIT_ASSERT(dim_sampled.unit() && *dim_sampled.unit() == "s");

    nix::SetDimension dim_set = array1.appendSetDimension(setLabels);
    CPPUNIT_ASSERT(dim_set.labels().size() == 3);

    CPPUNIT_ASSERT(array2.getDimension(dims[0].index()).dimensionType() == nix::DimensionType::Sample);
    CPPUNIT_ASSERT(array2.getDimension(dims[1].index()).dimensionType() == nix::DimensionType::Set);
    CPPUNIT_ASSERT(array2.getDimension(dims[2].index()).dimensionType() == nix::DimensionType::Range);
    CPPUNIT_ASSERT(array2.getDimension(dims[3].index()).dimensionType() == nix::DimensionType::Set);
    CPPUNIT_ASSERT(!dim_range.alias());

    CPPUNIT_ASSERT(array2.dimensionCount() == 4);
    dims = array2.dimensions([](nix::Dimension dim) { return dim.dimensionType() == nix::DimensionType::Sample; });
    CPPUNIT_ASSERT(dims.size() == 1);
    CPPUNIT_ASSERT(dims[0].dimensionType() == nix::DimensionType::Sample);
    dims = array2.dimensions([](nix::Dimension dim) { return dim.dimensionType() == nix::DimensionType::Set; });
    CPPUNIT_ASSERT(dims.size() == 2);
    CPPUNIT_ASSERT(dims[0].dimensionType() == nix::DimensionType::Set);
    CPPUNIT_ASSERT(dims[1].dimensionType() == nix::DimensionType::Set);
    dims = array2.dimensions([](nix::Dimension dim) { return dim.dimensionType() == nix::DimensionType::Range; });
    CPPUNIT_ASSERT(dims.size() == 1);
    CPPUNIT_ASSERT(dims[0].dimensionType() == nix::DimensionType::Range);
    dims = array2.dimensions();
    CPPUNIT_ASSERT(dims.size() == 4);
    CPPUNIT_ASSERT(dims[0].dimensionType() == nix::DimensionType::Sample);
    CPPUNIT_ASSERT(dims[1].dimensionType() == nix::DimensionType::Set);
    CPPUNIT_ASSERT(dims[2].dimensionType() == nix::DimensionType::Range);
    CPPUNIT_ASSERT(dims[3].dimensionType() == nix::DimensionType::Set);
    // since deleteDimension renumbers indices to be continuous we test that too
    array2.deleteDimensions();
    dims = array2.dimensions();
    CPPUNIT_ASSERT(array2.dimensionCount() == 0);
    CPPUNIT_ASSERT(dims.size() == 0);
}


void BaseTestDataArray::testAliasRangeDimension() {
    nix::Dimension dim = array3.appendAliasRangeDimension();
    CPPUNIT_ASSERT(array3.dimensionCount() == 1);
    CPPUNIT_ASSERT(dim.dimensionType() == nix::DimensionType::Range);
    CPPUNIT_ASSERT_THROW(array2.appendAliasRangeDimension(), nix::InvalidDimension);
    CPPUNIT_ASSERT_THROW(array2.appendAliasRangeDimension(), nix::InvalidDimension);
    CPPUNIT_ASSERT_THROW(array3.appendAliasRangeDimension(), nix::InvalidDimension);
    CPPUNIT_ASSERT_THROW(array3.appendAliasRangeDimension(), nix::InvalidDimension);
    DataArray bool_array = block.createDataArray("string array", "string_array",
                                                 nix::DataType::Bool,
                                                 nix::NDSize({20}));
    CPPUNIT_ASSERT_THROW(bool_array.appendAliasRangeDimension(), nix::InvalidDimension);
    nix::RangeDimension rd;
    rd = dim;
    CPPUNIT_ASSERT(rd.alias());

    CPPUNIT_ASSERT((rd.label() && array3.label()) && (*rd.label() == *array3.label()));
    rd.label("new_label");
    CPPUNIT_ASSERT((rd.label() && array3.label()) && (*rd.label() == *array3.label()));
    rd.label(none);
    CPPUNIT_ASSERT(!rd.label() && !array3.label());

    CPPUNIT_ASSERT((rd.unit() && array3.unit()) && (*rd.unit() == *array3.unit()));
    rd.unit("ms");
    CPPUNIT_ASSERT((rd.unit() && array3.unit()) && (*rd.unit() == *array3.unit()));
    rd.unit(none);
    CPPUNIT_ASSERT(!rd.unit() && !array3.unit());

    std::vector<double> t = rd.ticks();
    CPPUNIT_ASSERT(t.size() == array3.dataExtent().nelms());
    t.clear();
    t.resize(10);
    for (size_t i = 0; i < 10; i++) {
        t.push_back(0.2 * i);
    }
    rd.ticks(t);
    CPPUNIT_ASSERT(t.size() == t.size());
    CPPUNIT_ASSERT(t.size() == array3.dataExtent().nelms());

    DataArray int_array = block.createDataArray("int array", "int_array",
                                                 nix::DataType::Int64,
                                                 nix::NDSize({20}));
    CPPUNIT_ASSERT_NO_THROW(int_array.appendAliasRangeDimension());
    dim = int_array.getDimension(1);
    rd = dim;
    CPPUNIT_ASSERT(rd.alias());
    rd.ticks(t);
    CPPUNIT_ASSERT(t.size() == t.size());
    CPPUNIT_ASSERT(t.size() == int_array.dataExtent().nelms());

    std::vector<double> ticks_2 = rd.ticks();
    CPPUNIT_ASSERT(t.size() == ticks_2.size());
}


void BaseTestDataArray::testOperator() {
    std::stringstream mystream;
    mystream << array1;

    array1 = none;
    CPPUNIT_ASSERT(array1 == false);
    CPPUNIT_ASSERT(array1 == none);
}
