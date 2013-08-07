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

class TestBlock : public CPPUNIT_NS::TestFixture {

private:

  CPPUNIT_TEST_SUITE(TestBlock);
  CPPUNIT_TEST(testAddRemoveDataArray);
  CPPUNIT_TEST_SUITE_END ();

  File *f1;
public:

  void setUp() {
    f1 = new File("test_block.h5", "org.g-node", FileMode::ReadWrite);
  }

  void tearDown() {
    delete f1;
  }


  void testAddRemoveDataArray() {
    Block b1 = f1->createBlock("test", "dataset");

    CPPUNIT_ASSERT(b1.dataArrayCount() == 0);

    DataArray d1 = b1.createDataArray("testData","analogSignal");
    CPPUNIT_ASSERT(b1.dataArrayCount() == 1);

    DataArray d2 = b1.createDataArray("testData2","analogSignal");
    CPPUNIT_ASSERT(b1.dataArrayCount() == 2);

    CPPUNIT_ASSERT(b1.hasDataArray(d2.id()));
    CPPUNIT_ASSERT(!b1.hasDataArray("some_id"));
    bool d1_present = false;
    bool d2_present = false;

    for(DataArrayIterator iter = b1.dataArrays(); iter != iter.end(); ++iter){
      DataArray temp = *iter;
      if(temp.id().compare(d1.id())==0){
        d1_present = true;
        continue;
      }
      if(temp.id().compare(d2.id())==0){
        d2_present = true;
        continue;
      }
    }
    CPPUNIT_ASSERT(d1_present && d2_present);

    b1.removeDataArray(d2.id());
    CPPUNIT_ASSERT(b1.dataArrayCount() == 1);
    b1.removeDataArray(d1.id());
    CPPUNIT_ASSERT(b1.dataArrayCount() == 0);

    f1->deleteBlock(b1.id());
  }




};
