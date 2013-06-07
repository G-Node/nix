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
#include "pandora/Source.hpp"
#include "pandora/SourceIterator.hpp"
#include "pandora/SourceTreeIterator.hpp"

using namespace std;
using namespace pandora;

class TestSource:public CPPUNIT_NS::TestFixture {
private:

  CPPUNIT_TEST_SUITE(TestSource);

  CPPUNIT_TEST(testCreateAndRemove);
  CPPUNIT_TEST(testIterators);

  CPPUNIT_TEST_SUITE_END ();

  File *f1;

public:

  void setUp() {
    f1 = new File("test_block.h5", "org.g-node", FileMode::ReadWrite);

  }

  void tearDown() {
    delete f1;
  }


  void testCreateAndRemove() {
    Source s1 = f1->createSource("S1","test");
    Source s2 = f1->createSource("S2","test");

    stringstream msg;
    msg << "Creating s1 or s2 failed!" ;
    CPPUNIT_ASSERT_MESSAGE(msg.str(), f1->hasSource(s1.id()) && f1->hasSource(s2.id()) );

    size_t count = f1->sourceCount();
    stringstream errmsg;
    errmsg << "Source count does not match! Found " << count << " should have been 2";
    CPPUNIT_ASSERT_MESSAGE(errmsg.str(), count == 2);

    f1->removeSource(s1.id());
    stringstream msg2;
    msg2 << "Removing s1 failed!" ;
    CPPUNIT_ASSERT_MESSAGE(msg2.str(), !f1->hasSource(s1.id()));
    f1->removeSource(s2.id());
  }

  void testIterators(){
    Source s1 = f1->createSource("S1","test");
    s1.addSource("S3","test");
    s1.addSource("S4","test");
    Source s2 = f1->createSource("S2","test");
    s2.addSource("S5","test");

    size_t count = s1.childCount();
    CPPUNIT_ASSERT_EQUAL(count,(size_t)2);
    count = s2.childCount();
    CPPUNIT_ASSERT_EQUAL(count,(size_t)1);




    f1->removeSource(s1.id());
    f1->removeSource(s2.id());
  }


};

