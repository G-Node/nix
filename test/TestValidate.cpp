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
using namespace validation;

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
    auto myProperty1 = [&](){ return block.id(); };
    auto myCheck1 = notFalse();
    auto myCheck2 = isFalse();
    
        
    Result myResult = validate(vector<condition> {
        must(myProperty1, myCheck1, "id is false!"), 
        must(myProperty1, myCheck2, "id is not false!"), 
        should(myProperty1, myCheck2, "id is not false!")
    });
    
    cout << endl;
    cout << myResult;
}
