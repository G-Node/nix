// Copyright © 2014 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <kellner@bio.lmu.de>

#include "TestProperty.hpp"

#include <nix/util/util.hpp>
#include <nix/valid/validate.hpp>

#include <ctime>
#include <iostream>


using namespace std;
using namespace nix;
using namespace valid;


void TestProperty::setUp()
{
    startup_time = time(NULL);
    file = nix::File::open("test_property.h5", nix::FileMode::Overwrite);
    section = file.createSection("cool section", "metadata");
    int_dummy = Value(10);
    str_dummy = Value("test");
    property = section.createProperty("prop", int_dummy);
    property_other = section.createProperty("other", int_dummy);
    property_null = nix::none;

    file_fs = nix::File::open("test_property", nix::FileMode::Overwrite, nix::Implementation::FileSys);
    section_fs = file_fs.createSection("cool section", "metadata");
    property_fs = section.createProperty("test_prop", DataType::Double);
    property_other_fs = section.createProperty("other test_prop", DataType::Double);
}


void TestProperty::tearDown()
{
    file.close();
}


void TestProperty::testValidate() {
    // values are set but unit is missing: 1 warning
    valid::Result result = validate(property);
    CPPUNIT_ASSERT(result.getErrors().size() == 0);
    CPPUNIT_ASSERT(result.getWarnings().size() == 1);
}


void TestProperty::testId() {
    CPPUNIT_ASSERT(property.id().size() == 36);
    CPPUNIT_ASSERT(property_fs.id().size() == 36);
}


void TestProperty::testName() {
    CPPUNIT_ASSERT(property.name() == "prop");
    CPPUNIT_ASSERT(property_fs.name() == "test_prop");
}


void TestProperty::testDefinition() {
    test_definition(property);
    test_definition(property_fs);
}

void TestProperty::test_definition(Property &p) {
    string def = "some_str";
    CPPUNIT_ASSERT_THROW(p.definition(""), EmptyString);
    p.definition(def);
    CPPUNIT_ASSERT(*p.definition() == def);
    p.definition(nix::none);
    CPPUNIT_ASSERT(p.definition() == nix::none);
}


void TestProperty::testMapping() {
    test_mapping(property);
    test_mapping(property_fs);
}

void TestProperty::test_mapping(Property &p) {
    string map = "some_str";
    CPPUNIT_ASSERT_THROW(p.mapping(""), EmptyString);
    p.mapping(map);
    CPPUNIT_ASSERT(*p.mapping() == map);
    p.mapping(boost::none);
    CPPUNIT_ASSERT(!p.mapping());
}


void TestProperty::testValues()
{
    nix::Section section = file.createSection("Area51", "Boolean");
    nix::Property p1 = section.createProperty("strProperty", str_dummy);
    std::vector<nix::Value> strValues = { nix::Value("Freude"),
                                          nix::Value("schoener"),
                                          nix::Value("Goetterfunken") };
    p1.values(strValues);
    CPPUNIT_ASSERT_EQUAL(p1.valueCount(), static_cast<ndsize_t>(strValues.size()));

    std::vector<nix::Value> ctrlValues = p1.values();
    for(size_t i = 0; i < ctrlValues.size(); ++i) {
        CPPUNIT_ASSERT_EQUAL(strValues[i], ctrlValues[i]);
    }

    strValues.emplace_back("Tochter");
    strValues.emplace_back("aus");
    strValues.emplace_back("Elysium");
    strValues.emplace_back("Wir betreten feuertrunken");

    p1.values(strValues);
    CPPUNIT_ASSERT_EQUAL(p1.valueCount(), static_cast<ndsize_t>(strValues.size()));

    strValues.erase(strValues.begin()+6);
    p1.values(strValues);
    CPPUNIT_ASSERT_EQUAL(p1.valueCount(), static_cast<ndsize_t>(strValues.size()));

    nix::Property p2 = section.createProperty("toDelete", str_dummy);
    CPPUNIT_ASSERT(p2.valueCount() == 1);
    CPPUNIT_ASSERT_EQUAL(p2.values()[0], str_dummy);

    strValues.clear();
    p2.values(strValues);
    CPPUNIT_ASSERT_EQUAL(p2.valueCount(), static_cast<ndsize_t>(strValues.size()));
    p2.deleteValues();
    CPPUNIT_ASSERT(p2.values().empty() == true);
    p2.values(strValues);
    p2.values(none);
    CPPUNIT_ASSERT(p2.values().empty() == true);
}


void TestProperty::testDataType() {
    nix::Section section = file.createSection("Area51", "Boolean");
    std::vector<nix::Value> strValues = { nix::Value("Freude"),
                                          nix::Value("schoener"),
                                          nix::Value("Goetterfunken") };
    std::vector<nix::Value> doubleValues = { nix::Value(1.0),
                                             nix::Value(2.0),
                                             nix::Value(-99.99) };
    nix::Property p1 = section.createProperty("strProperty", strValues);
    nix::Property p2 = section.createProperty("doubleProperty", doubleValues);

    CPPUNIT_ASSERT(p1.dataType() == DataType::String);
    CPPUNIT_ASSERT(p2.dataType() == DataType::Double);

    file.deleteSection(section.id());
}


void TestProperty::testUnit(){
    test_unit(property);
    test_unit(property_fs);
}

void TestProperty::test_unit(Property &p) {
    std::string inv_unit = "invalid unit";
    std::string valid_unit = "mV*cm^-2";
    std::string second_unit = "mV";

    CPPUNIT_ASSERT_THROW(p.unit(""), nix::EmptyString);
    CPPUNIT_ASSERT_THROW(p.unit(inv_unit), nix::InvalidUnit);
    CPPUNIT_ASSERT(!p.unit());

    p.unit(valid_unit);
    CPPUNIT_ASSERT(p.unit() && *p.unit() == valid_unit);

    p.unit(none);
    CPPUNIT_ASSERT(!p.unit());
    CPPUNIT_ASSERT_NO_THROW(p.unit(second_unit));
    CPPUNIT_ASSERT(p.unit() && *p.unit() == second_unit);
}


void TestProperty::testOperators() {
    test_operators(property, property_other, property_null);
    test_operators(property_fs, property_other_fs, property_null);
}

void TestProperty::test_operators(nix::Property &p, nix::Property &other, nix::Property &null) {
    CPPUNIT_ASSERT(null == false);
    CPPUNIT_ASSERT(null == none);

    CPPUNIT_ASSERT(p != false);
    CPPUNIT_ASSERT(p != none);

    CPPUNIT_ASSERT(p == p);
    CPPUNIT_ASSERT(p != other);
    CPPUNIT_ASSERT(p.compare(other) != 0);

    other = p;
    CPPUNIT_ASSERT(p == other);
    CPPUNIT_ASSERT(p.compare(other) == 0);

    other = none;
    CPPUNIT_ASSERT(other == false);
    CPPUNIT_ASSERT(other == none);

    stringstream s;
    s << p;
    CPPUNIT_ASSERT(s.str() == "Property: {name = " + p.name() + "}");
}


void TestProperty::testCreatedAt() {
    test_created_at(property);
    test_created_at(property_fs);
}

void TestProperty::test_created_at(Property &p) {
    CPPUNIT_ASSERT(p.createdAt() >= startup_time);
    time_t past_time = time(NULL) - 10000000;
    p.forceCreatedAt(past_time);
    CPPUNIT_ASSERT(p.createdAt() == past_time);
}

void TestProperty::testUpdatedAt() {
    CPPUNIT_ASSERT(property.updatedAt() >= startup_time);
    CPPUNIT_ASSERT(property_fs.updatedAt() >= startup_time);
}

