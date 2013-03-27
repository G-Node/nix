#include <iostream>
#include <sstream>
#include <iterator>

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
#include "pandora/SectionIterator.hpp"

using namespace std;
using namespace pandora;

class TestFile:public CPPUNIT_NS::TestFixture {
private:

  CPPUNIT_TEST_SUITE(TestFile);
  CPPUNIT_TEST(testCreateId);
  CPPUNIT_TEST(testVersion);
  CPPUNIT_TEST(testFormat);
  CPPUNIT_TEST(testCreatedAt);
  CPPUNIT_TEST(testUpdatedAt);
  CPPUNIT_TEST(testBlockAccess);
  CPPUNIT_TEST(testSectionAccess);
  //    CPPUNIT_TEST(testTimeStamp);
  //    CPPUNIT_TEST(testGetAttribs);
  //    CPPUNIT_TEST(testCreateBlock);
  CPPUNIT_TEST_SUITE_END ();

  File *f1;

public:

  void setUp() {
    f1 = new File("test_block.h5", "org.g-node", File::READ_WRITE);
  }

  void tearDown() {
    delete f1;
  }

  void testCreateId() {
    string curr = "", prev;
    int count = 0;
    for (int i = 0; i < 100; i++) {
      prev = curr;
      curr = f1->createId();
      if (prev == curr)
        count++;
    }

    stringstream errmsg;
    errmsg << "Identical IDs generated: " << count;
    CPPUNIT_ASSERT_MESSAGE(errmsg.str(), count == 0);
  }

  void testVersion() {
    string version = f1->version();
    stringstream errmsg;
    errmsg << "Version doesn't match: " << version << " != " << File::VERSION;
    CPPUNIT_ASSERT_MESSAGE(errmsg.str(), version == File::VERSION);
  }

  void testFormat() {
    string format = f1->format();
    stringstream errmsg;
    errmsg << "Format doesn't match: " << format << " != " << File::FORMAT;
    CPPUNIT_ASSERT_MESSAGE(errmsg.str(), format == File::FORMAT);
  }

  void testCreatedAt() {
    CPPUNIT_ASSERT_MESSAGE("Creation time missing", f1->createdAt() > 0);
  }

  void testUpdatedAt() {
    CPPUNIT_ASSERT_MESSAGE("Update time missing", f1->updatedAt() > 0);
  }

  void testBlockAccess() {
    const char *names[5] = {"block_a", "block_b", "block_c", "block_d", "block_e"};
    for (int i = 0; i < 5; i++) {
      Block b1(f1->createBlock(names[i], "dataset"));
      Block b2(f1->getBlock(b1.id()));
      stringstream errmsg;
      errmsg << "Error while accessing block: b1.id() = " << b1.id() << " / b2.id() = " << b2.id();
      CPPUNIT_ASSERT_MESSAGE(errmsg.str(), b1 == b2);
    }
  }

  void testSectionAccess() {
    const char *names[5] = {"section_a", "section_b", "section_c", "section_d", "section_e"};
    std::string lastSectionId;
    for (int i = 0; i < 5; i++) {
      Section s1(f1->createSection(names[i], "Recording"));
      Section s2(f1->getSection(s1.id()));
      lastSectionId = s2.id();
      stringstream errmsg;
      errmsg << "Error while accessing block: s1.id() = " << s1.id() << " / s2.id() = " << s2.id();
      CPPUNIT_ASSERT_MESSAGE(errmsg.str(), s1 == s2);
    }
    Section test = f1->getSection(lastSectionId);
    stringstream errmsg;
    errmsg << "Error while accessing Section with id: " << lastSectionId << ", retrieved section id = " << test.id();
    CPPUNIT_ASSERT_MESSAGE(errmsg.str(), test.id() == lastSectionId);

    stringstream msg1;
    msg1 << "Error while getting section iterator from section with no children: " << lastSectionId;
    CPPUNIT_ASSERT_MESSAGE(msg1.str(), test.children() == test.children().end());

    size_t sectionCount = f1->metadataGroup().objectCount(); // we will need that later
    stringstream msg2;
    msg2 << "Error while adding child sections to section: " << lastSectionId << " should have two children!";
    Section c1 = test.addSection("child1","dataset");
    Section c2 = test.addSection("child2","dataset");
    int childCount = 0;
    for(SectionIterator iter = test.children(); iter != iter.end(); ++iter){
      childCount ++;
    }
    CPPUNIT_ASSERT_MESSAGE(msg2.str(), childCount == 2);
    CPPUNIT_ASSERT_MESSAGE(msg2.str(), test.hasChildren());

    stringstream msg5;
    msg5 << "Error while removing a parent section: " << test.id() << " with cascade == false!";
    bool result = f1->removeSection(test.id(),false);

    CPPUNIT_ASSERT_MESSAGE(msg5.str(), !result);
    CPPUNIT_ASSERT_MESSAGE(msg5.str(), f1->getSection(test.id()) == test);

    stringstream msg3;
    msg3 << "Error while removing child section: " << c1.id() << " from section: " << lastSectionId << " with cascade == true!";
    test.removeSection(c1.id(),true);
    CPPUNIT_ASSERT_MESSAGE(msg3.str(), (sectionCount + 1) == f1->metadataGroup().objectCount());

    stringstream msg4;
    msg4 << "Error while removing child section: " << c1.id() << " from section: " << lastSectionId << " with cascade == false!";
    test.removeSection(c2.id(),false);
    CPPUNIT_ASSERT_MESSAGE(msg4.str(), sectionCount == (f1->metadataGroup().objectCount()));

    test.addSection("child3","dataset");
    test.addSection("child4","dataset");
    stringstream msg6;
    msg6 << "Error while removing section recursively: " << test.id();
    f1->removeSection(test.id(),true);
    CPPUNIT_ASSERT_MESSAGE(msg6.str(), (sectionCount-1) == (f1->metadataGroup().objectCount()));

    /*
    for(SectionIterator iter = f1->sections(); iter != iter.end(); ++iter){
      Section s = *iter;
      cout << "rootSection: " << s.id() << endl;
      for(SectionIterator iter2 = s.children(); iter2 != iter2.end(); ++iter2){
        Section s3 = *iter2;
        cout << "\t\t child section: " << s3.id() << endl;
      }
    }
     */

  }

  //  void testTimeStamp( void ) {
  //    string s = f1->time_stamp();
  //    cout << "\n\tnew time stamp: " << s << endl;
  //  }
  //
  //  void testGetAttribs( void ) {
  //    cout << "\n" << "File format is: " << f1->format() << endl;
  //    cout << "File format version:  " << f1->version() << endl;
  //    cout << "File was created at: " << f1->created_at() << endl;
  //    cout << "File was last updated at: " << f1->updated_at() << endl;
  //  }
  //
  //  void testCreateBlock( void ) {
  //    pandora::Block b = f1->createBlock("testBlock", "blockType");
  //    cout << "\n\tBlock name: " << b.name() << endl;
  //    cout << "\tBlock type: " << b.type() << endl;
  //    cout << "\tBlock block_id: " << b.blockId() << endl;
  //  }
  //
  //  void testBlockAccess( void ) {
  //    size_t count = f1->blockCount();
  //    cout << "\n\tBlock count: " << count << endl;
  //    cout << "\tName by index: " << endl;
  //    for (size_t i = 0; i < count; i++) {
  //      cout << "\t\t" << f1->blockName(i) << endl;
  //    }
  //    bool res = f1->hasBlock(f1->blockId(0));
  //    CPPUNIT_ASSERT_EQUAL(true, res);
  //
  //    res = f1->hasBlock("unkownBlock");
  //    CPPUNIT_ASSERT_EQUAL_MESSAGE("test for existence of unknown block: ",false, res);
  //
  //    pandora::Block block = f1->createBlock("additionalBlock1", "Type");
  //
  //    size_t newCount = f1->blockCount();
  //    CPPUNIT_ASSERT_EQUAL(true,((count+1) ==  newCount));
  //
  //    pandora::Block block2 = f1->createBlock("additionalBlock2", "Type");
  //    newCount = f1->blockCount();
  //    CPPUNIT_ASSERT_EQUAL(true, ((count+2) == newCount));
  //
  //    cout << "\tDeleting block by name!" << endl;
  //    f1->deleteBlock(block.blockId());
  //    CPPUNIT_ASSERT_EQUAL(true,(f1->blockCount() == (newCount-1)));
  //
  //    cout << "\tDeleting block by Block!" << endl;
  //    f1->deleteBlock(block2);
  //    CPPUNIT_ASSERT_EQUAL(true,(count == f1->blockCount()));
  //  }

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
