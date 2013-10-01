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

class TestNDBuffer : public CPPUNIT_NS::TestFixture {

public:

  void setUp();
  void basic();
  void testCharon();
  void tearDown();
  
  
private:
  static unsigned int &open_mode();

  H5::H5File h5file;
  H5::Group h5group;

  CPPUNIT_TEST_SUITE(TestNDBuffer);
  CPPUNIT_TEST(basic);
  CPPUNIT_TEST(testCharon);
  CPPUNIT_TEST_SUITE_END ();
};

