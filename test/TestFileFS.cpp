// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "TestFileFS.hpp"

#include <nix/util/util.hpp>
#include <nix/valid/validate.hpp>

#include <ctime>


using namespace std;
using namespace nix;
using namespace valid;


void TestFileFS::setUp() {
    startup_time = time(NULL);
    file = File::open("test_file", FileMode::Overwrite, Implementation::FileSys);
}


void TestFileFS::tearDown() {
    boost::filesystem::remove_all(boost::filesystem::path(file.location().c_str()));
}


void TestFileFS::testOpenCreate() {
    boost::filesystem::exists(boost::filesystem::path(file.location().c_str()));
}

