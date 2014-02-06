// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in Section and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <ctime>

#include <nix/util/util.hpp>
#include "TestSection.hpp"

using namespace std;
using namespace nix;


void TestSection::setUp() {
    startup_time = time(NULL);
    file = File::open("test_section.h5", FileMode::Overwrite);

    section = file.createSection("section", "metadata");
    section_other = file.createSection("other_section", "metadata");
    section_null  = nullptr;
}

void TestSection::tearDown() {
    file.close();
}

void TestSection::testId() {
    CPPUNIT_ASSERT(section.id().size() == 24);
}


void TestSection::testName() {
    CPPUNIT_ASSERT(section.name() == "section");
    string name = util::createId("", 32);
    section.name(name);
    CPPUNIT_ASSERT(section.name() == name);
}


void TestSection::testType() {
    CPPUNIT_ASSERT(section.type() == "metadata");
    string typ = util::createId("", 32);
    section.type(typ);
    CPPUNIT_ASSERT(section.type() == typ);
}


void TestSection::testDefinition() {
    string def = util::createId("", 128);
    section.definition(def);
    CPPUNIT_ASSERT(section.definition() == def);
}


void TestSection::testRepository() {
    CPPUNIT_ASSERT(section.repository() == "");
    string rep = "http://foo.bar/" + util::createId("", 32);
    section.repository(rep);
    CPPUNIT_ASSERT(section.repository() == rep);
}


void TestSection::testLink() {
    CPPUNIT_ASSERT(section.link() == "");
    section.repository(section_other.id());
    CPPUNIT_ASSERT(section.repository() == section_other.id());
}


void TestSection::testMapping() {
    CPPUNIT_ASSERT(section.mapping() == "");
    string map = "http://foo.bar/" + util::createId("", 32);
    section.mapping(map);
    CPPUNIT_ASSERT(section.mapping() == map);
}


void TestSection::testSectionAccess() {
    vector<string> names = { "section_a", "section_b", "section_c", "section_d", "section_e" };

    CPPUNIT_ASSERT(section.sectionCount() == 0);
    CPPUNIT_ASSERT(section.sections().size() == 0);


    vector<string> ids;
    for (auto it = names.begin(); it != names.end(); it++) {
        Section child_section = section.createSection(*it, "metadata");
        CPPUNIT_ASSERT(child_section.name() == *it);

        ids.push_back(child_section.id());
    }


    CPPUNIT_ASSERT(section.sectionCount() == names.size());
    CPPUNIT_ASSERT(section.sections().size() == names.size());


    for (auto it = ids.begin(); it != ids.end(); it++) {
        Section child_section = section.getSection(*it);
        CPPUNIT_ASSERT(section.hasSection(*it) == true);
        CPPUNIT_ASSERT(child_section.id() == *it);

        section.removeSection(*it);
    }

    CPPUNIT_ASSERT(section.sectionCount() == 0);
    CPPUNIT_ASSERT(section.sections().size() == 0);
}


void TestSection::testFindSection() {
    // prepare
    Section l1n1 = section.createSection("l1n1", "typ1");
    Section l1n2 = section.createSection("l1n2", "typ2");
    Section l1n3 = section.createSection("l1n3", "typ3");

    Section l2n1 = l1n1.createSection("l2n1", "typ1");
    Section l2n2 = l1n1.createSection("l2n2", "typ2");
    Section l2n3 = l1n1.createSection("l2n3", "typ2");
    Section l2n4 = l1n3.createSection("l2n4", "typ2");
    Section l2n5 = l1n3.createSection("l2n5", "typ2");
    Section l2n6 = l1n3.createSection("l2n6", "typ3");

    Section l3n1 = l2n1.createSection("l2n3", "typ1");
    Section l3n2 = l2n3.createSection("l2n3", "typ2");
    Section l3n3 = l2n3.createSection("l2n3", "typ2");
    Section l3n4 = l2n5.createSection("l2n3", "typ2");

    // test depth limit
    CPPUNIT_ASSERT(section.findSections().size() == 14);
    CPPUNIT_ASSERT(section.findSections(util::acceptAllFilter<Section>, 2).size() == 10);
    CPPUNIT_ASSERT(section.findSections(util::acceptAllFilter<Section>, 1).size() == 4);
    CPPUNIT_ASSERT(section.findSections(util::acceptAllFilter<Section>, 0).size() == 1);

    // test filter
    auto filter_typ1 = [](const Section &s) { return s.type() == "typ1"; };
    auto filter_typ2 = [](const Section &s) { return s.type() == "typ2"; };

    CPPUNIT_ASSERT(section.findSections(filter_typ1).size() == 3);
    CPPUNIT_ASSERT(section.findSections(filter_typ2).size() == 8);
}


void TestSection::testPropertyAccess() {
    vector<string> names = { "property_a", "property_b", "property_c", "property_d", "property_e" };

    CPPUNIT_ASSERT(section.propertyCount() == 0);
    CPPUNIT_ASSERT(section.properties().size() == 0);


    vector<string> ids;
    for (auto it = names.begin(); it != names.end(); it++) {
        Property prop = section.createProperty(*it);
        CPPUNIT_ASSERT(prop.name() == *it);
        CPPUNIT_ASSERT(section.hasPropertyWithName(*it));

        Property prop_copy = section.getPropertyByName(*it);

        CPPUNIT_ASSERT(prop_copy.id() == prop.id());

        ids.push_back(prop.id());
    }


    CPPUNIT_ASSERT(section.propertyCount() == names.size());
    CPPUNIT_ASSERT(section.properties().size() == names.size());


    for (auto it = ids.begin(); it != ids.end(); it++) {
        Property prop = section.getProperty(*it);
        CPPUNIT_ASSERT(section.hasProperty(*it));
        CPPUNIT_ASSERT(prop.id() == *it);

        section.removeProperty(*it);
    }

    CPPUNIT_ASSERT(section.propertyCount() == 0);
    CPPUNIT_ASSERT(section.properties().size() == 0);
}


void TestSection::testOperators() {
    CPPUNIT_ASSERT(section_null == NULL);
    CPPUNIT_ASSERT(section_null == nullptr);

    CPPUNIT_ASSERT(section != NULL);
    CPPUNIT_ASSERT(section != nullptr);

    CPPUNIT_ASSERT(section == section);
    CPPUNIT_ASSERT(section != section_other);

    section_other = section;
    CPPUNIT_ASSERT(section == section_other);

    section_other = nullptr;
    CPPUNIT_ASSERT(section_null == NULL);
    CPPUNIT_ASSERT(section_null == nullptr);
}


void TestSection::testCreatedAt() {
    CPPUNIT_ASSERT(section.createdAt() >= startup_time);
    time_t past_time = time(NULL) - 10000000;
    section.forceCreatedAt(past_time);
    CPPUNIT_ASSERT(section.createdAt() == past_time);
}


void TestSection::testUpdatedAt() {
    CPPUNIT_ASSERT(section.updatedAt() >= startup_time);
}

/*
void TestSection::testAddAndRemove() {
    const char *names[5] = { "section_a", "section_b", "section_c", "section_d",
                             "section_e" };
    std::vector<std::string> ids;
    size_t count = f1->sectionCount();
    for (int i = 0; i < 5; i++) {
        Section s1(f1->createSection(names[i], "Recording"));
        ids.push_back(s1.id());
    }
    CPPUNIT_ASSERT(f1->sectionCount() == (count + 5));
    //test retrieving
    for (size_t i = 0; i < ids.size(); i++) {
        CPPUNIT_ASSERT(f1->hasSection(ids[i]));
        Section s = f1->getSection(ids[i]);
        CPPUNIT_ASSERT(s.name().compare(names[i])==0);
    }
    //test removing
    for (size_t i = 0; i < ids.size(); i++) {
        f1->removeSection(ids[i]);
    }
    CPPUNIT_ASSERT(f1->sectionCount() == count);

   Section test = f1->findSection(lastSectionId)[0];
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


void TestSection::testDepthRetrieving() {
    Section s1 = f1->createSection("Iterator test", "test");
    Section child1 = s1.addSection("Test child 1", "test");
    child1.addSection("grand child 1", "test");
    Section grandchild2 = child1.addSection("grand child 2", "test");
    grandchild2.addSection("great grandchild 1", "test");
    Section child2 = s1.addSection("Test child 2", "test");

    std::vector<Section> secs = s1.collectIf([](const Section &section) {
        return true;}, true, 2);
    stringstream msg2;
    msg2
            << "Error while iterating with depth = 2. Count should have been 4 but is "
            << secs.size() << "!";
    CPPUNIT_ASSERT_MESSAGE(msg2.str(), secs.size() == 4);

    stringstream msg3;
    secs = s1.collectIf([](const Section &section) {return true;}, true, 3);
    msg3
            << "Error while iterating with depth = 3. Count should have been 5 but is "
            << secs.size() << "!";
    CPPUNIT_ASSERT_MESSAGE(msg3.str(), secs.size() == 5);

    secs = s1.collectIf([](const Section &section) {return true;}, true, -1);
    stringstream msg4;
    msg4
            << "Error while iterating with unlimited depth (0). Count should have been 5 but is "
            << secs.size() << "!";
    CPPUNIT_ASSERT_MESSAGE(msg4.str(), secs.size() == 5);
    f1->removeSection(s1.id());
}

void TestSection::testRelatedSections() {
    Section a1 = f1->createSection("RelationTest", "a");
    Section b1 = a1.addSection("b1", "b");
    Section b2 = a1.addSection("b2", "b");
    Section c1 = a1.addSection("c1", "c");
    Section d1 = c1.addSection("d1", "d");
    Section c2 = a1.addSection("c2", "c");
    Section d2 = c2.addSection("d2", "d");
    Section e1 = d2.addSection("e1", "e");
    Section d3 = e1.addSection("d3", "d");
    Section f1 = e1.addSection("f1", "f");
    Section c3 = a1.addSection("c3", "c");

    std::vector<Section> v = e1.getRelatedSections("d");
    stringstream msg;
    msg
            << "Error retrieving related section of e1. Should be 1 section with name d3";
    CPPUNIT_ASSERT_MESSAGE(msg.str(),
                           v.size() == 1 && v[0].id().compare(d3.id()) == 0);

    v = e1.getRelatedSections("c");
    stringstream msg2;
    msg2
            << "Error retrieving related section of e1. Should be 1 section with name c2";
    CPPUNIT_ASSERT_MESSAGE(msg2.str(),
                           v.size() == 1 && v[0].id().compare(c2.id()) == 0);

    v = e1.getRelatedSections("b");
    bool b1Present = (v[0].id().compare(b1.id()) == 0)
            || (v[1].id().compare(b1.id()) == 0);
    bool b2Present = (v[0].id().compare(b2.id()) == 0)
            || (v[1].id().compare(b2.id()) == 0);
    stringstream msg3;
    msg3
            << "Error retrieving related section of e1. Should be 2 sections with name b1 and b2";
    CPPUNIT_ASSERT_MESSAGE(msg3.str(), v.size() == 2 && b1Present && b2Present);

    v = b2.getRelatedSections("c");
    bool c1Present = (v[0].id().compare(c1.id()) == 0)
            || (v[1].id().compare(c1.id()) == 0)
            || (v[2].id().compare(c1.id()) == 0);
    bool c2Present = (v[0].id().compare(c2.id()) == 0)
            || (v[1].id().compare(c2.id()) == 0)
            || (v[2].id().compare(c2.id()) == 0);
    bool c3Present = (v[0].id().compare(c3.id()) == 0)
            || (v[1].id().compare(c3.id()) == 0)
            || (v[2].id().compare(c3.id()) == 0);

    stringstream msg4;
    msg4
            << "Error retrieving related section of b2. Should be 3 sibling sections.";
    CPPUNIT_ASSERT_MESSAGE(msg4.str(),
                           v.size() == 3 && c1Present && c2Present && c3Present);

    v = b2.getRelatedSections("d");
    stringstream msg5;
    msg5 << "Error retrieving related section of b2. Should be 0  sections.";
    CPPUNIT_ASSERT_MESSAGE(msg4.str(), v.size() == 0);

    f1.removeSection(a1.id());
}

void TestSection::testAddingProperties() {
    Section s = f1->sections()[0];
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
    Property p = s.properties()[0];
    stringstream msg2;
    msg2
            << "Error while adding already existing property. Should have thrown a runtime exception!";
    CPPUNIT_ASSERT_THROW_MESSAGE(msg2.str(), s.addProperty(p.name()),
                                 std::runtime_error);
}

void TestSection::testAccessingProperties() {
    Section s = f1->sections()[0];
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

void TestSection::testRemovingProperties() {
    Section s = f1->sections()[0];
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

void TestSection::testSectionLinks() {
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
    msg1
            << "Error while creating an invalid section-link. Should have thrown a runtime exception!";
    CPPUNIT_ASSERT_THROW_MESSAGE(msg1.str(), derived.link(base2.id()),
                                 std::runtime_error);

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
    std::vector<Property> props = derived.inheritedProperties();
    stringstream msg4;
    msg4 << "Error while retrieving Property from section that is linked!";
    CPPUNIT_ASSERT_MESSAGE(msg4.str(), props.size() != 0);

    stringstream msg5;
    msg5
            << "Error while retrieving Property that is not linked! Should have thrown runtime exception!";
    CPPUNIT_ASSERT_MESSAGE(msg5.str(), base2.inheritedProperties().size() == 0);

    f1->removeSection(base.id());
    f1->removeSection(base2.id());
}
*/
