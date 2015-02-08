// Copyright © 2013,2014 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <kellner@bio.lmu.de>

#include "TestValue.hpp"

void TestValue::setUp() {
}

void TestValue::tearDown() {
}

void TestValue::testObject() {

    nix::Value boolVal(true);
    CPPUNIT_ASSERT_EQUAL(boolVal.get<bool>(), true);
    CPPUNIT_ASSERT_THROW(boolVal.get<int32_t>(), std::invalid_argument);

    const int32_t i32c = 42;
    nix::Value i32Val(i32c);
    CPPUNIT_ASSERT_EQUAL(i32Val.get<int32_t>(), i32c);
    CPPUNIT_ASSERT_THROW(i32Val.get<bool>(), std::invalid_argument);

    const uint32_t ui32c = 42U;
    nix::Value ui32Val(ui32c);
    CPPUNIT_ASSERT_EQUAL(ui32Val.get<uint32_t>(), ui32c);
    CPPUNIT_ASSERT_THROW(ui32Val.get<bool>(), std::invalid_argument);

    const int64_t i64c = 42;
    nix::Value i64Val(i64c);
    CPPUNIT_ASSERT_EQUAL(i64Val.get<int64_t>(), i64c);
    CPPUNIT_ASSERT_THROW(i64Val.get<bool>(), std::invalid_argument);

    const uint64_t ui64c = 42U;
    nix::Value ui64Val(ui64c);
    CPPUNIT_ASSERT_EQUAL(ui64Val.get<uint64_t>(), ui64c);
    CPPUNIT_ASSERT_THROW(ui64Val.get<bool>(), std::invalid_argument);

    const double dc = 2.71828;
    nix::Value dVal(dc);
    CPPUNIT_ASSERT_EQUAL(dVal.get<double>(), dc);
    CPPUNIT_ASSERT_THROW(dVal.get<bool>(), std::invalid_argument);

    const std::string strc = "When shall we three meet again";
    nix::Value sVal(strc);
    CPPUNIT_ASSERT_EQUAL(sVal.get<std::string>(), strc);
    CPPUNIT_ASSERT_THROW(sVal.get<bool>(), std::invalid_argument);


    nix::Value v1 = sVal; //copy constructor
    CPPUNIT_ASSERT_EQUAL(v1, sVal); // check ==
    CPPUNIT_ASSERT(v1 != dVal);

    CPPUNIT_ASSERT_EQUAL(v1.get<std::string>(), strc);

    v1 = dVal; //assignment op (copy)
    CPPUNIT_ASSERT_EQUAL(v1.get<double>(), dc);

    v1 = nix::Value(31337); //assignment op (move)
    CPPUNIT_ASSERT_EQUAL(v1.get<int>(), 31337);

    v1.set(nix::none); //set v1 to none
    CPPUNIT_ASSERT_EQUAL(v1.type(), nix::DataType::Nothing);
}


void TestValue::testSwap()
{
    using std::swap;

    nix::Value v1("Hallo");
    nix::Value v2("Welt");

    v1.swap(v2);

    CPPUNIT_ASSERT_EQUAL(v1.get<std::string>(), std::string("Welt"));
    CPPUNIT_ASSERT_EQUAL(v2.get<std::string>(), std::string("Hallo"));

    //now swap them back with global swap
    swap(v1, v2);

    CPPUNIT_ASSERT_EQUAL(v1.get<std::string>(), std::string("Hallo"));
    CPPUNIT_ASSERT_EQUAL(v2.get<std::string>(), std::string("Welt"));

    //lets swap a int32_t with a Nothing
    nix::Value v3(42);
    nix::Value v4;

    v4.swap(v3);

    CPPUNIT_ASSERT_EQUAL(v4.get<int>(), 42);
    CPPUNIT_ASSERT_EQUAL(v3.type(), nix::DataType::Nothing);

    // and now with the string
    const std::string checkStr = v2.get<std::string>();
    v4.swap(v2);
    CPPUNIT_ASSERT_EQUAL(v2.get<int>(), 42);
    CPPUNIT_ASSERT_EQUAL(v4.get<std::string>(), checkStr);
}


