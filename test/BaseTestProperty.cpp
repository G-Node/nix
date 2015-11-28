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
#include <sstream>
#include <iterator>
#include <stdexcept>
#include <limits>

#include <boost/math/constants/constants.hpp>
#include <boost/iterator/zip_iterator.hpp>

#include <nix/util/util.hpp>
#include <nix/valid/validate.hpp>

#include "BaseTestProperty.hpp"

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>


using namespace nix;
using namespace valid;


void BaseTestProperty::testValidate() {
    // values are set but unit is missing: 1 warning
    valid::Result result = validate(property);
    CPPUNIT_ASSERT(result.getErrors().size() == 0);
    CPPUNIT_ASSERT(result.getWarnings().size() == 1);
}


void BaseTestProperty::testId() {
    CPPUNIT_ASSERT(property.id().size() == 36);
}


void BaseTestProperty::testName() {
    CPPUNIT_ASSERT(property.name() == "prop");
}


void BaseTestProperty::testDefinition() {
    std::string def = "some_str";
    property.definition(def);
    CPPUNIT_ASSERT(*property.definition() == def);
    property.definition(nix::none);
    CPPUNIT_ASSERT(property.definition() == nix::none);
    CPPUNIT_ASSERT_THROW(property.definition(""), EmptyString);
}


void BaseTestProperty::testMapping() {
    std::string map = "some_str";
    CPPUNIT_ASSERT_THROW(property.mapping(""), EmptyString);
    property.mapping(map);
    CPPUNIT_ASSERT(*property.mapping() == map);
    property.mapping(boost::none);
    CPPUNIT_ASSERT(!property.mapping());
}


void BaseTestProperty::testValues()
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


void BaseTestProperty::testDataType() {
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


void BaseTestProperty::testUnit(){
    nix::Section section = file.createSection("testSection", "test");
    nix::Value v(22.2);
    v.uncertainty = 1.2;
    std::vector<Value> values = {v};
    nix::Property p1 = section.createProperty("testProperty", int_dummy);

    std::string inv_unit = "invalid unit";
    std::string valid_unit = "mV*cm^-2";
    std::string second_unit = "mV";

    CPPUNIT_ASSERT_THROW(property.unit(""), nix::EmptyString);
    CPPUNIT_ASSERT_THROW(property.unit(inv_unit), nix::InvalidUnit);
    CPPUNIT_ASSERT(!property.unit());

    property.unit(valid_unit);
    CPPUNIT_ASSERT(property.unit() && *property.unit() == valid_unit);

    property.unit(none);
    CPPUNIT_ASSERT(!property.unit());
    CPPUNIT_ASSERT_NO_THROW(property.unit(second_unit));
    CPPUNIT_ASSERT(property.unit() && *property.unit() == second_unit);
}


void BaseTestProperty::testOperators() {
    CPPUNIT_ASSERT(property_null == false);
    CPPUNIT_ASSERT(property_null == none);

    CPPUNIT_ASSERT(property != false);
    CPPUNIT_ASSERT(property != none);

    CPPUNIT_ASSERT(property == property);
    CPPUNIT_ASSERT(property != property_other);
    CPPUNIT_ASSERT(property.compare(property_other) != 0);

    property_other = property;
    CPPUNIT_ASSERT(property == property_other);
    CPPUNIT_ASSERT(property.compare(property_other) == 0);

    property_other = none;
    CPPUNIT_ASSERT(property_other == false);
    CPPUNIT_ASSERT(property_other == none);

    CPPUNIT_ASSERT(property_null == false);
    CPPUNIT_ASSERT(property_null == none);
    
    std::stringstream s;
    s << property;
    CPPUNIT_ASSERT(s.str() == "Property: {name = " + property.name() + "}");
}


void BaseTestProperty::testCreatedAt() {
    CPPUNIT_ASSERT(property.createdAt() >= startup_time);
    time_t past_time = time(NULL) - 10000000;
    property.forceCreatedAt(past_time);
    CPPUNIT_ASSERT(property.createdAt() == past_time);
}


void BaseTestProperty::testUpdatedAt() {
    CPPUNIT_ASSERT(property.updatedAt() >= startup_time);
}

void BaseTestProperty::testIsValidEntity() {
    Property p = section.createProperty("testProperty", DataType::Double);
    CPPUNIT_ASSERT(p.isValidEntity());
    section.deleteProperty(p.name());
    CPPUNIT_ASSERT(!p.isValidEntity());
}
