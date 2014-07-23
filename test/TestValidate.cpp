// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <ctime>
#include <functional>

#include <nix/valid/validator.hpp>
#include <nix/valid/checks.hpp>
#include <nix/valid/conditions.hpp>
#include <nix/valid/validate.hpp>

#include <nix.hpp>

#include "TestValidate.hpp"

using namespace nix;
using namespace valid;
using namespace std;

void TestValidate::setUp() {
    startup_time = time(NULL);
}

void TestValidate::tearDown() {
    return;
}

void TestValidate::test() {
    // dummy class to test empty checks
    class fooC {
    public:
        std::string getFoo () const { return std::string("I'm not empty!"); };
        std::string getBar () const { return std::string(); };
    };
    
    std::vector<std::string> vect = {"foo", "bar"};
    std::vector<std::string> vect2;
    fooC foobar;
    
    // NOTE: we can't test nix specific checks since nix & validation
    // structs cannot be included together
    valid::Result myResult = validator({
        could(vect, &std::vector<std::string>::empty, isFalse(), {
            must(vect, &std::vector<std::string>::size, notSmaller(2), "notSmaller(2)") }),
        must(vect, &std::vector<std::string>::size, notSmaller(2), "notSmaller(2)"),
        must(vect2, &std::vector<std::string>::size, isSmaller(2), "isSmaller(2)"),
        should(vect, &std::vector<std::string>::size, notGreater(2), "notGreater(2)"),
        should(vect, &std::vector<std::string>::size, isGreater(0), "isGreater(0)"),
        must(vect, &std::vector<std::string>::size, notEqual<size_t>(0), "notEqual<size_t>(0)"),
        should(vect, &std::vector<std::string>::size, isEqual<size_t>(2), "isEqual<size_t>(2)"),
        must(vect2, &std::vector<std::string>::size, isFalse(), "isFalse()"),
        must(foobar, &fooC::getFoo, notEmpty(), "notEmpty()"),
        should(foobar, &fooC::getBar, isEmpty(), "isEmpty()")    
    });
    // uncomment this to have debug info
    // std::cout << myResult;

    CPPUNIT_ASSERT(myResult.hasWarnings() == false);
    CPPUNIT_ASSERT(myResult.hasErrors() == false);
}
