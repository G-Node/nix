// Copyright © 2015 Adrian Stoewer <adrian.stoewer@rz.ifi.lmu.de>
//                  Jan Grewe <jan.grewe@g-node.org>
//                  German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/util/util.hpp>

#include <iterator>

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include <boost/math/constants/constants.hpp>

#include "BaseTestBaseTag.hpp"

using namespace nix;
using namespace valid;
using namespace std;

//TODO Constraints on References are not tested yet.

void BaseTestBaseTag::testReferences() {
    DataArray da_1 = block.createDataArray("TestReference 1","Reference",
            DataType::Double, nix::NDSize({ 0 }));
    DataArray da_2 = block.createDataArray("TestReference 2","Reference",
            DataType::Double, nix::NDSize({ 0 }));

    Tag st = block.createTag("TestTag1", "Tag", {0.0, 2.0, 3.4});
    st.references(refs);

    CPPUNIT_ASSERT_THROW(st.getReference(42), nix::OutOfBounds);

    std::stringstream counterrmsg;
    counterrmsg << "BaseTestMultiTag::testReference: Counts do not match!";
    CPPUNIT_ASSERT_MESSAGE(counterrmsg.str(), st.referenceCount() == 5);

    st.addReference(da_1.id());
    st.addReference(da_2.id());

    CPPUNIT_ASSERT_MESSAGE(counterrmsg.str(), st.referenceCount() == 7);

    std::stringstream retrieveerrmsg;
    DataArray ref1 = st.getReference(da_1.id());
    retrieveerrmsg << "TestTag::testReference: Retrieval did not work!";
    CPPUNIT_ASSERT_MESSAGE(retrieveerrmsg.str(), ref1.id() == da_1.id());
    DataArray ref2 = st.getReference(da_1.name());
    retrieveerrmsg << "TestTag::testReference: Retrieval by name did not work!";
    CPPUNIT_ASSERT_MESSAGE(retrieveerrmsg.str(), ref2.id() == da_1.id());

    std::vector<DataArray> arrays = st.references();
    CPPUNIT_ASSERT_MESSAGE(retrieveerrmsg.str(), arrays.size() == 7);

    std::stringstream hasReferrmsg;
    hasReferrmsg << "TestTag::testReference: hadReference did not work!";
    CPPUNIT_ASSERT_MESSAGE(hasReferrmsg.str(), st.hasReference(da_1.id()));
    CPPUNIT_ASSERT_MESSAGE(hasReferrmsg.str(), st.hasReference(da_2.id()));

    std::stringstream delReferrmsg;
    delReferrmsg << "TestTag::testReference: removeReference did not work!";
    st.removeReference(da_1.id());
    CPPUNIT_ASSERT_MESSAGE(delReferrmsg.str(), st.referenceCount() == 6);
    st.removeReference(da_2.id());
    CPPUNIT_ASSERT_MESSAGE(delReferrmsg.str(), st.referenceCount() == 5);

    // delete data arrays
    std::vector<std::string> ids;
    block.deleteDataArray(da_1.id());
    block.deleteDataArray(da_2.id());
    for (auto it = refs.begin(); it != refs.end(); it++) {
        ids.push_back((*it).id());
        block.deleteDataArray((*it).id());
    }
    // check if references are gone too!
    CPPUNIT_ASSERT(st.referenceCount() == 0);
    for (auto ref_id : ids) {
        CPPUNIT_ASSERT(!st.hasReference(ref_id));
    }
    block.deleteTag(st.id());
}


void BaseTestBaseTag::testFeatures() {
    Tag st = block.createTag("TestTag", "tag", {10.0});
    DataArray da1 = block.createDataArray("featureArray1", "test", nix::DataType::Double, {1});
    DataArray da2 = block.createDataArray("featureArray2", "test", nix::DataType::Double, {1});
    DataArray da3 = block.createDataArray("featureArray3", "test", nix::DataType::Double, {1});

    int failures = 0;
    for (int i = 0; i < 20; i++) {
        Feature f1 = st.createFeature(da1, nix::LinkType::Untagged);
        Feature f2 = st.createFeature(da2, nix::LinkType::Tagged);
        Feature f3 = st.createFeature(da3, nix::LinkType::Untagged);
        if (st.getFeature(0).id() != f1.id() || st.getFeature(1).id() != f2.id() || st.getFeature(2).id() != f3.id()) {
            failures ++;
        }
        st.deleteFeature(f1.id());
        st.deleteFeature(f2.id());
        st.deleteFeature(f3.id());
    }
    block.deleteTag(st.id());
    block.deleteDataArray(da1.id());
    block.deleteDataArray(da2.id());
    block.deleteDataArray(da3.id());
    CPPUNIT_ASSERT(failures == 0);
}
