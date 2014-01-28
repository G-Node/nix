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

void TestProperty::setUp()
{
    file = nix::File::open("test_property.h5", nix::FileMode::Overwrite);
}

void TestProperty::tearDown()
{

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
