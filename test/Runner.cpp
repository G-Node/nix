#include "TestBlock.hpp"
#include "TestCharon.hpp"
////#include "TestFile.hpp"
//#include "TestSection.hpp"
////#include "TestProperty.hpp"
#include "TestDataSet.hpp"
#include "TestGroup.hpp"
//#include "TestNDBuffer.hpp"
//#include "TestSource.hpp"
//#include "TestDataArray.hpp"
//#include "TestNamedEntityWithSources.hpp"

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>

#include <iostream>


int main(int argc, char* argv[])
{
//  //CPPUNIT_TEST_SUITE_REGISTRATION(TestFile);
  CPPUNIT_TEST_SUITE_REGISTRATION(TestBlock);
  CPPUNIT_TEST_SUITE_REGISTRATION(TestCharon);
  CPPUNIT_TEST_SUITE_REGISTRATION(TestDataSet);
  CPPUNIT_TEST_SUITE_REGISTRATION(TestGroup);
//  //CPPUNIT_TEST_SUITE_REGISTRATION(TestSection);
//  //CPPUNIT_TEST_SUITE_REGISTRATION(TestProperty);
//  CPPUNIT_TEST_SUITE_REGISTRATION(TestDataSet);
//  CPPUNIT_TEST_SUITE_REGISTRATION(TestNDBuffer);
//  CPPUNIT_TEST_SUITE_REGISTRATION(TestSource);
//  CPPUNIT_TEST_SUITE_REGISTRATION(TestDataArray);
//  CPPUNIT_TEST_SUITE_REGISTRATION(TestNamedEntityWithSources);

  CPPUNIT_NS::TestResult testresult;
  CPPUNIT_NS::TestResultCollector collectedresults;
  testresult.addListener(&collectedresults);

  CPPUNIT_NS::BriefTestProgressListener progress;
  testresult.addListener(&progress);

  CPPUNIT_NS::TestRunner testrunner;
  CPPUNIT_NS::TestFactoryRegistry &registry = CPPUNIT_NS::TestFactoryRegistry::getRegistry();
  
  CPPUNIT_NS::Test *test = registry.makeTest();;
  
  if (argc > 1) {
    try {
    test = test->findTest(argv[1]);
    } catch (std::invalid_argument &iae) {
      std::cerr << "Test not found: \"" << argv[1] << "\". Aborting." << std::endl;
      return -1;
    }
  }
  
  testrunner.addTest(test);
  testrunner.run(testresult);

  CPPUNIT_NS::CompilerOutputter compileroutputter(&collectedresults, std::cerr);
  compileroutputter.write();

  return !collectedresults.wasSuccessful();
}
