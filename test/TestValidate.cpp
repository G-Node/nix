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
    
    // id must not be false
    auto myProperty1 = [&](){ return block.id(); };
    std::function<bool(T)> myCheck1 = notFalse<T>();
    condition<TFUNC, T>::type myConditionFunc1 = must<TFUNC, T>;
    conditionType myCondition1 = myConditionFunc1(myProperty1, myCheck1, "id is false!");
    
    // id must be false
    std::function<bool(T)> myCheck2 = isFalse<T>();
    conditionType myCondition2 = myConditionFunc1(myProperty1, myCheck2, "id is not false!");
    
    // id should be false
    condition<TFUNC, T>::type myConditionFunc2 = should<TFUNC, T>;
    conditionType myCondition3 = myConditionFunc2(myProperty1, myCheck2, "id is not false!");
        
    vector<conditionType> myConditions {myCondition1, myCondition2, myCondition3};
    
    Result myResult = validator(myConditions);
    
    cout << endl;
    cout << myResult;
}
