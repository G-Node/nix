// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <iostream>
#include <stdexcept>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>

#include "TestEntity.hpp"
#include "TestEntityWithMetadata.hpp"
#include "TestEntityWithSources.hpp"
#include "TestFile.hpp"
#include "TestBlock.hpp"
#include "TestSource.hpp"
#include "TestSection.hpp"
#include "TestDataSet.hpp"
#include "TestGroup.hpp"
#include "TestDataArray.hpp"
#include "TestDataTag.hpp"
#include "TestSimpleTag.hpp"
#include "TestValue.hpp"
#include "TestProperty.hpp"
#include "TestNDArray.hpp"
#include "TestUtil.hpp"
#include "TestImplContainer.hpp"

int main(int argc, char* argv[]) {
	CPPUNIT_TEST_SUITE_REGISTRATION(TestEntity);
	CPPUNIT_TEST_SUITE_REGISTRATION(TestEntityWithMetadata);
	CPPUNIT_TEST_SUITE_REGISTRATION(TestEntityWithSources);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestFile);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestBlock);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestSource);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestSection);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestDataSet);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestGroup);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestDataArray);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestDataTag);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestSimpleTag);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestValue);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestProperty);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestNDArray);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestUtil);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestImplContainer);
    
    CPPUNIT_NS::TestResult testresult;
    CPPUNIT_NS::TestResultCollector collectedresults;
    testresult.addListener(&collectedresults);

    CPPUNIT_NS::BriefTestProgressListener progress;
    testresult.addListener(&progress);

    CPPUNIT_NS::TestRunner testrunner;
    CPPUNIT_NS::TestFactoryRegistry &registry = CPPUNIT_NS::TestFactoryRegistry::getRegistry();

    CPPUNIT_NS::Test *test = registry.makeTest();;

    if (argc > 1) {
        std::string testName = argv[1];
        static std::string testPrefix = "Test";

        if (testName.compare(0, 4, testPrefix)) {
            testName = testPrefix + testName;
        }
        std::cout << testName << std::endl;

        try {
            test = test->findTest(testName);
        } catch (std::invalid_argument &iae) {
            std::cerr << "Test not found: \"" << argv[1] << "\". Aborting." << std::endl;
            return -1;
        }
    }

    testrunner.addTest(test);
    testrunner.run(testresult);

    CPPUNIT_NS::CompilerOutputter compileroutputter(&collectedresults, std::cerr);
    compileroutputter.write();

    return !collectedresults.wasSuccessful();
}
