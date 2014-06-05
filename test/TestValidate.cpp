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

using namespace nix;
using namespace valid;
using namespace std;

void TestValidate::setUp() {
    file = File::open("test_validate.h5", FileMode::Overwrite);
    section = file.createSection("foo_section", "metadata");
    block1 = file.createBlock("block_one", "dataset");
    block2 = file.createBlock("block_two", "dataset");
    startup_time = time(NULL);
}

void TestValidate::tearDown() {
    file.close();
}

void TestValidate::test() {
    Result myResult = validate(std::initializer_list<condition> {
        must(block1, &Block::id, notEmpty(), "id is empty!"),
        must(block2, &Block::id, isEmpty(), "id is not empty!"),
        should(block2, &Block::id, isEmpty(), "id is not empty!")
    });

    cout << std::endl;
    cout << myResult;
}
