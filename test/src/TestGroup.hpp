#include <iostream>
#include <sstream>
#include <iterator>
#include <stdexcept>
#include <limits>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>

#include <pandora.hpp>

#include <boost/math/constants/constants.hpp>
#include <boost/iterator/zip_iterator.hpp>

class TestGroup: public CPPUNIT_NS::TestFixture {

public:

  void setUp() {
    unsigned int &openMode = open_mode();

    h5file = H5::H5File("test_group.h5", openMode);
    if (openMode == H5F_ACC_TRUNC) {
      h5group = h5file.createGroup("charon");
    } else {
      h5group = h5file.openGroup("charon");
    }
    openMode = H5F_ACC_RDWR;
  }

  void tearDown() {
    h5group.close();
    h5file.close();
  }

  void groupIface() {
    pandora::Group group(h5group);

    bool hasObj = group.hasObject("nonexisting");
    CPPUNIT_ASSERT_EQUAL(hasObj, false);

    CPPUNIT_ASSERT(group.hasGroup("nonexisting") ==
      group.hasObject("nonexisting"));

    pandora::Group g2 = group.openGroup("subgroup", true);
    hasObj = group.hasObject("subgroup");
    CPPUNIT_ASSERT(group.hasGroup("subgroup") == hasObj);
    CPPUNIT_ASSERT_EQUAL(hasObj, true);


    size_t objcount = group.objectCount();
    CPPUNIT_ASSERT_EQUAL(objcount, 1UL);
    std::string objname = group.objectName(0);
    CPPUNIT_ASSERT(objname == "subgroup");

    group.removeGroup("subgroup");
    hasObj = group.hasObject("subgroup");
    CPPUNIT_ASSERT(group.hasGroup("subgroup") == hasObj);
    CPPUNIT_ASSERT_EQUAL(hasObj, false);
  }

private:

  static unsigned int &open_mode();

  H5::H5File h5file;
  H5::Group h5group;

  CPPUNIT_TEST_SUITE(TestGroup);
  CPPUNIT_TEST(groupIface);
  CPPUNIT_TEST_SUITE_END();
};

unsigned int & TestGroup::open_mode()
{
  static unsigned int openMode = H5F_ACC_TRUNC;
  return openMode;
}

