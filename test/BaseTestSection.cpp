// Copyright (c) 2013 - 2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in Section and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <ctime>
#include <iostream>
#include <sstream>
#include <iterator>
#include <stdexcept>

#include <nix/util/util.hpp>
#include <nix/valid/validate.hpp>

#include "BaseTestSection.hpp"

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>

using namespace nix;
using namespace valid;


void BaseTestSection::testValidate() {
    valid::Result result = validate(section);
    CPPUNIT_ASSERT(result.getErrors().size() == 0);
    CPPUNIT_ASSERT(result.getWarnings().size() == 0);
}


void BaseTestSection::testId() {
    CPPUNIT_ASSERT(section.id().size() == 36);
}


void BaseTestSection::testName() {
    CPPUNIT_ASSERT(section.name() == "section");
}


void BaseTestSection::testType() {
    CPPUNIT_ASSERT(section.type() == "metadata");
    std::string typ = util::createId();
    section.type(typ);
    CPPUNIT_ASSERT(section.type() == typ);
}


void BaseTestSection::testDefinition() {
    std::string def = util::createId();
    section.definition(def);
    CPPUNIT_ASSERT(*section.definition() == def);
    section.definition(nix::none);
    CPPUNIT_ASSERT(section.definition() == nix::none);
}


void BaseTestSection::testParent() {
    CPPUNIT_ASSERT(section.parent() == nix::none);

    Section child = section.createSection("child", "section");
    CPPUNIT_ASSERT(child.parent() != nix::none);
    CPPUNIT_ASSERT(child.parent().id() == section.id());

    CPPUNIT_ASSERT(child.parent().parent() == nix::none);
}


void BaseTestSection::testRepository() {
    CPPUNIT_ASSERT(!section.repository());
    std::string rep = "http://foo.bar/" + util::createId();
    section.repository(rep);
    CPPUNIT_ASSERT(section.repository() == rep);
    section.repository(boost::none);
    CPPUNIT_ASSERT(!section.repository());
    CPPUNIT_ASSERT_THROW(section.repository(""), EmptyString);
}


void BaseTestSection::testLink() {
    CPPUNIT_ASSERT(!section.link());

    section.link(section_other);
    CPPUNIT_ASSERT(section.link());
    CPPUNIT_ASSERT(section.link().id() == section_other.id());

    // test none-unsetter
    section.link(none);
    CPPUNIT_ASSERT(!section.link());
    section.link(section_other);
    CPPUNIT_ASSERT(section.link());
    Section null;
    section.link(null);
    CPPUNIT_ASSERT(!section.link());

    // test id setter and resetter
    CPPUNIT_ASSERT_THROW(section.link(""), EmptyString);
    CPPUNIT_ASSERT_THROW(section.link("invalid id"), std::runtime_error);

    section.link(section.id());
    CPPUNIT_ASSERT(section.link().id() == section.id());
    section.link(section_other.id());
    CPPUNIT_ASSERT(section.link().id() == section_other.id());
    // test deleter removing link too
    section.link(section);
    file.deleteSection(section.id());
    CPPUNIT_ASSERT(!section.link());
}


void BaseTestSection::testMapping() {
    CPPUNIT_ASSERT(!section.mapping());
    std::string map = "http://foo.bar/" + util::createId();
    section.mapping(map);
    CPPUNIT_ASSERT(section.mapping() == map);
    section.mapping(boost::none);
    CPPUNIT_ASSERT(!section.mapping());
    CPPUNIT_ASSERT_THROW(section.mapping(""), EmptyString);
}


void BaseTestSection::testSectionAccess() {
    std::vector<std::string> names = { "section_a", "section_b", "section_c", "section_d", "section_e" };
    Section null;

    CPPUNIT_ASSERT(section.sectionCount() == 0);
    CPPUNIT_ASSERT(section.sections().size() == 0);
    CPPUNIT_ASSERT(section.getSection("invalid_id") == false);
    CPPUNIT_ASSERT_EQUAL(false, section.hasSection("invalid_id"));

    std::vector<std::string> ids;
    for (auto name : names) {
        Section child_section = section.createSection(name, "metadata");
        CPPUNIT_ASSERT(child_section.name() == name);
        CPPUNIT_ASSERT_EQUAL(true, section.hasSection(name));

        ids.push_back(child_section.id());
    }
    CPPUNIT_ASSERT(section.sectionCount() == names.size());
    CPPUNIT_ASSERT(section.sections().size() == names.size());

    CPPUNIT_ASSERT_THROW(section.createSection(names[0], "metadata"),
                         DuplicateName);
    CPPUNIT_ASSERT_THROW(section.getSection(section.sectionCount()), OutOfBounds);
    CPPUNIT_ASSERT_THROW(section.createSection("", "some type"), EmptyString);
    CPPUNIT_ASSERT(!section.hasSection(null));

    for (auto id : ids) {
        Section child_section = section.getSection(id);
        CPPUNIT_ASSERT(section.hasSection(id));
        CPPUNIT_ASSERT(section.hasSection(child_section));
        CPPUNIT_ASSERT_EQUAL(id, child_section.id());
        section.deleteSection(id);
    }
    Section s2 = section.createSection("a name", "a type");
    CPPUNIT_ASSERT(!section.deleteSection(null));
    CPPUNIT_ASSERT(section.deleteSection(s2));

    CPPUNIT_ASSERT(section.sectionCount() == 0);
    CPPUNIT_ASSERT(section.sections().size() == 0);
    CPPUNIT_ASSERT(section.getSection("invalid_id") == false);

    Section s = section.createSection("Test", "test");
    CPPUNIT_ASSERT(section.deleteSection(s));
    CPPUNIT_ASSERT(!section.deleteSection(s));
}


void BaseTestSection::testFindSection() {
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

    Section l3n1 = l2n1.createSection("l3n1", "typ1");
    Section l3n2 = l2n3.createSection("l3n2", "typ2");
    Section l3n3 = l2n3.createSection("l3n3", "typ2");
    Section l3n4 = l2n5.createSection("l3n4", "typ2");

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

void BaseTestSection::testFindRelated() {
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

    std::string t1 = "t1";
    std::string t3 = "t3";
    std::string t4 = "t4";
    std::string typ2 = "typ2";
    std::string typ1 = "typ1";

    std::vector<Section> related = l1n1.findRelated(util::TypeFilter<Section>(t1));
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


void BaseTestSection::testPropertyAccess() {
    std::vector<std::string> names = { "property_a", "property_b", "property_c", "property_d", "property_e" };

    CPPUNIT_ASSERT(section.propertyCount() == 0);
    CPPUNIT_ASSERT(section.properties().size() == 0);
    CPPUNIT_ASSERT(section.getProperty("invalid_id") == false);
    CPPUNIT_ASSERT_EQUAL(false, section.hasProperty("invalid_id"));

    Property p = section.createProperty("empty_prop", DataType::Double);
    CPPUNIT_ASSERT(section.propertyCount() == 1);
    CPPUNIT_ASSERT(section.hasProperty(p));
    CPPUNIT_ASSERT(section.hasProperty("empty_prop"));
    Property prop = section.getProperty("empty_prop");
    CPPUNIT_ASSERT(prop.dataType() == nix::DataType::Double);
    section.deleteProperty(p.id());
    CPPUNIT_ASSERT(section.propertyCount() == 0);
    CPPUNIT_ASSERT(!section.hasProperty(p));
    CPPUNIT_ASSERT(!section.deleteProperty(p));

    Value dummy(10);
    prop = section.createProperty("single value", dummy);
    CPPUNIT_ASSERT(section.hasProperty("single value"));
    CPPUNIT_ASSERT(section.propertyCount() == 1);
    section.deleteProperty(prop);
    CPPUNIT_ASSERT(section.propertyCount() == 0);

    std::vector<std::string> ids;
    for (auto name : names) {
        prop = section.createProperty(name, dummy);
        CPPUNIT_ASSERT(prop.name() == name);
        CPPUNIT_ASSERT(section.hasProperty(name));

        Property prop_copy = section.getProperty(name);

        CPPUNIT_ASSERT(prop_copy.id() == prop.id());

        ids.push_back(prop.id());
    }
    CPPUNIT_ASSERT_THROW(section.createProperty(names[0], dummy),
                         DuplicateName);

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
}


void BaseTestSection::testOperators() {
    CPPUNIT_ASSERT(section_null == false);
    CPPUNIT_ASSERT(section_null == none);

    CPPUNIT_ASSERT(section != false);
    CPPUNIT_ASSERT(section != none);

    CPPUNIT_ASSERT(section == section);
    CPPUNIT_ASSERT(section != section_other);

    section_other = section;
    CPPUNIT_ASSERT(section == section_other);

    section_other = none;
    CPPUNIT_ASSERT(section_other == false);
    CPPUNIT_ASSERT(section_other == none);

    std::stringstream str1, str2;
    str1 <<  "Section: {name = " << section.name();
    str1 << ", type = " << section.type();
    str1 << ", id = " << section.id() << "}";
    str2 << section;
    CPPUNIT_ASSERT(str1.str() == str2.str());

}


void BaseTestSection::testCreatedAt() {
    CPPUNIT_ASSERT(section.createdAt() >= startup_time);
    time_t past_time = time(NULL) - 10000000;
    section.forceCreatedAt(past_time);
    CPPUNIT_ASSERT(section.createdAt() == past_time);
}


void BaseTestSection::testUpdatedAt() {
    CPPUNIT_ASSERT(section.updatedAt() >= startup_time);
}
