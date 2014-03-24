// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <ctime>

#include "TestUtil.hpp"

using namespace std;
using namespace nix;


void TestUtil::testUnitScaling(){
    CPPUNIT_ASSERT_THROW(util::getSIScaling("mOhm","ms"), nix::InvalidUnit);
    CPPUNIT_ASSERT(util::getSIScaling("mV","kV") == 1e+6);
    CPPUNIT_ASSERT(util::getSIScaling("V","kV") == 1e+03);
    CPPUNIT_ASSERT(util::getSIScaling("kV","V") == 1e-03);
}

void TestUtil::testIsSIUnit(){
    CPPUNIT_ASSERT(util::isSIUnit("V"));
    CPPUNIT_ASSERT(util::isSIUnit("mV"));
    CPPUNIT_ASSERT(util::isSIUnit("mV^-2"));
    CPPUNIT_ASSERT(util::isSIUnit("mV/cm") == false);
}

void TestUtil::testSIUnitSplit(){
    string unit_1 = "V";
    string unit_2 = "mV";
    string unit_3 = "mV^2";
    string unit_4 = "mV^-2";

    string unit, prefix, power;
    util::splitUnit(unit_1, prefix, unit, power);
    CPPUNIT_ASSERT(prefix.compare("") == 0 && unit.compare("V") == 0 && power.compare("") == 0);
    util::splitUnit(unit_2, prefix, unit, power);
    CPPUNIT_ASSERT(prefix.compare("m") == 0 && unit.compare("V") == 0 && power.compare("") == 0);
    util::splitUnit(unit_3, prefix, unit, power);
    CPPUNIT_ASSERT(prefix.compare("m") == 0 && unit.compare("V") == 0 && power.compare("^2") == 0);
    util::splitUnit(unit_4, prefix, unit, power);
    CPPUNIT_ASSERT(prefix.compare("m") == 0 && unit.compare("V") == 0 && power.compare("^-2") == 0);
}

void TestUtil::testIsCompoundSIUnit(){
    string unit_1 = "mV*cm^-2";
    string unit_2 = "mV/cm^2";
    string unit_3 = "mV/cm^2*kg";
    string unit_4 = "mV";

    CPPUNIT_ASSERT(util::isCompoundSIUnit(unit_1));
    CPPUNIT_ASSERT(util::isCompoundSIUnit(unit_2));
    CPPUNIT_ASSERT(util::isCompoundSIUnit(unit_3));
    CPPUNIT_ASSERT(!util::isCompoundSIUnit(unit_4));
}


void TestUtil::testSplitCompoundUnit(){
    string unit = "mV/cm^2*kg*V";
    vector<string> atomic_units;

    util::splitCompoundUnit(unit, atomic_units);
    CPPUNIT_ASSERT(atomic_units.size() == 4);
    CPPUNIT_ASSERT(atomic_units[0].compare("mV") == 0 && atomic_units[1].compare("cm^2") == 0 &&
            atomic_units[2].compare("kg") == 0 && atomic_units[3].compare("V") == 0);
}
