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

#include <nix/valid/validate.hpp>

using namespace std;
using namespace nix;
using namespace valid;


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


void TestSection::testValidate() {
    valid::Result result = validate(section);
    CPPUNIT_ASSERT(result.getErrors().size() == 0);
    CPPUNIT_ASSERT(result.getWarnings().size() == 0);
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
    CPPUNIT_ASSERT(*section.definition() == def);
    section.definition(nix::none);
    CPPUNIT_ASSERT(section.definition() == nix::none);
}


void TestSection::testParent() {
    CPPUNIT_ASSERT(section.parent() == nullptr);

    Section child = section.createSection("child", "section");
    CPPUNIT_ASSERT(child.parent() != nullptr);
    CPPUNIT_ASSERT(child.parent().id() == section.id());

    CPPUNIT_ASSERT(child.parent().parent() == nullptr);
}


void TestSection::testRepository() {
    CPPUNIT_ASSERT(!section.repository());
    string rep = "http://foo.bar/" + util::createId("", 32);
    section.repository(rep);
    CPPUNIT_ASSERT(section.repository() == rep);
    section.repository(boost::none);
    CPPUNIT_ASSERT(!section.repository());
}


void TestSection::testLink() {
    CPPUNIT_ASSERT(!section.link());

    section.link(section_other);
    CPPUNIT_ASSERT(section.link());
    CPPUNIT_ASSERT(section.link().id() == section_other.id());

    // test none-unsetter
    section.link(none);
    CPPUNIT_ASSERT(!section.link());
    // test deleter removing link too
    section.link(section);
    file.deleteSection(section.id());
    CPPUNIT_ASSERT(!section.link());
    // re-create section
    section = file.createSection("foo_section", "metadata");
}


void TestSection::testMapping() {
    CPPUNIT_ASSERT(!section.mapping());
    string map = "http://foo.bar/" + util::createId("", 32);
    section.mapping(map);
    CPPUNIT_ASSERT(section.mapping() == map);
    section.mapping(boost::none);
    CPPUNIT_ASSERT(!section.mapping());
}


void TestSection::testSectionAccess() {
    vector<string> names = { "section_a", "section_b", "section_c", "section_d", "section_e" };

    CPPUNIT_ASSERT(section.sectionCount() == 0);
    CPPUNIT_ASSERT(section.sections().size() == 0);
    CPPUNIT_ASSERT(section.getSection("invalid_id") == false);

    vector<string> ids;
    for (auto name : names) {
        Section child_section = section.createSection(name, "metadata");
        CPPUNIT_ASSERT(child_section.name() == name);

        ids.push_back(child_section.id());
    }

    CPPUNIT_ASSERT(section.sectionCount() == names.size());
    CPPUNIT_ASSERT(section.sections().size() == names.size());

    for (auto id : ids) {
        Section child_section = section.getSection(id);
        CPPUNIT_ASSERT(section.hasSection(id) == true);
        CPPUNIT_ASSERT(child_section.id() == id);

        section.deleteSection(id);
    }

    CPPUNIT_ASSERT(section.sectionCount() == 0);
    CPPUNIT_ASSERT(section.sections().size() == 0);
    CPPUNIT_ASSERT(section.getSection("invalid_id") == false);
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
    CPPUNIT_ASSERT(section.findSections(util::AcceptAll<Section>(), 2).size() == 10);
    CPPUNIT_ASSERT(section.findSections(util::AcceptAll<Section>(), 1).size() == 4);
    CPPUNIT_ASSERT(section.findSections(util::AcceptAll<Section>(), 0).size() == 1);

    // test filter
    auto filter_typ1 = util::TypeFilter<Section>("typ1");
    auto filter_typ2 = util::TypeFilter<Section>("typ2");

    CPPUNIT_ASSERT(section.findSections(filter_typ1).size() == 3);
    CPPUNIT_ASSERT(section.findSections(filter_typ2).size() == 8);
}

void TestSection::testFindRelated() {
    /* We create the following tree:
     * 
     * section---l1n1---l2n1---l3n1------------
     *            |      |                    |
     *            ------l2n2---l3n2---l4n1---l5n1
     *                   |      |      |
     *                   ------l3n3---l4n2
     * section_other------------|
     */
    Section l1n1 = section.createSection("l1n1", "typ1");

    Section l2n1 = l1n1.createSection("l2n1", "t1");
    Section l2n2 = l1n1.createSection("l2n2", "t2");
    Section l3n1 = l2n1.createSection("l3n1", "t3");
    Section l3n2 = l2n2.createSection("l3n2", "t3");
    Section l3n3 = l2n2.createSection("l3n3", "t4");
    Section l4n1 = l3n2.createSection("l4n1", "typ2");
    Section l4n2 = l3n3.createSection("l4n2", "typ2");
    Section l5n1 = l4n1.createSection("l5n1", "typ2");
    l2n1.link(l2n2.id());
    l3n1.link(l5n1.id());
    l3n2.link(l3n3.id());
    l4n1.link(l4n2.id());
    section_other.link(l3n3.id());

    string t1 = "t1";
    string t3 = "t3";
    string t4 = "t4";
    string typ2 = "typ2";
    string typ1 = "typ1";

    vector<Section> related = l1n1.findRelated(util::TypeFilter<Section>(t1));
    CPPUNIT_ASSERT(related.size() == 1);
    related = l1n1.findRelated(util::TypeFilter<Section>(t3));
    CPPUNIT_ASSERT(related.size() == 2);
    related = l1n1.findRelated(util::TypeFilter<Section>(t4));
    CPPUNIT_ASSERT(related.size() == 1);
    related = l1n1.findRelated(util::TypeFilter<Section>(typ2));
    CPPUNIT_ASSERT(related.size() == 2);
    related = l4n1.findRelated(util::TypeFilter<Section>(typ1));
    CPPUNIT_ASSERT(related.size() == 1);
    related = l4n1.findRelated(util::TypeFilter<Section>(t1));
    CPPUNIT_ASSERT(related.size() == 1);
    related = l3n2.findRelated(util::TypeFilter<Section>(t1));
    CPPUNIT_ASSERT(related.size() == 1);
    related = l3n2.findRelated(util::TypeFilter<Section>(t3));
    CPPUNIT_ASSERT(related.size() == 0);

    /* Chop the tree to:
     * 
     * section---l1n1---l2n1---l3n1
     * section_other
     *                   
     */
    l1n1.deleteSection(l2n2.id());
    CPPUNIT_ASSERT(section.findSections().size() == 4);
    // test that all (horizontal) links are gone too:
    CPPUNIT_ASSERT(!l2n1.link());
    CPPUNIT_ASSERT(!l3n1.link());
    CPPUNIT_ASSERT(!l3n2.link());
    CPPUNIT_ASSERT(!l4n1.link());
    CPPUNIT_ASSERT(!section_other.link());
    CPPUNIT_ASSERT(!l1n1.hasSection(l2n2));
    /* Extend the tree to:
     * 
     * section---l1n1---l2n1---l3n1
     * section_other-----|
     * 
     * and then chop it down to:
     * 
     * section_other
     *                   
     */
    section_other.link(l2n1.id());
    file.deleteSection(section.id());
    CPPUNIT_ASSERT(section_other.findSections().size() == 1);
    CPPUNIT_ASSERT(!section_other.link());

    // re-create section
    section = file.createSection("section", "metadata");
}


void TestSection::testPropertyAccess() {
    vector<string> names = { "property_a", "property_b", "property_c", "property_d", "property_e" };

    CPPUNIT_ASSERT(section.propertyCount() == 0);
    CPPUNIT_ASSERT(section.properties().size() == 0);
    CPPUNIT_ASSERT(section.getProperty("invalid_id") == false);

    Property p = section.createProperty("empty_prop", DataType::Double);
    CPPUNIT_ASSERT(section.propertyCount() == 1);
    Property prop = section.getPropertyByName("empty_prop");
    CPPUNIT_ASSERT(prop.valueCount() == 0);
    CPPUNIT_ASSERT(prop.dataType() == nix::DataType::Double);
    section.deleteProperty(p.id());
    CPPUNIT_ASSERT(section.propertyCount() == 0);

    Value dummy(10);
    prop = section.createProperty("single value", dummy);
    CPPUNIT_ASSERT(section.hasPropertyWithName("single value"));
    CPPUNIT_ASSERT(section.propertyCount() == 1);
    section.deleteProperty(prop.id());
    CPPUNIT_ASSERT(section.propertyCount() == 0);

    vector<string> ids;
    for (auto name : names) {
        prop = section.createProperty(name, dummy);
        CPPUNIT_ASSERT(prop.name() == name);
        CPPUNIT_ASSERT(section.hasPropertyWithName(name));

        Property prop_copy = section.getPropertyByName(name);

        CPPUNIT_ASSERT(prop_copy.id() == prop.id());

        ids.push_back(prop.id());
    }

    CPPUNIT_ASSERT(section.propertyCount() == names.size());
    CPPUNIT_ASSERT(section.properties().size() == names.size());
    section_other.createProperty("some_prop", dummy);
    section_other.link(section);
    CPPUNIT_ASSERT(section_other.propertyCount() == 1);
    CPPUNIT_ASSERT(section_other.inheritedProperties().size() == names.size() + 1);

    for (auto id : ids) {
        Property prop = section.getProperty(id);
        CPPUNIT_ASSERT(section.hasProperty(id));
        CPPUNIT_ASSERT(prop.id() == id);

        section.deleteProperty(id);
    }

    CPPUNIT_ASSERT(section.propertyCount() == 0);
    CPPUNIT_ASSERT(section.properties().size() == 0);
    CPPUNIT_ASSERT(section.getProperty("invalid_id") == false);

    vector<Value> values;
    values.push_back(Value(10));
    values.push_back(Value(100));
    section.createProperty("another test", values);
    CPPUNIT_ASSERT(section.propertyCount() == 1);
    prop = section.getPropertyByName("another test");
    CPPUNIT_ASSERT(prop.valueCount() == 2);
}


void TestSection::testOperators() {
    CPPUNIT_ASSERT(section_null == false);
    CPPUNIT_ASSERT(section_null == none);

    CPPUNIT_ASSERT(section != false);
    CPPUNIT_ASSERT(section != none);

    CPPUNIT_ASSERT(section == section);
    CPPUNIT_ASSERT(section != section_other);

    section_other = section;
    CPPUNIT_ASSERT(section == section_other);

    section_other = none;
    CPPUNIT_ASSERT(section_null == false);
    CPPUNIT_ASSERT(section_null == none);
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
