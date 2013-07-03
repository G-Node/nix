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

class TestDataArray : public CPPUNIT_NS::TestFixture {

public:

  void setUp() {
  }

  void tearDown() {

  }


  void basic() {

  }

private:

  CPPUNIT_TEST_SUITE(TestDataArray);
  CPPUNIT_TEST(basic);
  CPPUNIT_TEST_SUITE_END ();
};

