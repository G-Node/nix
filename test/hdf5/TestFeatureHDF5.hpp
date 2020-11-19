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
    CPPUNIT_TEST(testData);
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

        tag = block.createTag("featureTest", "Test", {0.0, 2.0, 3.4});
    }

    void tearDown() {
        file.deleteBlock(block.id());
        file.close();
    }

};


#endif //NIX_TESTFEATUREHDF5_HPP
