// Copyright (c) 2013 - 2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in Section and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "TestSection.hpp"

#include <nix/util/util.hpp>
#include <nix/valid/validate.hpp>

#include <ctime>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <nix/hdf5/ExceptionHDF5.hpp>

using namespace std;
using namespace nix;
using namespace valid;


void TestSection::setUp() {
    startup_time = time(NULL);
    file = File::open("test_section.h5", FileMode::Overwrite);

    section = file.createSection("section", "metadata");
    section_other = file.createSection("other_section", "metadata");
    section_null  = nullptr;

    file_fs = File::open("test_section", FileMode::Overwrite, Implementation::FileSys);
    section_fs = file_fs.createSection("section_fs", "metadata");
    section_fs_other = file_fs.createSection("other_section", "metadata");
}

void TestSection::tearDown() {
    file.close();
    boost::filesystem::remove_all(boost::filesystem::path(file_fs.location().c_str()));
}


void TestSection::testValidate() {
    valid::Result result = validate(section);
    CPPUNIT_ASSERT(result.getErrors().size() == 0);
    CPPUNIT_ASSERT(result.getWarnings().size() == 0);
}


void TestSection::testId() {
    CPPUNIT_ASSERT(section.id().size() == 36);
    CPPUNIT_ASSERT(section_fs.id().size() == 36);
}


void TestSection::testName() {
    CPPUNIT_ASSERT(section.name() == "section");
    CPPUNIT_ASSERT(section_fs.name() == "section_fs");
}


void TestSection::testType() {
    test_type(section);
    test_type(section_fs);
}

void TestSection::test_type(Section &s) {
    CPPUNIT_ASSERT(s.type() == "metadata");
    string typ = util::createId();
    s.type(typ);
    CPPUNIT_ASSERT(s.type() == typ);
}


void TestSection::testDefinition() {
    test_definition(section);
    test_definition(section_fs);
}

void TestSection::test_definition(Section &s) {
    string def = util::createId();
    s.definition(def);
    CPPUNIT_ASSERT(*s.definition() == def);
    s.definition(nix::none);
    CPPUNIT_ASSERT(s.definition() == nix::none);
}

void TestSection::testParent() {
    CPPUNIT_ASSERT((section.parent() == nullptr));
    Section child = section.createSection("child", "section");
    CPPUNIT_ASSERT(child.parent() != nullptr);
    CPPUNIT_ASSERT(child.parent().id() == section.id());
    CPPUNIT_ASSERT(child.parent().parent() == nullptr);

    CPPUNIT_ASSERT(section_fs.parent() == nullptr);
    Section child_fs = section_fs.createSection("child", "section");
    CPPUNIT_ASSERT(child_fs.parent() != nullptr);
    CPPUNIT_ASSERT(child_fs.parent().id() == section_fs.id());
    CPPUNIT_ASSERT(child_fs.parent().parent() == nullptr);

}


void TestSection::testRepository() {
    test_repository(section);
    test_repository(section_fs);
}

void TestSection::test_repository(Section &s) {
    CPPUNIT_ASSERT(!s.repository());
    string rep = "http://foo.bar/" + util::createId();
    s.repository(rep);
    CPPUNIT_ASSERT(s.repository() == rep);
    s.repository(boost::none);
    CPPUNIT_ASSERT(!s.repository());
    CPPUNIT_ASSERT_THROW(s.repository(""), EmptyString);
}


void TestSection::testLink() {
    test_link(file, section, section_other);
    test_link(file_fs, section_fs, section_fs_other);
}

void TestSection::test_link(File &f, Section &s, Section &other) {
    CPPUNIT_ASSERT(!s.link());

    s.link(other);
    CPPUNIT_ASSERT(s.link());
    CPPUNIT_ASSERT(s.link().id() == other.id());

    // test none-unsetter
    s.link(none);
    CPPUNIT_ASSERT(!s.link());
    s.link(other);
    CPPUNIT_ASSERT(s.link());
    Section null;
    s.link(null);
    CPPUNIT_ASSERT(!s.link());

    // test id setter and resetter
    CPPUNIT_ASSERT_THROW(s.link(""), EmptyString);
    CPPUNIT_ASSERT_THROW(s.link("invalid id"), runtime_error);

    s.link(s.id());
    CPPUNIT_ASSERT(s.link().id() == s.id());
    s.link(other.id());
    CPPUNIT_ASSERT(s.link().id() == other.id());
    // test deleter removing link too
    s.link(s);
    f.deleteSection(s.id());
    CPPUNIT_ASSERT(!s.link());
}


void TestSection::testMapping() {
    test_mapping(section);
    test_mapping(section_fs);
}

void TestSection::test_mapping(Section &s) {
    CPPUNIT_ASSERT(!s.mapping());
    string map = "http://foo.bar/" + util::createId();
    s.mapping(map);
    CPPUNIT_ASSERT(s.mapping() == map);
    s.mapping(boost::none);
    CPPUNIT_ASSERT(!s.mapping());
    CPPUNIT_ASSERT_THROW(s.mapping(""), EmptyString);
}


void TestSection::testSectionAccess() {
    test_section_access(section, Implementation::Hdf5);
    test_section_access(section_fs, Implementation::FileSys);
}

void TestSection::test_section_access(Section &s, Implementation impl) {
    vector<string> names = { "section_a", "section_b", "section_c", "section_d", "section_e" };
    Section null;

    CPPUNIT_ASSERT(s.sectionCount() == 0);
    CPPUNIT_ASSERT(s.sections().size() == 0);
    CPPUNIT_ASSERT(s.getSection("invalid_id") == false);
    CPPUNIT_ASSERT_EQUAL(false, s.hasSection("invalid_id"));

    vector<string> ids;
    for (auto name : names) {
        Section child_section = s.createSection(name, "metadata");
        CPPUNIT_ASSERT(child_section.name() == name);
        CPPUNIT_ASSERT_EQUAL(true, s.hasSection(name));

        ids.push_back(child_section.id());
    }
    CPPUNIT_ASSERT(s.sectionCount() == names.size());
    CPPUNIT_ASSERT(s.sections().size() == names.size());

    CPPUNIT_ASSERT_THROW(s.createSection(names[0], "metadata"),
                         DuplicateName);
    CPPUNIT_ASSERT_THROW(s.hasSection(null), runtime_error);
    CPPUNIT_ASSERT_THROW(s.getSection(s.sectionCount()), OutOfBounds);
    CPPUNIT_ASSERT_THROW(s.createSection("", "some type"), EmptyString);

    for (auto id : ids) {
        Section child_section = s.getSection(id);
        CPPUNIT_ASSERT(s.hasSection(id));
        CPPUNIT_ASSERT_EQUAL(id, child_section.id());

        s.deleteSection(id);
    }
    Section s2 = s.createSection("a name", "a type");
    CPPUNIT_ASSERT_THROW(s.deleteSection(null), runtime_error);
    CPPUNIT_ASSERT_NO_THROW(s.deleteSection(s2));

    CPPUNIT_ASSERT(s.sectionCount() == 0);
    CPPUNIT_ASSERT(s.sections().size() == 0);
    CPPUNIT_ASSERT(s.getSection("invalid_id") == false);
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
    CPPUNIT_ASSERT_EQUAL(false, section.hasProperty("invalid_id"));

    Property p = section.createProperty("empty_prop", DataType::Double);
    CPPUNIT_ASSERT(section.propertyCount() == 1);
    CPPUNIT_ASSERT(section.hasProperty(p));
    CPPUNIT_ASSERT(section.hasProperty("empty_prop"));
    Property prop = section.getProperty("empty_prop");
    CPPUNIT_ASSERT(prop.valueCount() == 0);
    CPPUNIT_ASSERT(prop.dataType() == nix::DataType::Double);
    section.deleteProperty(p.id());
    CPPUNIT_ASSERT(section.propertyCount() == 0);

    Value dummy(10);
    prop = section.createProperty("single value", dummy);
    CPPUNIT_ASSERT(section.hasProperty("single value"));
    CPPUNIT_ASSERT(section.propertyCount() == 1);
    section.deleteProperty(prop.id());
    CPPUNIT_ASSERT(section.propertyCount() == 0);

    vector<string> ids;
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

    vector<Value> values;
    values.push_back(Value(10));
    values.push_back(Value(100));
    section.createProperty("another test", values);
    CPPUNIT_ASSERT(section.propertyCount() == 1);
    prop = section.getProperty("another test");
    CPPUNIT_ASSERT(prop.valueCount() == 2);

    // Filesystem checks
    CPPUNIT_ASSERT(section_fs.propertyCount() == 0);
    CPPUNIT_ASSERT(section_fs.properties().size() == 0);
    CPPUNIT_ASSERT(section_fs.getProperty("invalid_id") == false);
    CPPUNIT_ASSERT_EQUAL(false, section_fs.hasProperty("invalid_id"));

    p = section_fs.createProperty("empty_prop", DataType::Double);
    CPPUNIT_ASSERT(section_fs.propertyCount() == 1);
    CPPUNIT_ASSERT(section_fs.hasProperty(p));
    CPPUNIT_ASSERT(section_fs.hasProperty("empty_prop"));
    prop = section_fs.getProperty("empty_prop");
    CPPUNIT_ASSERT(prop.valueCount() == 0);
    CPPUNIT_ASSERT(prop.dataType() == nix::DataType::Double);
    section_fs.deleteProperty(p.id());
    CPPUNIT_ASSERT(section_fs.propertyCount() == 0);

    /*
    Value dummy(10);
    prop = section.createProperty("single value", dummy);
    CPPUNIT_ASSERT(section.hasProperty("single value"));
    CPPUNIT_ASSERT(section.propertyCount() == 1);
    section.deleteProperty(prop.id());
    CPPUNIT_ASSERT(section.propertyCount() == 0);
    */ // values are not yet supported in filesys backend
    ids.clear();
    for (auto name : names) {
        prop = section_fs.createProperty(name, dummy);
        CPPUNIT_ASSERT(prop.name() == name);
        CPPUNIT_ASSERT(section_fs.hasProperty(name));

        Property prop_copy = section_fs.getProperty(name);
        CPPUNIT_ASSERT(prop_copy.id() == prop.id());
        ids.push_back(prop.id());
    }
    CPPUNIT_ASSERT_THROW(section_fs.createProperty(names[0], dummy),
                         DuplicateName);

    CPPUNIT_ASSERT(section_fs.propertyCount() == names.size());
    CPPUNIT_ASSERT(section_fs.properties().size() == names.size());
    section_fs_other.createProperty("some_prop", dummy);
    section_fs_other.link(section_fs);
    CPPUNIT_ASSERT(section_fs_other.propertyCount() == 1);
    CPPUNIT_ASSERT(section_fs_other.inheritedProperties().size() == names.size() + 1);

    for (auto id : ids) {
        Property prop = section_fs.getProperty(id);
        CPPUNIT_ASSERT(section_fs.hasProperty(id));
        CPPUNIT_ASSERT(prop.id() == id);

        section_fs.deleteProperty(id);
    }

    CPPUNIT_ASSERT(section_fs.propertyCount() == 0);
    CPPUNIT_ASSERT(section_fs.properties().size() == 0);
    CPPUNIT_ASSERT(section_fs.getProperty("invalid_id") == false);
    /*
    vector<Value> values;
    values.push_back(Value(10));
    values.push_back(Value(100));
    section.createProperty("another test", values);
    CPPUNIT_ASSERT(section.propertyCount() == 1);
    prop = section.getProperty("another test");
    CPPUNIT_ASSERT(prop.valueCount() == 2);
    */ //FIXME Values are not supported yet
}


void TestSection::testOperators() {
    test_operators(section, section_other, section_null);
    test_operators(section_fs, section_fs_other, section_null);
}

void TestSection::test_operators(Section &s, Section &other, Section &null) {
    CPPUNIT_ASSERT(null == false);
    CPPUNIT_ASSERT(null == none);

    CPPUNIT_ASSERT(s != false);
    CPPUNIT_ASSERT(s != none);

    CPPUNIT_ASSERT(s == s);
    CPPUNIT_ASSERT(s != other);

    other = s;
    CPPUNIT_ASSERT(s == other);

    other = none;
    CPPUNIT_ASSERT(other == false);
    CPPUNIT_ASSERT(other == none);

    stringstream str1, str2;
    str1 <<  "Section: {name = " << s.name();
    str1 << ", type = " << s.type();
    str1 << ", id = " << s.id() << "}";
    str2 << s;
    CPPUNIT_ASSERT(str1.str() == str2.str());

}

void TestSection::testCreatedAt() {
    CPPUNIT_ASSERT(section.createdAt() >= startup_time);
    time_t past_time = time(NULL) - 10000000;
    section.forceCreatedAt(past_time);
    CPPUNIT_ASSERT(section.createdAt() == past_time);

    CPPUNIT_ASSERT(section_fs.createdAt() >= startup_time);
    section_fs.forceCreatedAt(past_time);
    CPPUNIT_ASSERT(section_fs.createdAt() == past_time);
}


void TestSection::testUpdatedAt() {
    CPPUNIT_ASSERT(section.updatedAt() >= startup_time);
    CPPUNIT_ASSERT(section_fs.updatedAt() >= startup_time);
}
