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

#include "pandora/Section.hpp"
#include "pandora/SectionIterator.hpp"
#include "pandora/Property.hpp"
#include "pandora/PropertyIterator.hpp"

using namespace std;
using namespace pandora;

class TestProperty: public CPPUNIT_NS::TestFixture {
private:

  CPPUNIT_TEST_SUITE(TestProperty);
  CPPUNIT_TEST(testProperties);
  CPPUNIT_TEST_SUITE_END ();

  File *f1;

public:

  void setUp() {
    f1 = new File("test_block.h5", "org.g-node", File::READ_WRITE);
  }

  void tearDown() {
    delete f1;
  }


  void testProperties() {
    Section s = f1->getSection(f1->metadataGroup().objectName(0));
    if(s.propertyCount() > 0){
      Property p = *s.properties();
      std::string str = "This is a  test string.";
      std::string str2 = "a reference";
      std::string str3 = "a a file name";
      p.addStringValue(str,str2,str3);
    }
  }

};

int main(int argc, char* argv[]) {
  CPPUNIT_TEST_SUITE_REGISTRATION(TestProperty);
  // Informiert Test-Listener ueber Testresultate
  CPPUNIT_NS::TestResult testresult;
  // Listener zum Sammeln der Testergebnisse registrieren
  CPPUNIT_NS::TestResultCollector collectedresults;
  testresult.addListener(&collectedresults);
  // Listener zur Ausgabe der Ergebnisse einzelner Tests
  CPPUNIT_NS::BriefTestProgressListener progress;
  testresult.addListener(&progress);
  // Test-Suite ueber die Registry im Test-Runner einfuegen
  CPPUNIT_NS::TestRunner testrunner;
  testrunner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest());
  testrunner.run(testresult);
  // Resultate im Compiler-Format ausgeben
  CPPUNIT_NS::CompilerOutputter compileroutputter(&collectedresults, std::cerr);
  compileroutputter.write();

  // Rueckmeldung, ob Tests erfolgreich waren
  return collectedresults.wasSuccessful() ? 0 : 1;
}
