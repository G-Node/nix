// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <ctime>

#include <nix/validate/validate.hpp>
#include "TestValidate.hpp"

using namespace std;
using namespace nix;

void TestValidate::setUp() {
    startup_time = time(NULL);
}

void TestValidate::tearDown() {
}

void TestValidate::test() {
    validate::test();
}
