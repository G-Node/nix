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
#include "pandora/SectionIterator.hpp"
#include "pandora/Property.hpp"
#include "pandora/PropertyIterator.hpp"

using namespace std;
using namespace pandora;

class TestSection: public CPPUNIT_NS::TestFixture {
private:

  CPPUNIT_TEST_SUITE(TestSection);
  CPPUNIT_TEST(testAddAndRemove);
  CPPUNIT_TEST(testAddingProperties);
  CPPUNIT_TEST(testAccessingProperties);
  CPPUNIT_TEST(testRemovingProperties);
  CPPUNIT_TEST_SUITE_END ();

  File *f1;

public:

  void setUp() {
    f1 = new File("test_block.h5", "org.g-node", File::READ_WRITE);
  }

  void tearDown() {
    delete f1;
  }

  void testAddAndRemove() {
    const char *names[5] = { "section_a", "section_b", "section_c", "section_d",
        "section_e" };
    std::string lastSectionId;
    for (int i = 0; i < 5; i++) {
      Section s1(f1->createSection(names[i], "Recording"));
      Section s2(f1->getSection(s1.id()));
      lastSectionId = s2.id();
      stringstream errmsg;
      errmsg << "Error while accessing block: s1.id() = " << s1.id()
                      << " / s2.id() = " << s2.id();
      CPPUNIT_ASSERT_MESSAGE(errmsg.str(), s1 == s2);
    }
    Section test = f1->getSection(lastSectionId);
    stringstream errmsg;
    errmsg << "Error while accessing Section with id: " << lastSectionId
        << ", retrieved section id = " << test.id();
    CPPUNIT_ASSERT_MESSAGE(errmsg.str(), test.id() == lastSectionId);

    stringstream msg;
    msg << "Error while accessing Section.children.end on section: "
        << test.id() << ", should have thrown an exception!";
    CPPUNIT_ASSERT_THROW_MESSAGE(msg.str(), *test.children().end(),
        std::range_error);

    stringstream msg1;
    msg1
    << "Error while getting section iterator from section with no children: "
    << lastSectionId;
    SectionIterator iter = test.children();
    CPPUNIT_ASSERT_MESSAGE(msg1.str(),
        test.children() == test.children().end());
    size_t sectionCount = f1->metadataGroup().objectCount(); // we will need that later
    stringstream msg2;
    msg2 << "Error while adding child sections to section: " << lastSectionId
        << " should have two children!";
    Section c1 = test.addSection("child1", "dataset");
    Section c2 = test.addSection("child2", "dataset");
    size_t childCount = test.childCount();
    CPPUNIT_ASSERT_MESSAGE(msg2.str(), childCount == 2);
    CPPUNIT_ASSERT_MESSAGE(msg2.str(), test.hasChildren());

    stringstream msg5;
    msg5 << "Error while removing a parent section: " << test.id()
                    << " with cascade == false!";
    bool result = f1->removeSection(test.id(), false);

    CPPUNIT_ASSERT_MESSAGE(msg5.str(), !result);
    CPPUNIT_ASSERT_MESSAGE(msg5.str(), f1->getSection(test.id()) == test);

    stringstream msg3;
    msg3 << "Error while removing child section: " << c1.id()
                    << " from section: " << lastSectionId << " with cascade == true!";
    test.removeSection(c1.id(), true);
    CPPUNIT_ASSERT_MESSAGE(msg3.str(),
        (sectionCount + 1) == f1->metadataGroup().objectCount());

    stringstream msg4;
    msg4 << "Error while removing child section: " << c1.id()
                    << " from section: " << lastSectionId << " with cascade == false!";
    test.removeSection(c2.id(), false);
    CPPUNIT_ASSERT_MESSAGE(msg4.str(),
        sectionCount == (f1->metadataGroup().objectCount()));

    test.addSection("child3", "dataset");
    test.addSection("child4", "dataset");
    stringstream msg6;
    msg6 << "Error while removing section recursively: " << test.id();
    f1->removeSection(test.id(), true);
    CPPUNIT_ASSERT_MESSAGE(msg6.str(),
        (sectionCount-1) == (f1->metadataGroup().objectCount()));

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

  void testAddingProperties() {
    Section s = f1->getSection(f1->metadataGroup().objectName(0));
    const char *units[5] = { "ms", "kg", "S", "V", "mA" };
    size_t oldCount = s.propertyCount();
    for (int j = 0; j < 5; j++) {
      stringstream name;
      name << "property_" << oldCount + j + 1;
      Property p1 = s.addProperty(name.str());
      p1.unit(units[j]);
    }

    stringstream msg;
    msg << "Error while creating properties in section: s.id() = " << s.id()
                    << " Property count should be: " << oldCount + 5 << " but is: "
                    << s.propertyCount();
    CPPUNIT_ASSERT_MESSAGE(msg.str(), s.propertyCount() == (oldCount + 5));

    Property p = *s.properties();
    stringstream msg2;
    msg2
    << "Error while adding already existing property. Should have thrown a runtime exception!";
    CPPUNIT_ASSERT_THROW_MESSAGE(msg2.str(), s.addProperty(p.name()),
        std::runtime_error);

  }

  void testAccessingProperties() {
    Section s = f1->getSection(f1->metadataGroup().objectName(0));
    stringstream msg;
    msg << "Error while accessing property that does not exist. Should have thrown a runtime exception!";
    CPPUNIT_ASSERT_THROW_MESSAGE(msg.str(), s.getProperty("unknownProperty"),
        std::runtime_error);

    Property p = s.addProperty("test property");
    Property p2 = s.getPropertyByName("test property");
    stringstream msg2;
    msg2 << "Error while getting property by name!";
    CPPUNIT_ASSERT_MESSAGE(msg2.str(), p == p2);

    stringstream msg3;
    msg3 << "Error while accessing an unknown property by name. Should have thrown a runtime exception!";
    CPPUNIT_ASSERT_THROW_MESSAGE(msg3.str(), s.getPropertyByName("Test Property"),
        std::runtime_error);

    s.removeProperty(p.id());
  }

  void testRemovingProperties() {
    Section s = f1->getSection(f1->metadataGroup().objectName(0));
    size_t count = s.propertyCount();
    size_t expectation;
    Property p = s.addProperty("delete property");
    expectation = count + 1;

    stringstream msg;
    msg << "Error while adding property. Property count should have been: "
        << expectation << " but is: " << s.propertyCount();
    CPPUNIT_ASSERT_MESSAGE(msg.str(), expectation == s.propertyCount());

    s.removeProperty(p.id());

    stringstream msg2;
    msg2 << "Error while removing property. Property count should have been: "
        << count << " but is: " << s.propertyCount();
    CPPUNIT_ASSERT_MESSAGE(msg2.str(), s.propertyCount() == count);
  }

};

int main(int argc, char* argv[]) {
  CPPUNIT_TEST_SUITE_REGISTRATION(TestSection);
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
