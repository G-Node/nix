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
#include "pandora/Representation.hpp"
#include "pandora/DataArray.hpp"

using namespace std;
using namespace pandora;

class TestRepresentation:public CPPUNIT_NS::TestFixture {
private:

  CPPUNIT_TEST_SUITE(TestRepresentation);

  CPPUNIT_TEST(testCreate);

  CPPUNIT_TEST_SUITE_END ();

  File *f1;

public:

  void setUp() {
    f1 = new File("test_block.h5", "org.g-node", FileMode::ReadWrite);

  }

  void tearDown() {
    delete f1;
  }


  void testCreate(){

  }

};
