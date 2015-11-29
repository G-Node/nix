// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "TestH5.hpp"
#include "TestEntityHDF5.hpp"
#include "TestEntityWithMetadataHDF5.hpp"
#include "TestEntityWithSourcesHDF5.hpp"
#include "TestFileHDF5.hpp"
#include "TestBlockHDF5.hpp"
#include "TestSourceHDF5.hpp"
#include "TestSectionHDF5.hpp"
#include "TestDataSet.hpp"
#include "TestH5Group.hpp"
#include "TestDataArrayHDF5.hpp"
#include "TestBaseTagHDF5.hpp"
#include "TestMultiTagHDF5.hpp"
#include "TestTagHDF5.hpp"
#include "TestValue.hpp"
#include "TestPropertyHDF5.hpp"
#include "TestNDArray.hpp"
#include "TestUtil.hpp"
#include "TestImplContainerHDF5.hpp"
#include "TestDimensionHDF5.hpp"
#include "TestFeatureHDF5.hpp"
#include "TestDataAccessHDF5.hpp"
#include "TestOptionalObligatory.hpp"
#include "TestValidate.hpp"
#include "TestReadOnlyHDF5.hpp"
#include "TestGroupHDF5.hpp"
#include "TestAttributesFS.hpp"

int main(int argc, char* argv[]) {
    CPPUNIT_TEST_SUITE_REGISTRATION(TestH5);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestEntityHDF5);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestEntityWithMetadataHDF5);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestEntityWithSourcesHDF5);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestFileHDF5);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestBlockHDF5);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestSourceHDF5);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestSectionHDF5);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestDataSet);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestH5Group);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestDataAccessHDF5);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestDataArrayHDF5);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestBaseTagHDF5);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestMultiTagHDF5);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestTagHDF5);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestValue);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestPropertyHDF5);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestNDArray);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestUtil);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestImplContainerHDF5);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestDimensionHDF5);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestFeatureHDF5);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestOptionalObligatory);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestValidate);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestReadOnlyHDF5);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestGroupHDF5);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestAttributesFS);

    CPPUNIT_NS::TestResult testresult;
    CPPUNIT_NS::TestResultCollector collectedresults;
    testresult.addListener(&collectedresults);

    CPPUNIT_NS::BriefTestProgressListener progress;
    testresult.addListener(&progress);

    CPPUNIT_NS::TestRunner testrunner;
    CPPUNIT_NS::TestFactoryRegistry &registry = CPPUNIT_NS::TestFactoryRegistry::getRegistry();

    CPPUNIT_NS::Test *test = registry.makeTest();;

    //if XXX or TestXXX was supplied, only run that specific test
    //by passing the name to TestRunner::run()
    std::string testName;

    if (argc > 1) {
        testName = argv[1];
        static std::string testPrefix = "Test";

        if (testName.compare(0, 4, testPrefix)) {
            testName = testPrefix + testName;
        }
        std::cout << testName << std::endl;
    }

    testrunner.addTest(test);
    testrunner.run(testresult, testName);

    CPPUNIT_NS::CompilerOutputter compileroutputter(&collectedresults, std::cerr);
    compileroutputter.write();
    
    std::cout << "\n";

    return !collectedresults.wasSuccessful();
}
