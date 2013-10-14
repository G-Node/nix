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

class TestEntityWithMetadata: public CPPUNIT_NS::TestFixture {

private:

  CPPUNIT_TEST_SUITE(TestEntityWithMetadata);
  CPPUNIT_TEST(testMetadata);
  CPPUNIT_TEST_SUITE_END ();

  File *f1;
public:

  void setUp();
  void tearDown();
  void testMetadata();

};
