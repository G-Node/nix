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
    f1 = new File("test_block.h5", "org.g-node", FileMode::ReadWrite);
  }

  void tearDown() {
    f1->deleteBlock(b_id);
    f1->removeSource(s_id1);
    f1->removeSource(s_id2);
    delete f1;
  }


  void testAddRemoveSource() {
    Block b = f1->createBlock("test","recording");
    b_id = b.id();
    Source s1 = f1->createSource("test source","quelle");
    s_id1 = s1.id();
    Source s2 = f1->createSource("test source2","quelle");
    s_id2 = s2.id();
    b.addSource(s1);


    CPPUNIT_ASSERT(b.sourceCount() == 1);
    b.addSource(s1.id());
    CPPUNIT_ASSERT(b.sourceCount() == 1);
    CPPUNIT_ASSERT(b.hasSource(s1.id()));

    CPPUNIT_ASSERT(!b.hasSource("test"));

    CPPUNIT_ASSERT_THROW(b.addSource("test"),std::runtime_error);

    b.addSource(s2);
    CPPUNIT_ASSERT(b.sourceCount() == 2);

    CPPUNIT_ASSERT(b.hasSource(s2.id()));

  }




};
