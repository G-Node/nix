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
#include "TestMultiTag.hpp"
#include "TestTag.hpp"
#include "TestValue.hpp"
#include "TestProperty.hpp"
#include "TestNDArray.hpp"
#include "TestUtil.hpp"
#include "TestImplContainer.hpp"
#include "TestDimension.hpp"
#include "TestFeature.hpp"
#include "TestDataAccess.hpp"
#include "TestOptionalObligatory.hpp"
#include "TestValidate.hpp"
#include "TestReadOnly.hpp"

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
    CPPUNIT_TEST_SUITE_REGISTRATION(TestDataAccess);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestDataArray);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestMultiTag);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestTag);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestValue);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestProperty);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestNDArray);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestUtil);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestImplContainer);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestDimension);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestFeature);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestOptionalObligatory);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestValidate);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestReadOnly);

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
