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

#include <cstring>

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

    const char long_string[] = "When shall we three meet again"
                               "In thunder, lightning, or in rain?";

    sVal.set(long_string);

    nix::Value v1 = sVal; //copy constructor
    CPPUNIT_ASSERT_EQUAL(sVal, v1); // check ==
    CPPUNIT_ASSERT(v1 != dVal);

    CPPUNIT_ASSERT_EQUAL(v1.get<std::string>(), std::string(long_string));

    v1.set(long_string, strc.length()); // strc, long_string share this prefix
    CPPUNIT_ASSERT_EQUAL(v1.get<std::string>(), strc);

    v1 = dVal; //assignment op (copy)
    CPPUNIT_ASSERT_EQUAL(v1.get<double>(), dc);

    v1 = nix::Value(31337); //assignment op (move)
    CPPUNIT_ASSERT_EQUAL(v1.get<int>(), 31337);

    v1.set(nix::none); //set v1 to none
    CPPUNIT_ASSERT_EQUAL(v1.type(), nix::DataType::Nothing);

    //the rest of the supports_type test are in ValTester::check_basic, below
    CPPUNIT_ASSERT_EQUAL(false, nix::Value::supports_type(nix::DataType::Opaque));
}

struct ValTester {

    virtual void check_basic() const = 0;
    virtual void check_swap(const ValTester &other) const = 0;
    virtual void check_to_string() const = 0;
    virtual bool value_same(const nix::Value &other) const = 0;

    virtual nix::Value theVal() const = 0;
    virtual nix::DataType type() const = 0;

    virtual ~ValTester() { };
};

template<typename T>
struct ValueTester : ValTester {
    typedef T value_type;

    ValueTester(const T& v) : value(v), dtype(nix::to_data_type<T>::value), val(v) { }

    void check_basic() const override {
        CPPUNIT_ASSERT(nix::Value::supports_type(dtype));
        CPPUNIT_ASSERT_EQUAL(dtype, value.type());
        CPPUNIT_ASSERT_EQUAL(val, value.get<T>());
    }

    void check_swap(const ValTester &other) const override {
        nix::Value a = value;
        nix::Value b = other.theVal();

        a.swap(b);

        CPPUNIT_ASSERT_EQUAL(b.type(), dtype);
        CPPUNIT_ASSERT_EQUAL(a.type(), other.type());

        CPPUNIT_ASSERT_EQUAL(b.get<T>(), val);
        CPPUNIT_ASSERT(other.value_same(a));

        CPPUNIT_ASSERT_EQUAL(other.type() == dtype, other.theVal() == value);
    }

    void check_to_string() const override {
        std::stringstream val_stream;

        val_stream << val;

        std::stringstream value_stream;

        value_stream << value;

        std::string::size_type pos = value_stream.str().find(val_stream.str());
        CPPUNIT_ASSERT(pos != std::string::npos);
    }

    nix::Value theVal() const override {
        return value;
    }

    nix::DataType type() const override {
        return dtype;
    }

    bool value_same(const nix::Value &other) const override {
        return other.type() == dtype && other.get<T>() == val;
    }

    nix::Value    value;
    nix::DataType dtype;
    T             val;
};

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

    //now lets do that systematically

    std::vector<ValTester *> vals = { new ValueTester<std::string>("String"),
                                      new ValueTester<uint32_t>(42),
                                      new ValueTester< int32_t>(42),
                                      new ValueTester<uint64_t>(42),
                                      new ValueTester< int64_t>(42),
                                      new ValueTester< double>(42.0),
                                      new ValueTester< bool>(true)};

    for (const ValTester *a : vals) {
        a->check_basic();
        a->check_to_string();

        for (const ValTester *b : vals) {
            a->check_swap(*b);
        }
    }

    for (ValTester *vt : vals) {
        delete vt;
    }
}

void TestValue::testEquals() {
    nix::Value v1("Hallo");
    nix::Value v2("Welt");
    v1.uncertainty = 20;

    CPPUNIT_ASSERT(v2 != v1);
    v2 = v1;
    CPPUNIT_ASSERT(v2 == v1);
    v2.uncertainty = 10;
    CPPUNIT_ASSERT(v2 != v1);
}


