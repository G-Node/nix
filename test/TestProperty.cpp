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

    property = section.createProperty("prop");
    property_other = section.createProperty("other");
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
    CPPUNIT_ASSERT(property.name() == "prop");
    string name = util::createId("", 32);
    property.name(name);
    CPPUNIT_ASSERT(property.name() == name);
}


void TestProperty::testType() {
    string typ = util::createId("", 32);
    property.type(typ);
    CPPUNIT_ASSERT(property.type() == typ);
}


void TestProperty::testDefinition() {
    string def = util::createId("", 128);
    property.definition(def);
    CPPUNIT_ASSERT(property.definition() == def);
}


void TestProperty::testValues()
{
    nix::Section section = file.createSection("Area51", "Boolean");
    nix::Property p1 = section.createProperty("strProperty");


    std::vector<nix::Value> strValues = { nix::Value("Freude"),
                                          nix::Value("schoener"),
                                          nix::Value("Goetterfunken") };

    p1.setValues(strValues);
    CPPUNIT_ASSERT_EQUAL(p1.valueCount(), strValues.size());

    std::vector<nix::Value> ctrlValues = p1.getValues();

    for(size_t i = 0; i < ctrlValues.size(); ++i) {
        CPPUNIT_ASSERT_EQUAL(strValues[i], ctrlValues[i]);
    }

    strValues.emplace_back("Tochter");
    strValues.emplace_back("aus");
    strValues.emplace_back("Elysium");
    strValues.emplace_back("Wir betreten feuertrunken");

    p1.setValues(strValues);
    CPPUNIT_ASSERT_EQUAL(p1.valueCount(), strValues.size());

    strValues.erase(strValues.begin()+6);
    p1.setValues(strValues);
    CPPUNIT_ASSERT_EQUAL(p1.valueCount(), strValues.size());

    nix::Property p2 = section.createProperty("toDelete");
    p2.setValues(strValues);
    CPPUNIT_ASSERT_EQUAL(p2.valueCount(), strValues.size());
    strValues.clear();
    p2.setValues(strValues);
    CPPUNIT_ASSERT_EQUAL(p2.valueCount(), strValues.size());
    p2.removeValues();
}


void TestProperty::testOperators() {
    CPPUNIT_ASSERT(property_null == NULL);
    CPPUNIT_ASSERT(property_null == nullptr);

    CPPUNIT_ASSERT(property != NULL);
    CPPUNIT_ASSERT(property != nullptr);

    CPPUNIT_ASSERT(property == property);
    CPPUNIT_ASSERT(property != property_other);

    property_other = property;
    CPPUNIT_ASSERT(property == property_other);

    property_other = nullptr;
    CPPUNIT_ASSERT(property_null == NULL);
    CPPUNIT_ASSERT(property_null == nullptr);
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

