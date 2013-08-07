#include "TestBlock.hpp"
#include "TestCharon.hpp"
#include "TestFile.hpp"
#include "TestSection.hpp"
#include "TestProperty.hpp"
#include "TestDataSet.hpp"
#include "TestGroup.hpp"
#include "TestNDBuffer.hpp"
#include "TestSource.hpp"
#include "TestDataArray.hpp"
#include "TestNamedEntityWithSources.hpp"

int main(int argc, char* argv[])
{
  CPPUNIT_TEST_SUITE_REGISTRATION(TestBlock);
  CPPUNIT_TEST_SUITE_REGISTRATION(TestCharon);
  CPPUNIT_TEST_SUITE_REGISTRATION(TestGroup);
  CPPUNIT_TEST_SUITE_REGISTRATION(TestFile);
  CPPUNIT_TEST_SUITE_REGISTRATION(TestSection);
  CPPUNIT_TEST_SUITE_REGISTRATION(TestProperty);
  CPPUNIT_TEST_SUITE_REGISTRATION(TestDataSet);
  CPPUNIT_TEST_SUITE_REGISTRATION(TestNDBuffer);
  CPPUNIT_TEST_SUITE_REGISTRATION(TestSource);
  CPPUNIT_TEST_SUITE_REGISTRATION(TestDataArray);
  CPPUNIT_TEST_SUITE_REGISTRATION(TestNamedEntityWithSources);

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
