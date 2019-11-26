// Copyright (c) 2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_TESTDATAACCESSHDF5_HPP
#define NIX_TESTDATAACCESSHDF5_HPP

#include "BaseTestDataAccess.hpp"

#include <cppunit/TestFixture.h>

class TestDataAccessHDF5 : public BaseTestDataAccess {

    CPPUNIT_TEST_SUITE(TestDataAccessHDF5);
    CPPUNIT_TEST(testPositionToIndexSampledDimension);
    CPPUNIT_TEST(testPositionToIndexSetDimension);
    CPPUNIT_TEST(testPositionToIndexRangeDimension);
    CPPUNIT_TEST(testOffsetAndCount);
    CPPUNIT_TEST(testPositionInData);
    CPPUNIT_TEST(testRetrieveData);
    CPPUNIT_TEST(testTagFeatureData);
    CPPUNIT_TEST(testMultiTagFeatureData);
    CPPUNIT_TEST(testMultiTagUnitSupport);
    CPPUNIT_TEST(testDataView);
    CPPUNIT_TEST(testDataSlice);
    CPPUNIT_TEST(testFlexibleTagging);
    CPPUNIT_TEST_SUITE_END ();

public:

    void setUp() {
        file = nix::File::open("test_dataAccess.h5", nix::FileMode::Overwrite);
        block = file.createBlock("dimensionTest","test");
        data_array = block.createDataArray("dimensionTest",
                                           "test",
                                           nix::DataType::Double,
                                           nix::NDSize({0, 0, 0}));
        double samplingInterval = 1.0;
        std::vector<double> ticks {1.2, 2.3, 3.4, 4.5, 6.7};
        std::string unit = "ms";

        typedef boost::multi_array<double, 3> array_type;
        typedef array_type::index index;
        array_type data(boost::extents[2][10][5]);
        int value;
        for(index i = 0; i != 2; ++i) {
            value = 0;
            for(index j = 0; j != 10; ++j) {
                for(index k = 0; k != 5; ++k) {
                    data[i][j][k] = value++;
                }
            }
        }
        data_array.setData(data);

        setDim = data_array.appendSetDimension();
        std::vector<std::string> labels = {"label_a", "label_b"};
        setDim.labels(labels);

        sampledDim = data_array.appendSampledDimension(samplingInterval);
        sampledDim.unit(unit);

        rangeDim = data_array.appendRangeDimension(ticks);
        rangeDim.unit(unit);

        std::vector<nix::DataArray> refs;
        refs.push_back(data_array);
        std::vector<double> position {0.0, 2.0, 3.4};
        std::vector<double> extent {0.0, 6.0, 2.3};
        std::vector<std::string> units {"none", "ms", "ms"};

        position_tag = block.createTag("position tag", "event", position);
        position_tag.references(refs);
        position_tag.units(units);

        segment_tag = block.createTag("region tag", "segment", position);
        segment_tag.references(refs);
        segment_tag.extent(extent);
        segment_tag.units(units);

        //setup multiTag
        typedef boost::multi_array<double, 2> position_type;
        position_type event_positions(boost::extents[2][3]);
        position_type event_extents(boost::extents[2][3]);
        event_positions[0][0] = 0.0;
        event_positions[0][1] = 3.0;
        event_positions[0][2] = 3.4;

        event_extents[0][0] = 0.0;
        event_extents[0][1] = 6.0;
        event_extents[0][2] = 2.3;

        event_positions[1][0] = 0.0;
        event_positions[1][1] = 8.0;
        event_positions[1][2] = 2.3;

        event_extents[1][0] = 0.0;
        event_extents[1][1] = 3.0;
        event_extents[1][2] = 2.0;

        std::vector<std::string> event_labels = {"event 1", "event 2"};
        std::vector<std::string> dim_labels = {"dim 0", "dim 1", "dim 2"};

        nix::DataArray event_array = block.createDataArray("positions", "test",
                                                           nix::DataType::Double, nix::NDSize({ 0, 0 }));
        event_array.setData(event_positions);
        nix::SetDimension event_set_dim;
        event_set_dim = event_array.appendSetDimension();
        event_set_dim.labels(event_labels);
        event_set_dim = event_array.appendSetDimension();
        event_set_dim.labels(dim_labels);

        nix::DataArray extent_array = block.createDataArray("extents", "test",
                                                            nix::DataType::Double, nix::NDSize({ 0, 0 }));
        extent_array.setData(event_extents);
        nix::SetDimension extent_set_dim;
        extent_set_dim = extent_array.appendSetDimension();
        extent_set_dim.labels(event_labels);
        extent_set_dim = extent_array.appendSetDimension();
        extent_set_dim.labels(dim_labels);

        multi_tag = block.createMultiTag("multi_tag", "events", event_array);
        multi_tag.extents(extent_array);
        multi_tag.addReference(data_array);

        alias_array = block.createDataArray("alias array", "event times",
                                            nix::DataType::Double, nix::NDSize({ 100 }));
        std::vector<double> times(100);
        for (size_t i = 0; i < 100; i++) {
            times[i] = 1.3 * i;
        }
        alias_array.setData(times, nix::NDSize({ 0 }));
        alias_array.unit("ms");
        alias_array.label("time");
        aliasDim = alias_array.appendAliasRangeDimension();
        std::vector<double> segment_time(1, 4.5);
        times_tag = block.createTag("stimulus on", "segment", {4.5});
        times_tag.extent({100.0});
        times_tag.units({"ms"});
        times_tag.addReference(alias_array);

        // prepare a mtag with multiple valid segments
        std::vector<double> segment_starts{1.0, 5.0, 10.0, 15.0, 20.0};
        std::vector<double> segment_extents(5, 2.5);
        double sampling_interval = 0.1;
        std::vector<double> sinus((int)(25 / sampling_interval));
        for (size_t i = 0; i < sinus.size(); ++i) {
            sinus[i] = sin(i * 0.1  * 6.28);
        }

        nix::DataArray seg_starts = block.createDataArray("sinus_starts", "test",
                                                          nix::DataType::Double,
                                                          nix::NDSize(1,segment_starts.size()));
        seg_starts.setData(segment_starts);
        seg_starts.appendSetDimension();

        nix::DataArray seg_extents = block.createDataArray("sinus_extents", "test",
                                                           nix::DataType::Double,
                                                           nix::NDSize(1,segment_starts.size()));
        seg_extents.setData(segment_extents);
        seg_extents.appendSetDimension();

        nix::DataArray sinus_array = block.createDataArray("sinus", "test", nix::DataType::Double,
                                                           nix::NDSize(1, sinus.size()));
        sinus_array.setData(sinus);
        nix::SampledDimension sd = sinus_array.appendSampledDimension(sampling_interval);
        sd.unit("s");
        sd.label("time");

        mtag2 = block.createMultiTag("sinus_segments", "test", seg_starts);
        mtag2.extents(seg_extents);
        mtag2.addReference(sinus_array);

        pointmtag = block.createMultiTag("sinus_points", "test", seg_starts);
        pointmtag.addReference(sinus_array);
    }

    void tearDown() {
        file.close();
    }

};

#endif //NIX_TESTDATAACCESSHDF5_HPP
