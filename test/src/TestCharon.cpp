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

#include <pandora.hpp>

class TestCharon:public CPPUNIT_NS::TestFixture {

public:
  void setUp() {
    h5file = H5::H5File("test_charon.h5", H5F_ACC_TRUNC);
    h5group = h5file.createGroup("charon");
  }

  void tearDown() {
    h5group.close();
    h5file.close();
  }

  void testBaseTypes() {
    pandora::Group group(h5group);

    const std::string testStr = "I saw the best minds of my generation destroyed by madness";
    group.setAttr("t_string", testStr);

    std::string retString;
    group.getAttr("t_string", retString);
    CPPUNIT_ASSERT_EQUAL(testStr, retString);

    typedef boost::multi_array<double, 3> array_type;
    typedef array_type::index index;
    array_type A(boost::extents[3][4][2]);

    int values = 0;
    for(index i = 0; i != 3; ++i)
      for(index j = 0; j != 4; ++j)
        for(index k = 0; k != 2; ++k)
          A[i][j][k] = values++;

    group.setAttr<array_type>("t_doubleArray", A);
  }


private:

  H5::H5File h5file;
  H5::Group h5group;

  CPPUNIT_TEST_SUITE(TestCharon);
  CPPUNIT_TEST(testBaseTypes);
  CPPUNIT_TEST_SUITE_END ();
};




 int main(int argc, char* argv[])
 {
  CPPUNIT_TEST_SUITE_REGISTRATION(TestCharon);

  CPPUNIT_NS::TestResult testresult;
  CPPUNIT_NS::TestResultCollector collectedresults;
  testresult.addListener(&collectedresults);

  CPPUNIT_NS::BriefTestProgressListener progress;
  testresult.addListener(&progress);

  CPPUNIT_NS::TestRunner testrunner;
  testrunner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest());
  testrunner.run(testresult);

  CPPUNIT_NS::CompilerOutputter compileroutputter(&collectedresults, std::cerr);
  compileroutputter.write();

  return !collectedresults.wasSuccessful();
}