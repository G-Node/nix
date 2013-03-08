#include <iostream>

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

using namespace std;
using namespace pandora;

class TestFile:public CPPUNIT_NS::TestFixture {
private:

  CPPUNIT_TEST_SUITE(TestFile);
  CPPUNIT_TEST(testCreateId);
  CPPUNIT_TEST(testTimeStamp);
  CPPUNIT_TEST(testGetAttribs);
  CPPUNIT_TEST(testCreateBlock);
  CPPUNIT_TEST(testBlockAccess);
  CPPUNIT_TEST_SUITE_END ();

  File *f1;

public:

  void setUp( void ) {
    f1 = new File("test_block.h5", "org.g-node", File::READ_WRITE);
  }

  void tearDown( void ) {
    f1->close();
    delete f1;
  }

  void testCreateId( void ) {
    string s = f1->createId();
    cout << " new ID: " << s << "\n";
    cout << endl;
  }

  void testTimeStamp( void ) {
    string s = f1->time_stamp();
    cout << "new time stamp: " << s << endl;
    cout << endl;
  }

  void testGetAttribs( void ) {
    cout << "\n" << "File format is: " << f1->format() << endl;
    cout << "File format version:  " << f1->version() << endl;
    cout << "File was created at: " << f1->created_at() << endl;
    cout << "File was last updated at: " << f1->updated_at() << endl;
    cout << "\n";
  }

  void testCreateBlock( void ) {
    pandora::Block b = f1->createBlock("testBlock", "blockType");
    cout << "\n" << b.name() << endl;
    cout << b.type() << endl;
  }

  void testBlockAccess(void){
    cout << "Block count: " << f1->blockCount() << endl;
    cout << "Name by index: " << endl;
    int i = f1->blockCount();
    for (int j = 0; j < i ; j++){
      cout << "\t" << f1->blockName(j) << endl;
    }
    cout << "Has Block: " << f1->hasBlock("test") << endl;
  }

};

int main( int argc, char* argv[] ) {
  CPPUNIT_TEST_SUITE_REGISTRATION(TestFile);
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
