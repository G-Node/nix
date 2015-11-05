// Copyright © 2014 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <kellner@bio.lmu.de>

#ifndef NIX_BASETESTPROPERTY_HPP
#define NIX_BASETESTPROPERTY_HPP

#include <nix.hpp>

#include <cppunit/TestFixture.h>


class BaseTestProperty : public CPPUNIT_NS::TestFixture {

protected:
    time_t startup_time;
    nix::File file;
    nix::Section section;
    nix::Property property, property_other, property_null;
    nix::Value int_dummy, str_dummy;

public:
    void testValidate();
    void testId();
    void testName();
    void testDefinition();
    void testMapping();
    void testDataType();
    void testValues();
    void testUnit();

    void testOperators();
    void testUpdatedAt();
    void testCreatedAt();

};

#endif // NIX_BASETESTPROPERTY_HPP
