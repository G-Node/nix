#include <iostream>
#include <sstream>
#include <iterator>
#include <stdexcept>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>

#include "pandora/File.hpp"
#include "pandora/Block.hpp"
#include "pandora/Section.hpp"

using namespace std;
using namespace pandora;

class TestFile: public CPPUNIT_NS::TestFixture {
private:

CPPUNIT_TEST_SUITE(TestFile);
    CPPUNIT_TEST(testBlockAccess);
    CPPUNIT_TEST(testSectionAccess);
    CPPUNIT_TEST(testOperators);
  CPPUNIT_TEST_SUITE_END ();

  File *f1;

public:

  void setUp();
  void tearDown();
  void testBlockAccess();
  void testSectionAccess();
  void testOperators();

};

