#ifndef NIX_TESTFEATUREFS_HPP
#define NIX_TESTFEATUREFS_HPP

#include "BaseTestFeature.hpp"

class TestFeatureFS : public BaseTestFeature {


    CPPUNIT_TEST_SUITE(TestFeatureFS);
    CPPUNIT_TEST(testValidate);
    CPPUNIT_TEST(testId);
    CPPUNIT_TEST(testLinkType);
    CPPUNIT_TEST(testData);
    CPPUNIT_TEST(testLinkType2Str);
    CPPUNIT_TEST(testStreamOperator);
    CPPUNIT_TEST(testOperator);
    CPPUNIT_TEST_SUITE_END ();

public:
    void setUp() {
        file = nix::File::open("test_feature", nix::FileMode::Overwrite, "file");
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

#endif //NIX_TESTFEATUREFS_HPP
