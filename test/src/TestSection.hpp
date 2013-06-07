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
#include "pandora/SectionTreeIterator.hpp"

using namespace std;
using namespace pandora;

class TestSection: public CPPUNIT_NS::TestFixture {
private:

  CPPUNIT_TEST_SUITE(TestSection);
  CPPUNIT_TEST(testAddAndRemove);
  CPPUNIT_TEST(testBreadthFirstTreeIterator);
  CPPUNIT_TEST(testFinding);
  CPPUNIT_TEST(testTypeFilter);
  CPPUNIT_TEST(testRelatedSections);
  CPPUNIT_TEST(testAddingProperties);
  CPPUNIT_TEST(testAccessingProperties);
  CPPUNIT_TEST(testRemovingProperties);
  CPPUNIT_TEST(testSectionLinks);
  CPPUNIT_TEST_SUITE_END ();

  File *f1;

public:

  void setUp() {
    f1 = new File("test_block.h5", "org.g-node", FileMode::ReadWrite);
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
      Section s2(f1->findSection(s1.id()));
      lastSectionId = s2.id();
      stringstream errmsg;
      errmsg << "Error while accessing block: s1.id() = " << s1.id()
                                                  << " / s2.id() = " << s2.id();
      CPPUNIT_ASSERT_MESSAGE(errmsg.str(), s1 == s2);
    }
    Section test = f1->findSection(lastSectionId);
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
    CPPUNIT_ASSERT_MESSAGE(msg1.str(),test.children() == test.children().end());

    stringstream msg2;
    msg2 << "Error while adding child sections to section: " << lastSectionId
        << " should have two children!";
    Section c1 = test.addSection("child1", "dataset");
    Section c2 = test.addSection("child2", "dataset");
    size_t childCount = test.childCount();
    CPPUNIT_ASSERT_MESSAGE(msg2.str(), childCount == 2);
    CPPUNIT_ASSERT_MESSAGE(msg2.str(), test.hasChildren());
  }

  void testBreadthFirstTreeIterator(){
    Section s1 = f1->createSection("Iterator test","test");
    Section child1 = s1.addSection("Test child 1","test");
    child1.addSection("grand child 1","test");
    Section grandchild2 = child1.addSection("grand child 2","test");
    grandchild2.addSection("great grandchild 1", "test");
    Section child2 = s1.addSection("Test child 2","test");

    int count = 0;
    for(SectionTreeIterator iter = s1.treeIterator("",1); iter != iter.end(); ++iter){
      count++;
    }
    stringstream msg;
    msg << "Error while iterating with depth = 1. Count should have been 2 but is " << count << "!";
    CPPUNIT_ASSERT_MESSAGE(msg.str(), count == 2);

    count = 0;
    for(SectionTreeIterator iter = s1.treeIterator("",2); iter != iter.end(); ++iter){
      count++;
    }
    stringstream msg2;
    msg2 << "Error while iterating with depth = 2. Count should have been 4 but is " << count << "!";
    CPPUNIT_ASSERT_MESSAGE(msg2.str(), count == 4);

    count = 0;
    for(SectionTreeIterator iter = s1.treeIterator("",3); iter != iter.end(); ++iter){
      count++;
    }
    stringstream msg3;
    msg3 << "Error while iterating with depth = 3. Count should have been 5 but is " << count << "!";
    CPPUNIT_ASSERT_MESSAGE(msg3.str(), count == 5);

    count = 0;
    for(SectionTreeIterator iter = s1.treeIterator("",0); iter != iter.end(); ++iter){
      count++;
    }
    stringstream msg4;
    msg4 << "Error while iterating with unlimited depth (0). Count should have been 5 but is " << count << "!";
    CPPUNIT_ASSERT_MESSAGE(msg4.str(), count == 5);
    f1->removeSection(s1.id());
  }

  void testFinding(){
    Section s1 = f1->createSection("Iterator test","test");
    Section child1 = s1.addSection("Test child 1","test");
    child1.addSection("grand child 1","test");
    Section grandchild2 = child1.addSection("grand child 2","test");
    grandchild2.addSection("great grandchild 1", "test");
    Section child2 = s1.addSection("Test child 2","test");


    bool hasSection = f1->hasSection(grandchild2.id());
    stringstream msg;
    msg << "Error while checking for section that is on level 3 but was not found! ";
    CPPUNIT_ASSERT_MESSAGE(msg.str(), hasSection);

    hasSection = f1->hasSection(grandchild2.id(),"",2);
    stringstream msg2;
    msg2 << "Error while checking for section that is on level 3 and was found though the search depth was set to 2! ";
    CPPUNIT_ASSERT_MESSAGE(msg2.str(), !hasSection);

    if(f1->hasSection(grandchild2.id(),"",3)){
      Section temp = f1->findSection(grandchild2.id(),"",3);
      stringstream msg2;
      msg2 << "Error while retrieving existing section on level 3 when finding it with the appropriate depth (3)! ";
      CPPUNIT_ASSERT_MESSAGE(msg2.str(), temp.name().compare(grandchild2.name()) == 0);
    }
    f1->removeSection(s1.id());
  }

  void testTypeFilter(){
    //TODO test the TreeIterator with type filter, test empty filter
    Section s1 = f1->createSection("Iterator test","test");
    s1.addSection("Test child a.1","testa");
    s1.addSection("Test child a.2","testa");
    s1.addSection("Test child b.1","testb");
    s1.addSection("Test child b.2","testb");
    s1.addSection("Test child b.3", "testb");
    s1.addSection("Test child c.1","testc");

    int count = 0;
    for(SectionIterator iter = s1.children("testa"); iter != iter.end();++iter){
      count++;
    }
    stringstream msg;
    msg << "Error while cycling through subsection with type filter. Found "<< count << " children, should have been 2!";
    CPPUNIT_ASSERT_MESSAGE(msg.str(), count == 2);

    count = 0;
    for(SectionIterator iter = s1.children("testb"); iter != iter.end();++iter){
      count++;
    }

    stringstream msg2;
    msg2 << "Error while cycling through subsection with type filter. Found "<< count << " children, should have been 3!";
    CPPUNIT_ASSERT_MESSAGE(msg2.str(), count == 3);

    count = 0;
    for(SectionTreeIterator iter = s1.treeIterator("testb",0); iter != iter.end(); ++iter){
      count++;
    }
    stringstream msg4;
    msg4 << "Error while cycling through subsections with TreeIterator and filter. Found "<< count << " children, should have been 3!";
    CPPUNIT_ASSERT_MESSAGE(msg4.str(), count == 3);

    count = 0;
    for(SectionIterator iter = s1.children("testd"); iter != iter.end();++iter){
      count++;
    }
    stringstream msg3;
    msg3 << "Error while cycling through subsection with type filter. Found "<< count << " children, should have been 0!";
    CPPUNIT_ASSERT_MESSAGE(msg3.str(), count == 0);


    f1->removeSection(s1.id());
  }

  void testRelatedSections(){
    Section a1 = f1->createSection("RelationTest","a");
    Section b1 = a1.addSection("b1","b");
    Section b2 = a1.addSection("b2","b");
    Section c1 = a1.addSection("c1","c");
    Section d1 = c1.addSection("d1","d");
    Section c2 = a1.addSection("c2","c");
    Section d2 = c2.addSection("d2","d");
    Section e1 = d2.addSection("e1","e");
    Section d3 = e1.addSection("d3","d");
    Section f1 = e1.addSection("f1","f");
    Section c3 = a1.addSection("c3","c");

    std::vector<std::string> v = e1.getRelatedSections("d");
    stringstream msg;
    msg << "Error retrieving related section of e1. Should be 1 section with name d3";
    CPPUNIT_ASSERT_MESSAGE(msg.str(), v.size() == 1 && v[0].compare(d3.id()) == 0);

    v = e1.getRelatedSections("c");
    stringstream msg2;
    msg2 << "Error retrieving related section of e1. Should be 1 section with name c2";
    CPPUNIT_ASSERT_MESSAGE(msg2.str(), v.size() == 1 && v[0].compare(c2.id()) == 0);

    v = e1.getRelatedSections("b");
    bool b1Present = (v[0].compare(b1.id()) == 0) || (v[1].compare(b1.id()) == 0);
    bool b2Present = (v[0].compare(b2.id()) == 0) || (v[1].compare(b2.id()) == 0);
    stringstream msg3;
    msg3 << "Error retrieving related section of e1. Should be 2 sections with name b1 and b2";
    CPPUNIT_ASSERT_MESSAGE(msg3.str(), v.size() == 2 && b1Present && b2Present);

    v = b2.getRelatedSections("c");
    bool c1Present = (v[0].compare(c1.id()) == 0) || (v[1].compare(c1.id()) == 0) || (v[2].compare(c1.id()) == 0);
    bool c2Present = (v[0].compare(c2.id()) == 0) || (v[1].compare(c2.id()) == 0) || (v[2].compare(c2.id()) == 0);
    bool c3Present = (v[0].compare(c3.id()) == 0) || (v[1].compare(c3.id()) == 0) || (v[2].compare(c3.id()) == 0);

    stringstream msg4;
    msg4 << "Error retrieving related section of b2. Should be 3 sibling sections.";
    CPPUNIT_ASSERT_MESSAGE(msg4.str(), v.size() == 3 && c1Present && c2Present && c3Present);

    v = b2.getRelatedSections("d");
    stringstream msg5;
    msg5 << "Error retrieving related section of b2. Should be 0  sections.";
    CPPUNIT_ASSERT_MESSAGE(msg4.str(), v.size() == 0);

    f1.removeSection(a1.id());
  }



  void testAddingProperties() {
    Section s = f1->findSection(f1->metadataGroup().objectName(0));
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
    Section s = f1->findSection(f1->metadataGroup().objectName(0));
    stringstream msg;
    msg
    << "Error while accessing property that does not exist. Should have thrown a runtime exception!";
    CPPUNIT_ASSERT_THROW_MESSAGE(msg.str(), s.getProperty("unknownProperty"),
        std::runtime_error);

    Property p = s.addProperty("test property");
    Property p2 = s.getPropertyByName("test property");
    stringstream msg2;
    msg2 << "Error while getting property by name!";
    CPPUNIT_ASSERT_MESSAGE(msg2.str(), p == p2);

    stringstream msg3;
    msg3
    << "Error while accessing an unknown property by name. Should have thrown a runtime exception!";
    CPPUNIT_ASSERT_THROW_MESSAGE(msg3.str(),
        s.getPropertyByName("Test Property"), std::runtime_error);

    s.removeProperty(p.id());
  }

  void testRemovingProperties() {
    Section s = f1->findSection(f1->metadataGroup().objectName(0));
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

  void testSectionLinks() {
    Section base = f1->createSection("base stimulus", "stimulus/white_noise");

    Property baseP1 = base.addProperty("cutoff frequency");
    baseP1.unit("Hz");
    baseP1.addValue(300.0);

    Section base2 = f1->createSection("base stimulus", "stimulus/ou_noise");
    Property base2P1 = base2.addProperty("cutoff frequency");
    base2P1.unit("Hz");
    base2P1.addValue(150.0);

    Section derived = base.addSection("modified stimulus",
        "stimulus/white_noise");
    Property derivedP1 = derived.addProperty("intensity");
    derivedP1.addValue(2.5);
    derivedP1.unit("nA");

    //try to establish link that does not work
    stringstream msg1;
    msg1 << "Error while creating an invalid section-link. Should have thrown a runtime exception!";
    CPPUNIT_ASSERT_THROW_MESSAGE(msg1.str(), derived.link(base2.id()), std::runtime_error);

    //try to establish a link that is valid
    stringstream msg2;
    msg2 << "Error while creating a valid section-link!";
    derived.link(base.id());
    CPPUNIT_ASSERT_MESSAGE(msg2.str(), derived.link().compare(base.id()) == 0);

    //try to access a Property in derived section that is contained in base but not in derived.
    stringstream msg3;
    msg3 << "Error while accessing linked Property!";
    Property p = derived.getPropertyByName("cutoff frequency");

    DoubleValue val;
    p.value(0, val);
    CPPUNIT_ASSERT_MESSAGE(msg3.str(), val.value == 300.0);

    //test inherited PropertyIterator
    PropertyIterator iter = derived.inheritedProperties();
    stringstream msg4;
    msg4 << "Error while retrieving PropertyIterator from section that is linked!";
    CPPUNIT_ASSERT_MESSAGE(msg4.str(), iter.begin() != iter.end());

    stringstream msg5;
    msg5 << "Error while retrieving PropertyIterator that is not linked! Should have thrown runtime exception!";
    CPPUNIT_ASSERT_THROW_MESSAGE(msg5.str(), base2.inheritedProperties(), std::runtime_error);


    f1->removeSection(base.id());
    f1->removeSection(base2.id());

  }

};

