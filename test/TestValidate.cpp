// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <ctime>
#include <functional>
#include "TestValidate.hpp"

using namespace std;
using namespace nix;
using namespace validate;

void TestValidate::setUp() {
    file = File::open("test_validate.h5", FileMode::Overwrite);
    section = file.createSection("foo_section", "metadata");
    block = file.createBlock("block_one", "dataset");
    startup_time = time(NULL);
}

void TestValidate::tearDown() {
    file.close();
}

void TestValidate::test() {    
    auto t = block.id();
    typedef decltype(t) T;
    typedef function<T()> TFUNC;
    
    // Check that id is not false
    std::function<bool(T)> myCheck1 = notFalse<T>();
    condition<TFUNC, T>::type myConditionFunc1 = must<TFUNC, T>;
    conditionType myCondition1 = myConditionFunc1([&](){ return block.id(); }, myCheck1, "foo is going on!");
    
    // Check that id is false
    std::function<bool(T)> myCheck2 = isFalse<T>();
    condition<TFUNC, T>::type myConditionFunc2 = must<TFUNC, T>;
    conditionType myCondition2 = myConditionFunc2([&](){ return block.id(); }, myCheck2, "foo is NOT going on!");
        
    vector<conditionType> myConditions {myCondition1, myCondition2};
    
    Result myResult = validator(myConditions);
    
    std::cout << myResult;
}
