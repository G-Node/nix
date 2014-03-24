// Copyright © 2014 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <kellner@bio.lmu.de>

#include <ctime>
#include <iostream>

#include <nix/util/util.hpp>
#include "TestProperty.hpp"

using namespace std;
using namespace nix;


void TestProperty::setUp()
{
    startup_time = time(NULL);
    file = nix::File::open("test_property.h5", nix::FileMode::Overwrite);

    section = file.createSection("cool section", "metadata");

    property = section.createProperty("prop", "sometype");
    property_other = section.createProperty("other", "sometype");
    property_null = nullptr;
}


void TestProperty::tearDown()
{
    file.close();
}


void TestProperty::testId() {
    CPPUNIT_ASSERT(property.id().size() == 25);
}


void TestProperty::testName() {
    CPPUNIT_ASSERT(*property.name() == "prop");
    string name = util::createId("", 32);
    property.name(name);
    CPPUNIT_ASSERT(*property.name() == name);
    property.name(none);
    CPPUNIT_ASSERT(*property.name() == "");
}


void TestProperty::testType() {
    string typ = "some_str";
    property.type(typ);
    CPPUNIT_ASSERT(property.type() == typ);
}


void TestProperty::testDefinition() {
    string def = "some_str";
    property.definition(def);
    CPPUNIT_ASSERT(*property.definition() == def);
    property.definition(boost::none);
    CPPUNIT_ASSERT(*property.definition() == "");
}


void TestProperty::testMapping() {
    string map = "some_str";
    property.mapping(map);
    CPPUNIT_ASSERT(*property.mapping() == map);
    property.mapping(boost::none);
    CPPUNIT_ASSERT((property.mapping()) == false);
}


void TestProperty::testValues()
{
    nix::Section section = file.createSection("Area51", "Boolean");
    nix::Property p1 = section.createProperty("strProperty", "str");


    std::vector<nix::Value> strValues = { nix::Value("Freude"),
                                          nix::Value("schoener"),
                                          nix::Value("Goetterfunken") };

    p1.values(strValues);
    CPPUNIT_ASSERT_EQUAL(p1.valueCount(), strValues.size());

    std::vector<nix::Value> ctrlValues = p1.values();

    for(size_t i = 0; i < ctrlValues.size(); ++i) {
        CPPUNIT_ASSERT_EQUAL(strValues[i], ctrlValues[i]);
    }

    strValues.emplace_back("Tochter");
    strValues.emplace_back("aus");
    strValues.emplace_back("Elysium");
    strValues.emplace_back("Wir betreten feuertrunken");

    p1.values(strValues);
    CPPUNIT_ASSERT_EQUAL(p1.valueCount(), strValues.size());

    strValues.erase(strValues.begin()+6);
    p1.values(strValues);
    CPPUNIT_ASSERT_EQUAL(p1.valueCount(), strValues.size());

    nix::Property p2 = section.createProperty("toDelete", "tmp");
    p2.values(strValues);
    CPPUNIT_ASSERT_EQUAL(p2.valueCount(), strValues.size());
    strValues.clear();
    p2.values(strValues);
    CPPUNIT_ASSERT_EQUAL(p2.valueCount(), strValues.size());
    p2.deleteValues();
    p2.values(none);
    CPPUNIT_ASSERT(p2.values().empty() == true);
}


void TestProperty::testDataType() {
    nix::Section section = file.createSection("Area51", "Boolean");
    nix::Property p1 = section.createProperty("strProperty", "str");
    nix::Property p2 = section.createProperty("doubleProperty", "dbl");
    CPPUNIT_ASSERT(!p1.dataType());
    std::vector<nix::Value> strValues = { nix::Value("Freude"),
                                          nix::Value("schoener"),
                                          nix::Value("Goetterfunken") };
    p1.values(strValues);
    CPPUNIT_ASSERT(p1.dataType() && *p1.dataType() == DataType::String);
    p1.deleteValues();
    CPPUNIT_ASSERT(!p1.dataType());

    std::vector<nix::Value> doubleValues = { nix::Value(1.0),
                                             nix::Value(2.0),
                                             nix::Value(-99.99) };
    p2.values(doubleValues);
    CPPUNIT_ASSERT(p2.dataType() && *p2.dataType() == DataType::Double);

    file.deleteSection(section.id());
}


void TestProperty::testUnit(){
    nix::Section section = file.createSection("testSection", "test");
    nix::Property p1 = section.createProperty("testProperty", "testType");
    nix::Value v(22.2);
    v.uncertainty = 1.2;
    std::vector<Value> values = {v};
    std::string inv_unit = "invalid unit";
    std::string valid_unit = "mV*cm^-2";
    std::string second_unit = "mV";
    p1.values(values);

    CPPUNIT_ASSERT_THROW(p1.unit(inv_unit), nix::InvalidUnit);
    CPPUNIT_ASSERT(!p1.unit());
    p1.unit(valid_unit);
    CPPUNIT_ASSERT(p1.unit() && *p1.unit() == valid_unit);
    CPPUNIT_ASSERT_THROW(p1.unit(second_unit), runtime_error);
    p1.unit(none);
    CPPUNIT_ASSERT(!p1.unit());
    CPPUNIT_ASSERT_NO_THROW(p1.unit(second_unit));
    CPPUNIT_ASSERT(p1.unit() && *p1.unit() == second_unit);
}


void TestProperty::testOperators() {
    CPPUNIT_ASSERT(property_null == false);
    CPPUNIT_ASSERT(property_null == none);

    CPPUNIT_ASSERT(property != false);
    CPPUNIT_ASSERT(property != none);

    CPPUNIT_ASSERT(property == property);
    CPPUNIT_ASSERT(property != property_other);

    property_other = property;
    CPPUNIT_ASSERT(property == property_other);

    property_other = none;
    CPPUNIT_ASSERT(property_null == false);
    CPPUNIT_ASSERT(property_null == none);
}


void TestProperty::testCreatedAt() {
    CPPUNIT_ASSERT(property.createdAt() >= startup_time);
    time_t past_time = time(NULL) - 10000000;
    property.forceCreatedAt(past_time);
    CPPUNIT_ASSERT(property.createdAt() == past_time);
}


void TestProperty::testUpdatedAt() {
    CPPUNIT_ASSERT(property.updatedAt() >= startup_time);
}

