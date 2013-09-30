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

using namespace pandora;

class TestDataSet:public CPPUNIT_NS::TestFixture {

public:

  void setUp();
  void testPSize();
  void testChunkGuessing();
  void testBasic();
  void testSelection();
  void tearDown();

  private:

  static unsigned int &open_mode();

  H5::H5File h5file;
  H5::Group h5group;

  CPPUNIT_TEST_SUITE(TestDataSet);
  CPPUNIT_TEST(testPSize);
  CPPUNIT_TEST(testChunkGuessing);
  CPPUNIT_TEST(testBasic);
  CPPUNIT_TEST(testSelection);
  CPPUNIT_TEST_SUITE_END ();
};


