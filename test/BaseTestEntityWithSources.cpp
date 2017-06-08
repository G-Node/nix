// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "BaseTestEntityWithSources.hpp"

#include <iterator>

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>

using namespace nix;


void BaseTestEntityWithSources::testSourceAccess() {
    std::vector<std::string> names = { "source_a", "source_b", "source_c", "source_d", "source_e" };

    CPPUNIT_ASSERT(block.sourceCount() == 0);
    CPPUNIT_ASSERT(block.sources().size() == 0);

    std::vector<std::string> ids;
    for (auto it = names.begin(); it != names.end(); it++) {
        Source src = block.createSource(*it, "channel");
        CPPUNIT_ASSERT(src.name() == *it);

        ids.push_back(src.id());
    }

    std::string name = names[0];
    Source source = block.getSource(name);
    CPPUNIT_ASSERT(source.name() == name);

    CPPUNIT_ASSERT(block.sourceCount() == names.size());
    CPPUNIT_ASSERT(block.sources().size() == names.size());

    for (auto it = ids.begin(); it != ids.end(); it++) {
        Source src = block.getSource(*it);
        CPPUNIT_ASSERT(block.hasSource(*it) == true);
        CPPUNIT_ASSERT(src.id() == *it);

        block.deleteSource(*it);
    }

    CPPUNIT_ASSERT(block.sourceCount() == 0);
    CPPUNIT_ASSERT(block.sources().size() == 0);
}


void BaseTestEntityWithSources::testSourceVectorSetter() {
    std::vector<std::string> names = { "source_a", "source_b", "source_c", "source_d", "source_e" };
    std::vector<Source> sources;
    DataArray da = block.createDataArray("Test","test", nix::DataType::Double, nix::NDSize {0,0});
    for (auto it = names.begin(); it != names.end(); it++) {
        sources.push_back(block.createSource(*it, "channel"));
    }
    CPPUNIT_ASSERT(block.sourceCount() == sources.size());
    CPPUNIT_ASSERT(da.sourceCount() == 0);

    da.addSource(sources[0]);
    da.addSource(sources[1]);
    CPPUNIT_ASSERT(da.sourceCount() == 2);

    da.sources(sources);
    CPPUNIT_ASSERT(da.sourceCount() == sources.size());

    sources.clear();
    sources.push_back(block.createSource("source_f", "channel"));
    sources.push_back(block.createSource("source_g", "channel"));
    da.sources(sources);
    CPPUNIT_ASSERT(da.sourceCount() == sources.size());
    CPPUNIT_ASSERT(block.sourceCount() == (sources.size() + names.size()));
    CPPUNIT_ASSERT_THROW(da.addSource(""), EmptyString);

    std::vector<Source> deleter;
    da.sources(deleter);
    CPPUNIT_ASSERT(da.sourceCount() == 0);
}

void BaseTestEntityWithSources::testSourceOrder() {
    std::vector<std::string> names = { "source_a", "source_b", "source_c", "source_d", "source_e" };
    std::vector<Source> sources;
    Group append_group = block.createGroup("ag", "test");
    Group set_group = block.createGroup("sg", "test");

    for (auto it = names.begin(); it != names.end(); it++) {
        sources.push_back(block.createSource(*it, "channel"));
    }

    // append each source in order
    for (auto src : block.sources()) {
        append_group.addSource(src);
    }

    // use the vector setter
    set_group.sources(sources);

    for (size_t idx = 0; idx < sources.size(); ++idx) {
        CPPUNIT_ASSERT(sources[idx].name() == append_group.getSource(idx).name());
        CPPUNIT_ASSERT(sources[idx].name() == set_group.getSource(idx).name());
    }

    sources.clear();
}
