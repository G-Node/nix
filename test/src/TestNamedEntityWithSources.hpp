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

class TestNamedEntityWithSources : public CPPUNIT_NS::TestFixture {

private:

  CPPUNIT_TEST_SUITE(TestNamedEntityWithSources);
  CPPUNIT_TEST(testAddRemoveSource);
  CPPUNIT_TEST_SUITE_END ();

  File *f1;
  std::string s_id1, s_id2, b_id;
public:

  void setUp() {
    f1 = new File("test_block.h5", FileMode::ReadWrite);
  }

  void tearDown() {
    delete f1;
  }


  void testAddRemoveSource() {

  }




};
