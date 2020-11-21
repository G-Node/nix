//
// Created by stoewer on 05.11.15.
//

#ifndef NIX_TESTFEATUREHDF5_HPP
#define NIX_TESTFEATUREHDF5_HPP

#include "BaseTestFeature.hpp"

class TestFeatureHDF5 : public BaseTestFeature {


    CPPUNIT_TEST_SUITE(TestFeatureHDF5);
    CPPUNIT_TEST(testValidate);
    CPPUNIT_TEST(testId);
    CPPUNIT_TEST(testLinkType);
    CPPUNIT_TEST(testTargetType);
    CPPUNIT_TEST(testDataArrayFeature);
    CPPUNIT_TEST(testDataFrameFeature);
    CPPUNIT_TEST(testLinkType2Str);
    CPPUNIT_TEST(testStreamOperator);
    CPPUNIT_TEST(testOperator);
    CPPUNIT_TEST_SUITE_END ();

public:
    void setUp() {
        file = nix::File::open("test_feature.h5", nix::FileMode::Overwrite);
        block = file.createBlock("featureTest","test");

        data_array = block.createDataArray("featureTest", "Test",
                                           nix::DataType::Double, nix::NDSize({ 0 }));

        std::vector<nix::Column> cols = {
            {"bool", "", nix::DataType::Bool},
            {"int32", "V", nix::DataType::Int32},
            {"uint32", "V", nix::DataType::UInt32},
            {"int64", "A", nix::DataType::Int64},
            {"uint64", "A", nix::DataType::UInt64},
            {"string", "", nix::DataType::String},
            {"double", "mV", nix::DataType::Double}};
        data_frame = block.createDataFrame("isd", "isd", cols);

        tag = block.createTag("featureTest", "Test", {0.0, 2.0, 3.4});
    }

    void tearDown() {
        file.deleteBlock(block.id());
        file.close();
    }

};


#endif //NIX_TESTFEATUREHDF5_HPP
