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
    // id must not be false
    auto myProperty1 = [&](){ return block.id(); };
    auto myCheck1 = notFalse();
    auto myCondition1 = must(myProperty1, myCheck1, "id is false!");
    
    // id must be false
    auto myCheck2 = isFalse();
    auto myCondition2 = must(myProperty1, myCheck2, "id is not false!");
    
    // id should be false
    auto myCondition3 = should(myProperty1, myCheck2, "id is not false!");
        
    vector<conditionType> myConditions {myCondition1, myCondition2, myCondition3};
    
    Result myResult = validator(myConditions);
    
    cout << endl;
    cout << myResult;
}
