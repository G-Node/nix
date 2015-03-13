// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "TestUtil.hpp"

#include <ctime>
#include <cmath>


using namespace std;
using namespace nix;


void TestUtil::testStrUtils() {
    string a = " a ";
    string b = "\tb\t";
    string c = " c c\tc ";

    const string ac = a;
    const string bc = b;
    const string cc = c;

    string ad = util::deblankString(ac);
    string bd = util::deblankString(bc);
    string cd = util::deblankString(cc);

    util::deblankString(a);
    util::deblankString(b);
    util::deblankString(c);

    CPPUNIT_ASSERT_EQUAL(string("a"), a);
    CPPUNIT_ASSERT_EQUAL(string("b"), b);
    CPPUNIT_ASSERT_EQUAL(string("ccc"), c);

    CPPUNIT_ASSERT_EQUAL(ad, a);
    CPPUNIT_ASSERT_EQUAL(bd, b);
    CPPUNIT_ASSERT_EQUAL(cd, c);
}

void TestUtil::testUnitScaling() {
    CPPUNIT_ASSERT_THROW(util::getSIScaling("mOhm","ms"), nix::InvalidUnit);
    CPPUNIT_ASSERT(util::getSIScaling("mV","kV") == 1e-6);
    CPPUNIT_ASSERT(util::getSIScaling("V","kV") == 1e-03);
    CPPUNIT_ASSERT(util::getSIScaling("kV","V") == 1e+03);
    CPPUNIT_ASSERT_THROW(util::getSIScaling("mV^2","V"), nix::InvalidUnit);
    CPPUNIT_ASSERT(util::getSIScaling("V^2","V^2") == 1.0);
    CPPUNIT_ASSERT(util::getSIScaling("V","mV") == 1e+03);
    CPPUNIT_ASSERT(util::getSIScaling("V^2","mV^2") == 1e+06);
    CPPUNIT_ASSERT(util::getSIScaling("mV^2","kV^2") == 1e-12);
}

void TestUtil::testIsSIUnit() {
    CPPUNIT_ASSERT(util::isSIUnit("V"));
    CPPUNIT_ASSERT(util::isSIUnit("mV"));
    CPPUNIT_ASSERT(util::isSIUnit("mV^-2"));
    CPPUNIT_ASSERT(util::isSIUnit("mV/cm"));
    CPPUNIT_ASSERT(util::isSIUnit("dB"));
    CPPUNIT_ASSERT(util::isSIUnit("rad"));
}

void TestUtil::testSIUnitSplit() {
    string unit_1 = "V";
    string unit_2 = "mV";
    string unit_3 = "mV^2";
    string unit_4 = "mV^-2";
    string unit_5 = "m^2";

    string unit, prefix, power;
    util::splitUnit(unit_1, prefix, unit, power);
    CPPUNIT_ASSERT(prefix == "" && unit == "V" && power == "");
    util::splitUnit(unit_2, prefix, unit, power);
    CPPUNIT_ASSERT(prefix == "m" && unit == "V" && power == "");
    util::splitUnit(unit_3, prefix, unit, power);
    CPPUNIT_ASSERT(prefix == "m" && unit == "V" && power == "2");
    util::splitUnit(unit_4, prefix, unit, power);
    CPPUNIT_ASSERT(prefix == "m" && unit == "V" && power == "-2");
    util::splitUnit(unit_5, prefix, unit, power);
    CPPUNIT_ASSERT(prefix == "" && unit == "m" && power == "2");
}

void TestUtil::testIsAtomicSIUnit() {
    CPPUNIT_ASSERT(util::isAtomicSIUnit("V"));
    CPPUNIT_ASSERT(util::isAtomicSIUnit("mV"));
    CPPUNIT_ASSERT(util::isAtomicSIUnit("mV^-2"));
    CPPUNIT_ASSERT(!util::isAtomicSIUnit("mV/cm"));
    CPPUNIT_ASSERT(util::isAtomicSIUnit("dB"));
    CPPUNIT_ASSERT(util::isAtomicSIUnit("rad"));
}

void TestUtil::testIsCompoundSIUnit() {
    string unit_1 = "mV*cm^-2";
    string unit_2 = "mV/cm^2";
    string unit_3 = "mV/cm^2*kg";
    string unit_4 = "mV";

    CPPUNIT_ASSERT(util::isCompoundSIUnit(unit_1));
    CPPUNIT_ASSERT(util::isCompoundSIUnit(unit_2));
    CPPUNIT_ASSERT(util::isCompoundSIUnit(unit_3));
    CPPUNIT_ASSERT(!util::isCompoundSIUnit(unit_4));
}


void TestUtil::testSplitCompoundUnit() {
    string unit = "mV/cm^2*kg*V";
    string unit_2 = "mOhm/m";
    string unit_3 = "mV";

    vector<string> atomic_units, atomic_units_2, atomic_units_3;
    util::splitCompoundUnit(unit, atomic_units);
    CPPUNIT_ASSERT(atomic_units.size() == 4);
    CPPUNIT_ASSERT(atomic_units[0] == "mV" && atomic_units[1] == "cm^-2" &&
                   atomic_units[2] == "kg" && atomic_units[3] == "V");

    util::splitCompoundUnit(unit_2, atomic_units_2);
    CPPUNIT_ASSERT(atomic_units_2.size() == 2);
    CPPUNIT_ASSERT(atomic_units_2[0] == "mOhm" && atomic_units_2[1] == "m^-1");

    util::splitCompoundUnit(unit_3, atomic_units_3);
    CPPUNIT_ASSERT(atomic_units_3.size() == 1);
    CPPUNIT_ASSERT(atomic_units_3[0] == unit_3);
}


void TestUtil::testConvertToSeconds() {
    string unit_min = "min";
    string unit_h = "h";
    string unit_s = "s";
    string unit_ms = "ms";
    string unit_Ms = "Ms";
    double min_value = 25.5;
    double h_value = 12.25;
    double s_value = 100;
    int64_t m_value = 25;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1530.0, util::convertToSeconds(unit_min, min_value), std::numeric_limits<double>::round_error());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(44100.0, util::convertToSeconds(unit_h, h_value), std::numeric_limits<double>::round_error());
    CPPUNIT_ASSERT(1500 == util::convertToSeconds(unit_min, m_value));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(s_value, util::convertToSeconds(unit_s, s_value), std::numeric_limits<double>::round_error());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(s_value/1000., util::convertToSeconds(unit_ms, s_value), std::numeric_limits<double>::round_error());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(s_value*1000000., util::convertToSeconds(unit_Ms, s_value), std::numeric_limits<double>::round_error());
}

void TestUtil::testConvertToKelvin() {
    string unit_f = "°F";
    string unit_f2 = "F";
    string unit_c = "°C";
    string unit_c2 = "C";
    string unit_k = "K";
    string unit_mk = "mK";
    string unit_Mk = "MK";
    string unit_k2 ="°K" ;
    double temperature = 100.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(373.15, util::convertToKelvin(unit_c, temperature), std::numeric_limits<double>::round_error());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(373.15, util::convertToKelvin(unit_c2, temperature), std::numeric_limits<double>::round_error());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(311.0, round(util::convertToKelvin(unit_f, temperature)), std::numeric_limits<double>::round_error());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(311.0, round(util::convertToKelvin(unit_f2, temperature)), std::numeric_limits<double>::round_error());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(temperature, util::convertToKelvin(unit_k, temperature), std::numeric_limits<double>::round_error());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(temperature, util::convertToKelvin(unit_k2, temperature), std::numeric_limits<double>::round_error());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(temperature/1000., util::convertToKelvin(unit_mk, temperature), std::numeric_limits<double>::round_error());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(temperature*1000000., util::convertToKelvin(unit_Mk, temperature), std::numeric_limits<double>::round_error());
    int temp_fi = 100;
    CPPUNIT_ASSERT(util::convertToKelvin(unit_f, temp_fi) == 311);
}

void TestUtil::testUnitSanitizer() {
    std::string unit = " mul/µs ";
    CPPUNIT_ASSERT(util::unitSanitizer(unit) == "ul/us");
}
