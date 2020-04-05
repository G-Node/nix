// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "TestNDArray.hpp"
#include "TestNDSize.hpp"
#include "TestUtil.hpp"
#include "TestVariant.hpp"
#include "TestValue.hpp"
#include "TestVersion.hpp"
#include "TestOptionalObligatory.hpp"
#include "TestDataType.hpp"

#include "TestValidate.hpp"

#include "hdf5/TestH5.hpp"
#include "hdf5/TestEntityHDF5.hpp"
#include "hdf5/TestEntityWithMetadataHDF5.hpp"
#include "hdf5/TestEntityWithSourcesHDF5.hpp"
#include "hdf5/TestFileHDF5.hpp"
#include "hdf5/TestBlockHDF5.hpp"
#include "hdf5/TestSourceHDF5.hpp"
#include "hdf5/TestSectionHDF5.hpp"
#include "hdf5/TestDataSet.hpp"
#include "hdf5/TestH5Group.hpp"
#include "hdf5/TestDataArrayHDF5.hpp"
#include "hdf5/TestDataFrameHDF5.hpp"
#include "hdf5/TestBaseTagHDF5.hpp"
#include "hdf5/TestMultiTagHDF5.hpp"
#include "hdf5/TestTagHDF5.hpp"
#include "hdf5/TestPropertyHDF5.hpp"
#include "hdf5/TestImplContainerHDF5.hpp"
#include "hdf5/TestDimensionHDF5.hpp"
#include "hdf5/TestFeatureHDF5.hpp"
#include "hdf5/TestDataAccessHDF5.hpp"
#include "hdf5/TestReadOnlyHDF5.hpp"
#include "hdf5/TestGroupHDF5.hpp"

#ifdef ENABLE_FS_BACKEND
#include "fs/TestAttributesFS.hpp"
#include "fs/TestFileFS.hpp"
#include "fs/TestBlockFS.hpp"
#include "fs/TestEntityFS.hpp"
#include "fs/TestEntityWithMetadataFS.hpp"
#include "fs/TestEntityWithSourcesFS.hpp"
#include "fs/TestFeatureFS.hpp"
#include "fs/TestGroupFS.hpp"
#include "fs/TestMultiTagFS.hpp"
#include "fs/TestPropertyFS.hpp"
#include "fs/TestDataArrayFS.hpp"
#include "fs/TestSectionFS.hpp"
#include "fs/TestSourceFS.hpp"
#include "fs/TestTagFS.hpp"
#include "fs/TestBaseTagFS.hpp"
#include "fs/TestDimensionFS.hpp"
#endif

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
    CPPUNIT_TEST_SUITE_REGISTRATION(TestDataType);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestH5Group);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestDataAccessHDF5);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestDataArrayHDF5);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestDataFrameHDF5);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestBaseTagHDF5);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestMultiTagHDF5);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestTagHDF5);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestVariant);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestValue);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestPropertyHDF5);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestNDArray);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestNDSize);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestUtil);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestImplContainerHDF5);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestDimensionHDF5);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestFeatureHDF5);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestOptionalObligatory);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestValidate);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestVersion);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestReadOnlyHDF5);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestGroupHDF5);

#ifdef ENABLE_FS_BACKEND
    CPPUNIT_TEST_SUITE_REGISTRATION(TestAttributesFS);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestFileFS);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestBlockFS);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestEntityFS);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestEntityWithMetadataFS);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestEntityWithSourcesFS);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestFeatureFS);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestGroupFS);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestMultiTagFS);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestPropertyFS);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestDataArrayFS);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestSectionFS);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestSourceFS);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestTagFS);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestBaseTagFS);
    CPPUNIT_TEST_SUITE_REGISTRATION(TestDimensionFS);
#endif

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
