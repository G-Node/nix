#include <iostream>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>

#include "File.hpp"

using namespace std;
using namespace pandora;

class TestFile : public CPPUNIT_NS::TestFixture
{
private:

  CPPUNIT_TEST_SUITE(TestFile);
  CPPUNIT_TEST(testCreateId);
  CPPUNIT_TEST_SUITE_END ();

  File *f1;

public:

  void setUp(void) {
    f1 = new File("test_block.h5", "org.g-node");
  }

  void tearDown(void) {
    delete f1;
  }

  void testCreateId(void) {
    string s = f1->createId();
  }

};


int main (int argc, char* argv[])
{
    CPPUNIT_TEST_SUITE_REGISTRATION(TestFile);

    // Informiert Test-Listener ueber Testresultate
    CPPUNIT_NS::TestResult testresult;

    // Listener zum Sammeln der Testergebnisse registrieren
    CPPUNIT_NS::TestResultCollector collectedresults;
    testresult.addListener (&collectedresults);

    // Listener zur Ausgabe der Ergebnisse einzelner Tests
    CPPUNIT_NS::BriefTestProgressListener progress;
    testresult.addListener (&progress);

    // Test-Suite ueber die Registry im Test-Runner einfuegen
    CPPUNIT_NS::TestRunner testrunner;
    testrunner.addTest (CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest());
    testrunner.run (testresult);

    // Resultate im Compiler-Format ausgeben
    CPPUNIT_NS::CompilerOutputter compileroutputter(&collectedresults, std::cerr);
    compileroutputter.write ();

    // Rueckmeldung, ob Tests erfolgreich waren
    return collectedresults.wasSuccessful () ? 0 : 1;
}
