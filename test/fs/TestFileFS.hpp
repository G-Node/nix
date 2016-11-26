//
// Created by jan on 11/29/15.
//

#ifndef NIX_TESTFILEFS_HPP
#define NIX_TESTFILEFS_HPP

#include "BaseTestFile.hpp"
#include <boost/filesystem.hpp>

namespace bfs = boost::filesystem;

class TestFileFS: public BaseTestFile {

    CPPUNIT_TEST_SUITE(TestFileFS);
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
    CPPUNIT_TEST(testCheckHeader);
    CPPUNIT_TEST(testNonNix);

    CPPUNIT_TEST_SUITE_END ();

public:

    void setUp() {
        startup_time = time(NULL);
        file_open = nix::File::open("test_file", nix::FileMode::Overwrite, "file");
        file_other = nix::File::open("test_file_other", nix::FileMode::Overwrite, "file");
        file_null = nix::none;
    }


    void tearDown() {
        file_open.close();
        file_other.close();
    }


    void testLocation() {
        bfs::path p = boost::filesystem::current_path();
        bfs::path p_file("test_file");
        bfs::path p_other("test_file_other");
        CPPUNIT_ASSERT(file_open.location() ==  (p / p_file).string());
        CPPUNIT_ASSERT(file_other.location() == (p / p_other).string());
    }


    void testCheckHeader() {
        nix::file::AttributesFS attr(file_open.location(), file_open.fileMode());
        std::string frmt("xin");
        attr.set("format", frmt);
        CPPUNIT_ASSERT_THROW(nix::File::open("test_file", nix::FileMode::ReadWrite, "file"), std::runtime_error);
        attr.set("format", "nix");

        std::vector<int> version{2, 0, 0};
        attr.set("version", version);
        CPPUNIT_ASSERT_THROW(nix::File::open("test_file", nix::FileMode::ReadWrite, "file"), std::runtime_error);
    }

    void testNonNix() {
        bfs::path p("non-nix");
        bfs::path pa("non-nix_with_wrong_attributes");
        bfs::create_directory(p);
        bfs::create_directory(pa);
        std::ofstream ofs;
        ofs.open("non-nix_with_wrong_attributes/attributes", std::ofstream::out | std::ofstream::app);
        ofs.close();
        CPPUNIT_ASSERT_THROW(nix::File::open(p.string(), nix::FileMode::ReadOnly, "file"), nix::InvalidFile);
        CPPUNIT_ASSERT_THROW(nix::File::open(pa.string(), nix::FileMode::ReadOnly, "file"), nix::InvalidFile);
        bfs::remove_all(p);
        bfs::remove_all(pa);
    }
};

#endif //NIX_TESTFILEFS_HPP
