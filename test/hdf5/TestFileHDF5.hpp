//
// Created by stoewer on 02.11.15.
//

#ifndef NIX_TESTFILEHDF5_HPP_H
#define NIX_TESTFILEHDF5_HPP_H

#include "BaseTestFile.hpp"

class TestFileHDF5: public BaseTestFile {

    CPPUNIT_TEST_SUITE(TestFileHDF5);
    CPPUNIT_TEST(testOpen);
    CPPUNIT_TEST(testFlush);
    CPPUNIT_TEST(testValidate);
    CPPUNIT_TEST(testFormat);
    CPPUNIT_TEST(testLocation);
    CPPUNIT_TEST(testVersion);
    CPPUNIT_TEST(testCreatedAt);
    CPPUNIT_TEST(testUpdatedAt);
    CPPUNIT_TEST(testBlockAccess);
    CPPUNIT_TEST(testSectionAccess);
    CPPUNIT_TEST(testOperators);
    CPPUNIT_TEST(testReopen);
    CPPUNIT_TEST_SUITE_END ();

public:

    void testVersion() override;

    void setUp() override {
        startup_time = time(NULL);
        file_open = nix::File::open("test_file.h5", nix::FileMode::Overwrite);
        file_other = nix::File::open("test_file_other.h5", nix::FileMode::Overwrite);
        file_null = nix::none;
    }


    void tearDown() override {
        file_open.close();
        file_other.close();
    }

    nix::File openFile(const std::string &name, nix::FileMode mode) override {
        return nix::File::open(name + ".h5", mode);
    }

    void testLocation() override {
        CPPUNIT_ASSERT(file_open.location() == "test_file.h5");
        CPPUNIT_ASSERT(file_other.location() == "test_file_other.h5");
    }

};

#endif //NIX_TESTFILEHDF5_HPP_H
