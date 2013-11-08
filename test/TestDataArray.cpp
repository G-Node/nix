// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "TestDataArray.hpp"

using namespace std;

void TestDataArray::setUp() {
    f1 = new File("test_block.h5", FileMode::ReadWrite);
}

void TestDataArray::tearDown() {
    delete f1;
}

void TestDataArray::testCreateRemove(){
    Block b = f1->createBlock("sources test","test");
    DataArray a = b.createDataArray("test", "analog signal");

    CPPUNIT_ASSERT(b.dataArrayCount() == 1);
    CPPUNIT_ASSERT(b.hasDataArray(a.id()));
    CPPUNIT_ASSERT(b.dataArrays()[0].id().compare(a.id()) == 0);
    CPPUNIT_ASSERT(a.expansionOrigin() == 0.0);
    CPPUNIT_ASSERT(a.polynomCoefficients().size() == 2);
    CPPUNIT_ASSERT(a.polynomCoefficients()[0] == 0.0);
    b.removeDataArray(a.id());
    CPPUNIT_ASSERT(b.dataArrayCount() == 0);

    f1->removeBlock(b.id());
}

void TestDataArray::testSoures() {
    Block b = f1->createBlock("sources test","test");
    DataArray a = b.createDataArray("test", "analog signal");
    Source s1 = b.createSource("S1","test");
    a.addSource(s1);

    Source s2 = b.createSource("S2","test");
    a.addSource(s2);

    Source s3 = s1.createSource("S3","test");
    Source s4 = s1.createSource("S4","test");
    Source s5 = s2.createSource("S5","test");

    vector<Source> source = a.sources();

    CPPUNIT_ASSERT_EQUAL(static_cast<vector<Source>::size_type>(2), source.size());

    CPPUNIT_ASSERT(find_if(source.begin(), source.end(), [&](const Source &s){ return s.id() == s1.id(); }) != source.end());

    CPPUNIT_ASSERT(find_if(source.begin(), source.end(), [&](const Source &s){ return s.id() == s2.id(); }) != source.end());

    f1->removeBlock(b.id());
}

void TestDataArray::testCalibrationSettings(){
    Block b = f1->createBlock("testBlock","test");
    DataArray a = b.createDataArray("DataArray","sampledData");
    vector<double> coefficients;
    coefficients.push_back(2.0);
    coefficients.push_back(1.0);
    a.polynomCoefficients(coefficients);
    vector<double> readCs = a.polynomCoefficients();
    CPPUNIT_ASSERT(readCs.size() == 2);
    CPPUNIT_ASSERT(readCs[0] == 2.0 && readCs[1] == 1.0);

    a.expansionOrigin(10.0);
    CPPUNIT_ASSERT(a.expansionOrigin() == 10.0);
    f1->removeBlock(b.id());
}

void TestDataArray::testData(){
    Block b = f1->createBlock("testBlock","test");
    DataArray a = b.createDataArray("DataArray","sampledData");
    vector<double> coefficients;
    coefficients.push_back(0.0);
    coefficients.push_back(2.0);
    a.polynomCoefficients(coefficients);

    typedef boost::multi_array<double, 1> array_type;
    typedef array_type::index index;
    array_type A(boost::extents[5]);

    for(index i = 0; i != 5; ++i){
        A[i] = 100.0*i;
    }
    a.setRawData(A);

    array_type convertedData, rawData;
    a.getData(convertedData);
    a.getRawData(rawData);
    int errors = 0;
    for(size_t i = 0; i < rawData.size();i++){
        if ((rawData[i]*2.0) != convertedData[i])
            errors++;
    }
    CPPUNIT_ASSERT(errors == 0.0);
    f1->removeBlock(b.id());
}

void TestDataArray::testDimensions() {
    Block b = f1->createBlock("testBlock", "test");
    DataArray a = b.createDataArray("DataArray", "sampledData");

    double offset = 100;
    double inter = 0.001;
    string unit  = "mV";
    string label = "test_label";
    vector<string> labels = {"lebel_one", "label_two"};
    vector<double> tics = {1, 2, 3, 4, 5};

    // test for empty dimensions
    auto dims = a.dimensions();
    CPPUNIT_ASSERT(dims.size() == 0);

    // create dimensions
    shared_ptr<Dimension> dim = a.createDimension(1, DimensionType::SET_DIMENSION);
    shared_ptr<SetDimension> set_dim = static_pointer_cast<SetDimension>(dim);
    set_dim->labels(labels);

    dim = a.createDimension(2, DimensionType::RANGE_DIMENSION);
    shared_ptr<RangeDimension> range_dim = static_pointer_cast<RangeDimension>(dim);
    range_dim->label(label);
    range_dim->tics(tics);
    range_dim->unit(unit);

    dim = a.createDimension(3, DimensionType::SAMPLED_DIMENSION);
    shared_ptr<SampledDimension> sample_dim = static_pointer_cast<SampledDimension>(dim);
    sample_dim->unit(unit);
    sample_dim->label(label);
    sample_dim->samplingInterval(inter);
    sample_dim->offset(offset);

    // test for number of dimensions
    dims = a.dimensions();
    CPPUNIT_ASSERT(dims.size() == 3);

    // test reading
    set_dim = static_pointer_cast<SetDimension>(a.getDimension(1));
    CPPUNIT_ASSERT(set_dim->dimensionType() == DimensionType::SET_DIMENSION);
    CPPUNIT_ASSERT(set_dim->id() == 1);
    CPPUNIT_ASSERT(set_dim->labels() == labels);

    range_dim = static_pointer_cast<RangeDimension>(a.getDimension(2));
    CPPUNIT_ASSERT(range_dim->dimensionType() == DimensionType::RANGE_DIMENSION);
    CPPUNIT_ASSERT(range_dim->id() == 2);
    CPPUNIT_ASSERT(range_dim->label() == label);
    CPPUNIT_ASSERT(range_dim->unit() == unit);
    CPPUNIT_ASSERT(range_dim->tics() == tics);

    sample_dim = static_pointer_cast<SampledDimension>(a.getDimension(3));
    CPPUNIT_ASSERT(sample_dim->dimensionType() == DimensionType::SAMPLED_DIMENSION);
    CPPUNIT_ASSERT(sample_dim->id() == 3);
    CPPUNIT_ASSERT(sample_dim->label() == label);
    CPPUNIT_ASSERT(sample_dim->unit() == unit);
    CPPUNIT_ASSERT(sample_dim->samplingInterval() == inter);
    CPPUNIT_ASSERT(sample_dim->offset() == offset);

    // clean up
    f1->removeBlock(b.id());
}
