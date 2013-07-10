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
  CPPUNIT_TEST(testAddMetadata);
  CPPUNIT_TEST(testFindSources);

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
    Block b1 = f1->createBlock("test block","test");
    Source s1 = b1.createSource("S1","test");
    Source s2 = b1.createSource("S2","test");

    stringstream msg;
    msg << "Creating s1 or s2 failed!" ;
    CPPUNIT_ASSERT_MESSAGE(msg.str(), b1.hasSource(s1.id()) && b1.hasSource(s2.id()) );

    size_t count = b1.sourceCount();
    stringstream errmsg;
    errmsg << "Source count does not match! Found " << count << " should have been 2";
    CPPUNIT_ASSERT_MESSAGE(errmsg.str(), count == 2);

    b1.removeSource(s1.id());
    stringstream msg2;
    msg2 << "Removing s1 failed!" ;
    CPPUNIT_ASSERT_MESSAGE(msg2.str(), !b1.hasSource(s1.id()));
    b1.removeSource(s2.id());
    f1->deleteBlock(b1.id());
  }

  void testIterators(){
    Block b1 = f1->createBlock("test block","test");
    Source s1 = b1.createSource("S1","test");
    s1.addSource("S3","test");
    s1.addSource("S4","test");
    Source s2 = b1.createSource("S2","test");
    s2.addSource("S5","test");

    size_t count = s1.childCount();
    CPPUNIT_ASSERT_EQUAL(count,(size_t)2);
    count = s2.childCount();
    CPPUNIT_ASSERT_EQUAL(count,(size_t)1);

    b1.removeSource(s1.id());
    b1.removeSource(s2.id());
    f1->deleteBlock(b1.id());

  }

  void testFindSources(){
    Block b1 = f1->createBlock("test block","test");
    Source s1 = b1.createSource("S1","test");
    Source s2 = b1.createSource("S2","test");
    Source s3 = s1.addSource("S3","test");
    Source s4 = s1.addSource("S4","test");
    Source s5 = s2.addSource("S5","test");

    CPPUNIT_ASSERT_EQUAL(b1.hasSource("invalid_id"),false);
    CPPUNIT_ASSERT_EQUAL(b1.hasSource(s3.id()),true);
    CPPUNIT_ASSERT_EQUAL(b1.hasSource(s3.id(),"test"),true);
    CPPUNIT_ASSERT_EQUAL(b1.hasSource(s3.id(),"no test"),false);
    CPPUNIT_ASSERT_EQUAL(b1.hasSource(s3.id(),"test",1),false);
    CPPUNIT_ASSERT_EQUAL(b1.hasSource(s3.id(),"test",2),true);

    b1.removeSource(s1.id());
    b1.removeSource(s2.id());
    f1->deleteBlock(b1.id());
  }

  void testAddMetadata(){
    Block b1 = f1->createBlock("test block","test");
    Source s1 = b1.createSource("S1","test");
    Section sec1 = f1->createSection("Test","metadata");
    s1.metadata(sec1.id());
    CPPUNIT_ASSERT_EQUAL(s1.metadata(),sec1.id());

    CPPUNIT_ASSERT_THROW(s1.metadata("invalid_section"),std::runtime_error);

    b1.removeSource(s1.id());
    f1->removeSection(sec1.id());
  }

};

