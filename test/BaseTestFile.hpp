// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix.hpp>

#include <iostream>
#include <sstream>
#include <iterator>
#include <stdexcept>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>


class BaseTestFile : public CPPUNIT_NS::TestFixture {

protected:
    nix::File file_open, file_other, file_null, file_fs, file_other_fs;
    time_t startup_time;


    void test_section_access(nix::File &f);
    void test_block_access(nix::File &f);

public:

    void testOpen();
    void testValidate();
    void testFormat();
    void testLocation();
    void testVersion();
    void testCreatedAt();
    void testUpdatedAt();
    void testBlockAccess();
    void testSectionAccess();
    void testOperators();
    void testReopen();
    void testCheckHeader();
};
